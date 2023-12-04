//==============================================================================
//
//  VideoCard realization   
//
//==============================================================================
//
//  externals: 

    #include "VideoCard.hpp"
    #include <winuser.h>


namespace core
{
//------------------------------------------------------------------------------
//      
//  publics:

    extern ProgramCore*         Core;

    std::vector<DDSURFACEDESC>  ModeInfo;


//------------------------------------------------------------------------------

HRESULT WINAPI
enum_callback( LPDDSURFACEDESC surf_desc, LPVOID data )
{
    ModeInfo.push_back( *surf_desc );
    return DDENUMRET_OK;
}


//------------------------------------------------------------------------------

VideoCard::VideoCard()
  : fullscreen_mode(false)
{   
    try_ddraw
    (
        Core->ddraw()->SetCooperativeLevel( Core->window(), DDSCL_NORMAL )
    );
    
    DDSURFACEDESC           surf_desc;
    DDSCAPS                 surf_caps;
    LPDIRECTDRAWSURFACE     surf1;

    ZeroMemory( &surf_desc, sizeof(surf_desc) );
    surf_desc.dwSize            = sizeof( surf_desc );
    surf_desc.dwFlags           = DDSD_CAPS;
    surf_desc.ddsCaps.dwCaps    = DDSCAPS_PRIMARYSURFACE;

    try_ddraw(
               Core->ddraw()->CreateSurface( &surf_desc,
                                             &surf1,
                                             NULL
                                           )
               __CT("creating primary surface")
             );
    try_ddraw(
               surf1->QueryInterface( IID_IDirectDrawSurface2,
                                      (LPVOID *)&front_buf.surf2
                                    )
               __CT("getting DDsurface2 of primary surface")
             );
    surf1->Release();
    
    
    // create back buffer for window-mode

    RECT        rect;

    GetWindowRect( Core->window(), &rect );
    
    ZeroMemory( &surf_desc, sizeof(surf_desc) );
    surf_desc.dwSize            = sizeof( surf_desc );
    surf_desc.dwFlags           = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
    surf_desc.ddsCaps.dwCaps    = DDSCAPS_OFFSCREENPLAIN;
    surf_desc.dwWidth           = rect.right - rect.left;
    surf_desc.dwHeight          = rect.bottom - rect.top;

    try_ddraw(
               Core->ddraw()->CreateSurface( &surf_desc,
                                             &surf1,
                                             NULL
                                           )
               __CT("creating wnd back buf")
             );
    try_ddraw(
               surf1->QueryInterface( IID_IDirectDrawSurface2,
                                      (LPVOID *)&wnd_back_buf.surf2
                                    )
               __CT("getting DDsurface2 of wnd backbuf surface")
             );
    surf1->Release();
    
    front_buf.update_info();
    wnd_back_buf.update_info();
    
    
    try_ddraw
    (
        Core->ddraw()->EnumDisplayModes( DDEDM_REFRESHRATES,
                                         NULL,
                                         NULL,
                                         &enum_callback
                                       )
        __CT("enumin' vid-modes" )    
    );

    for( int i=0; i<ModeInfo.size(); ++i )
    {    
        mode    m = { ModeInfo[i].dwWidth,
                      ModeInfo[i].dwHeight,
                      ModeInfo[i].ddpfPixelFormat.dwRGBBitCount,
                      ModeInfo[i].dwRefreshRate
                    };
        /*
        this->mode_info.push_back( m );
        
        std::clog << m.xres << "x" << m.yres << "x" << m.bpp 
                  << "@" << m.freq << "\n" << std::flush;
        */
    } 
    //ZeroMemory( front_buf.memory(), front_buf.pitch() * front_buf.height() );
    
    ZeroMemory( wnd_back_buf.memory(), 
                wnd_back_buf.pitch() * wnd_back_buf.height() 
              );
    wnd_back_buf.unlock();
}


//------------------------------------------------------------------------------

VideoCard::~VideoCard()
{
}


//------------------------------------------------------------------------------

void 
VideoCard::set_mode( int xres, int yres, int bpp, int freq )
{
    if( front_buf.surf2 != NULL )
        front_buf.surf2->Release(), front_buf.surf2 = NULL;
    //if( back_buf.surf2 )
    //    back_buf.surf2->Release(), back_buf.surf2 = NULL;
    if( wnd_back_buf.surf2 != NULL )
        wnd_back_buf.surf2->Release(), wnd_back_buf.surf2 = NULL;
    
    try_ddraw(
               Core->ddraw()->SetCooperativeLevel( Core->window(), 
                                                   DDSCL_EXCLUSIVE | 
                                                   DDSCL_FULLSCREEN
                                                 )
               __CT("setting exclusive coop level")
             );
    
    
    // set video-mode with max possible refresh rate
    
    int     refresh = freq;

    if( refresh )
    {
        int freq = 0;
        
        for( int i=0; i<mode_info.size(); i++ )
        {
            if( mode_info[i].xres ==  xres  &&
                mode_info[i].yres ==  yres  &&
                mode_info[i].bpp  ==  bpp  
              )
            {
                if( mode_info[i].freq > freq )
                    freq = mode_info[i].freq;
            }
        }

        if( freq < refresh )
            refresh = freq;
    }
    
    try_ddraw( 
               Core->ddraw()->SetDisplayMode( xres, yres, bpp, refresh, 0 )
               __CT("setting vid-mode")
             );
    fullscreen_mode = true;

    DWORD   vfreq;
    try_ddraw(
                Core->ddraw()->GetMonitorFrequency( &vfreq )
             );
    std::clog << "actual v-greq is " << vfreq << "Hz\n" ;


    DDSURFACEDESC           surf_desc;
    DDSCAPS                 surf_caps;
    LPDIRECTDRAWSURFACE     surf1;
    //LPDIRECTDRAWSURFACE2    surf2;
    
    ZeroMemory( &surf_desc, sizeof(surf_desc) );
    surf_desc.dwSize            = sizeof( surf_desc );
    surf_desc.dwFlags           = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    surf_desc.ddsCaps.dwCaps    = DDSCAPS_PRIMARYSURFACE |
                                  DDSCAPS_FLIP |
                                  DDSCAPS_COMPLEX;
                                  //DDSCAPS_VIDEOMEMORY;
    surf_desc.dwBackBufferCount = 1;

    try_ddraw(
               Core->ddraw()->CreateSurface( &surf_desc, 
                                             &surf1, 
                                             NULL 
                                           )
               __CT("creating primary surface")
             );
    try_ddraw(
               surf1->QueryInterface( IID_IDirectDrawSurface2, 
                                      (LPVOID *)&front_buf.surf2
                                    )
               __CT("getting DDsurface2 of primary surface")
             );
    surf1->Release();
    
    ZeroMemory( &surf_caps, sizeof(surf_caps) );
    //surf_caps.dwSize = sizeof( surf_caps );
    surf_caps.dwCaps = DDSCAPS_BACKBUFFER;

    try_ddraw(
               front_buf.surf2->GetAttachedSurface( &surf_caps, 
                                                    &back_buf.surf2
                                                  )
               __CT("gettin' back buffer")
             );

    //back_buf.surf2->AddRef();

    front_buf.update_info();
    back_buf.update_info();
    ZeroMemory( &Surface::prim_surf_pixelformat, sizeof(DDPIXELFORMAT) );
    Surface::prim_surf_pixelformat.dwSize = sizeof(DDPIXELFORMAT);
    try_ddraw(
                front_buf.surf2->GetPixelFormat(&Surface::prim_surf_pixelformat)
               __CT("gettin' pixel format of primary surface")
             );

    ZeroMemory( front_buf.memory(), front_buf.pitch() * front_buf.height() );
    ZeroMemory( back_buf.memory(), back_buf.pitch() * back_buf.height() );
}


//------------------------------------------------------------------------------

bool 
VideoCard::vsync_in_progress()
{
    BOOL    vsync;
    
    try_ddraw(
               (Core->ddraw())->GetVerticalBlankStatus( &vsync )
               __CT("gettin' Vsync status")
             );

    return (vsync==TRUE)  ? true  : false;
}


//------------------------------------------------------------------------------

void 
VideoCard::wait_for_vsync()
{
    while( !vsync_in_progress() );
}


//------------------------------------------------------------------------------

void                
VideoCard::resize_wnd_back_buffer()
{
    if( fullscreen_mode )
        return;
    
    DDSURFACEDESC           surf_desc;
    DDSCAPS                 surf_caps;
    LPDIRECTDRAWSURFACE     surf1;
    RECT                    rect;
    
    GetWindowRect( Core->window(), &rect );

    if( rect.right-rect.left == wnd_back_buf.width()  ||  
        rect.bottom-rect.top == wnd_back_buf.height() 
      )
    {
        return;
    }

    if( wnd_back_buf.surf2 != NULL )
        wnd_back_buf.surf2->Release(), wnd_back_buf.surf2 = NULL;
    
    ZeroMemory( &surf_desc, sizeof(surf_desc) );
    
    surf_desc.dwSize            = sizeof( surf_desc );
    surf_desc.dwFlags           = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
    surf_desc.ddsCaps.dwCaps    = DDSCAPS_OFFSCREENPLAIN;
    surf_desc.dwWidth           = rect.right - rect.left;
    surf_desc.dwHeight          = rect.bottom - rect.top;

    try_ddraw(
               Core->ddraw()->CreateSurface( &surf_desc,
                                             &surf1,
                                             NULL
                                           )
               __CT("creating wnd back buf")
             );
    try_ddraw(
               surf1->QueryInterface( IID_IDirectDrawSurface2,
                                      (LPVOID *)&wnd_back_buf.surf2
                                    )
               __CT("getting DDsurface2 of wnd backbuf surface")
             );
    surf1->Release();
    
    wnd_back_buf.update_info();
    
}


//------------------------------------------------------------------------------

void
VideoCard::flip_pages( ScreenBuffer::FlipMethod method )
{
    if( fullscreen_mode )
    {
        front_buf.unlock();
        back_buf.unlock();
        
        if( method == ScreenBuffer::vsync )
        {
            front_buf.flip( ScreenBuffer::vsync );
        }
        else
        {
            while( !front_buf.blit_done() )
                ;
            
            front_buf.flip( ScreenBuffer::no_vsync );
        }

    }
    else
    {
        RECT    rect;

        GetWindowRect( Core->window(), &rect );

        wait_for_vsync();
        wnd_back_buf.blit_to( front_buf, Vector2i(rect.left,rect.top) );
        //ValidateRect( GetDesktopWindow(), NULL );
    }
}


//------------------------------------------------------------------------------

Surface&
VideoCard::front_buffer()
{
    return front_buf;
}


//------------------------------------------------------------------------------

Surface&
VideoCard::back_buffer()
{
    return (fullscreen_mode)  ? back_buf  : wnd_back_buf;
}

} // namespace core
