//==============================================================================
//
//  Surface realization   
//
//==============================================================================
//
//  externals: 

    #include "Surface.hpp"

namespace core
{
    extern ProgramCore* Core;
    
    DDPIXELFORMAT       Surface::prim_surf_pixelformat;
    
//------------------------------------------------------------------------------

Surface::Surface()
  : surf2(NULL),
    locked(false),
    transfer(DDBLTFAST_WAIT),
    blt_flags(DDBLT_WAIT)
{
}


//------------------------------------------------------------------------------

Surface::Surface( int width, int height, kind k )
  : surf2(NULL),
    locked(false),
    transfer(DDBLTFAST_WAIT),
    blt_flags(DDBLT_WAIT)
{
    LPDIRECTDRAWSURFACE     surf;
    DDSURFACEDESC           desc;

    ZeroMemory( (void*)&desc, sizeof(desc) );

    desc.dwSize             = sizeof(desc);
    desc.dwFlags            = DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT;//|DDSD_PIXELFORMAT;
    desc.dwWidth            = width;
    desc.dwHeight           = height;
    //desc.ddpfPixelFormat    = Surface::prim_surf_pixelformat;

    switch( k )
    {
        case SysMem :
            desc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | 
                                  DDSCAPS_SYSTEMMEMORY;
            break;

        case VidMem :
            desc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
            break;
    }


    try_ddraw(
               Core->ddraw()->CreateSurface( &desc, &surf, NULL )
               __CT("creating surface" )
             );

    try_ddraw(
               surf->QueryInterface( IID_IDirectDrawSurface2, 
                                     (LPVOID *)&surf2
                                   )
               __CT("creating surface: getting DDsurface2" )
             );
    surf->Release(), surf = NULL;

    update_info();
}


//------------------------------------------------------------------------------

Surface::Surface( int width, int height, const UByte* mem, kind k )
  : surf2(NULL),
    locked(false),
    transfer(DDBLTFAST_WAIT),
    blt_flags(DDBLT_WAIT)
{
    LPDIRECTDRAWSURFACE     surf;
    DDSURFACEDESC           desc;

    ZeroMemory( (void*)&desc, sizeof(desc) );

    desc.dwSize             = sizeof(desc);
    desc.dwFlags            = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;// |
                              //DDSD_PIXELFORMAT;
    desc.dwWidth            = width;
    desc.dwHeight           = height;
    //desc.ddpfPixelFormat    = Surface::prim_surf_pixelformat;

    switch( k )
    {
        case SysMem :
            desc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN |
                                  DDSCAPS_SYSTEMMEMORY;
            break;

        case VidMem :
            desc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
            break;

    }


    try_ddraw(
               Core->ddraw()->CreateSurface( &desc, &surf, NULL )
               __CT("creating surface" )
             );

    try_ddraw(
               surf->QueryInterface( IID_IDirectDrawSurface2,
                                     (LPVOID *)&surf2
                                   )
               __CT("creating surface: getting DDsurface2" )
             );
    surf->Release(), surf = NULL;

    update_info();
    
    
    // copy 32bit image into surface memory
    
    HDC         img_dc  = CreateCompatibleDC( NULL );
    HDC         surf_dc;
    HBITMAP     bitmap;
    BITMAPINFO  info;
    VOID*       ptr;
    
    if( img_dc == NULL )
        throw Error("can't create comp.DC (surface::surface)");
    
    
    ZeroMemory( &info.bmiHeader, sizeof(BITMAPINFOHEADER) );

    info.bmiHeader.biSize           = sizeof( BITMAPINFOHEADER );
    info.bmiHeader.biWidth          = width;
    info.bmiHeader.biHeight         = -height;
    info.bmiHeader.biPlanes         = 1;
    info.bmiHeader.biBitCount       = 32;
    info.bmiHeader.biCompression    = BI_RGB;
    info.bmiHeader.biSizeImage      = 0;

    bitmap = CreateDIBSection( img_dc,
                               &info,
                               DIB_RGB_COLORS,
                               &ptr,
                               NULL,
                               0
                             );
    if( bitmap == NULL )
        throw Error("can't create DIB section (surface::surface)");
    

    if( (DWORD)SelectObject( img_dc, bitmap ) == GDI_ERROR )
        throw Error("GDI error (surface::surface)");

    CopyMemory( ptr, mem, width*height*4 );
    
    try_ddraw(
                surf2->GetDC( &surf_dc )
                __CT("getting image DC (image::image)")
             );

    try_win32
    (
        BitBlt( surf_dc, 0,0, width, height,
                img_dc, 0,0, SRCCOPY
              )
        //__CT("blitting loaded image into surface (image::image)")
    );

    DeleteObject( bitmap );
    DeleteObject( img_dc );
    try_ddraw(
                surf2->ReleaseDC( surf_dc )
                __CT("releasing image DC (image::image)")
             );
}


//------------------------------------------------------------------------------

Surface::~Surface()
{
    if( surf2 != NULL )
        surf2->Release(), surf2 = NULL;
}


//------------------------------------------------------------------------------

int 
Surface::width() const
{
    return info.dwWidth;
}


//------------------------------------------------------------------------------

long 
Surface::pitch() const
{
    return info.lPitch;
}


//------------------------------------------------------------------------------

int 
Surface::height() const
{
    return info.dwHeight;
}


//------------------------------------------------------------------------------

int 
Surface::bpp() const
{
    return format.dwRGBBitCount;
}


//------------------------------------------------------------------------------

UByte* 
Surface::memory()
{
    lock();
        
    return (UByte*)info.lpSurface;
}


//------------------------------------------------------------------------------

void
Surface::lock()
{
    if( locked )
        return;
    
    try_ddraw(
               surf2->Lock( NULL, &info, DDLOCK_WAIT, NULL )
               __CT("locking surface")
             );

    locked = true;
}


//------------------------------------------------------------------------------

void
Surface::unlock()
{
    if( !locked )
        return;
    
    try_ddraw(
               surf2->Unlock( NULL )
               __CT("unlocking surface")
             );
    
    locked = false;
}


//------------------------------------------------------------------------------

void                    
Surface::blit_to( Surface& dst, const Vector2i& pos )
{
    RECT    src_rect;

    src_rect.left   = 0;
    src_rect.top    = 0;
    src_rect.right  = this->width();// - 1;
    src_rect.bottom = this->height();// - 1;
    
    dst.unlock();
    this->unlock();
    
    try_ddraw(
               (dst.surf2)->BltFast( pos[0], pos[1],
                                     this->surf2,
                                     &src_rect,
                                     this->transfer
                                   )
               __CT("blitting surface without clipping")
             );
}


//------------------------------------------------------------------------------

void                    
Surface::blit_to( Surface& dst, const Vector2i& dst_pos, const Rect2D& src_rect )
{
    RECT    rect;

    rect.left   = src_rect.left();
    rect.top    = src_rect.top();
    rect.right  = src_rect.right() + 1;
    rect.bottom = src_rect.bottom() + 1;
    
    dst.unlock();
    this->unlock();
    
    try_ddraw(
               (dst.surf2)->BltFast( dst_pos[0], dst_pos[1],
                                     this->surf2,
                                     &rect,
                                     this->transfer
                                   )
               __CT("blitting surface")
             );
}


//------------------------------------------------------------------------------

void                    
Surface::blit_to( Surface& dst, const Rect2D& dst_rect, const Rect2D& src_rect )
{
    RECT    src_rectangle;
    RECT    dst_rectangle;

    src_rectangle.left   = src_rect.left();
    src_rectangle.top    = src_rect.top();
    src_rectangle.right  = src_rect.right() + 1;
    src_rectangle.bottom = src_rect.bottom() + 1;
    
    dst_rectangle.left   = dst_rect.left();
    dst_rectangle.top    = dst_rect.top();
    dst_rectangle.right  = dst_rect.right() + 1;
    dst_rectangle.bottom = dst_rect.bottom() + 1;
    
    dst.unlock();
    this->unlock();
    
    try_ddraw(
               (dst.surf2)->Blt( &dst_rectangle,
                                 this->surf2,
                                 &src_rectangle,
                                 blt_flags,
                                 NULL
                               )
               __CT("blitting surface rect-to-rect")
             );
}


//------------------------------------------------------------------------------

void                    
Surface::set_pixel( int x, int y, UDword color )
{
    if( this->bpp() == 32 )
        *( (UDword*)(memory()) + y*pitch()/4 + x  ) = color;
    else if( this->bpp() == 16 )
        *( (UWord*)(memory()) + y*pitch()/2 + x  ) = (UWord)color;
    
    //this->unlock();
}


//------------------------------------------------------------------------------

void                    
Surface::set_pixel( int x, int y, UByte r, UByte g, UByte b )
{
    // r,g,b are 8bit values
    
    if( this->bpp() == 32 )
    {
        *( (UDword*)(memory()) + y*pitch()/4 + x ) = native_color(r,g,b);;
    }
    else if( this->bpp() == 16 )
    {
        *( (UWord*)(memory()) + y*pitch()/2 + x ) = native_color(r,g,b);
    }
}


//------------------------------------------------------------------------------

void                    
Surface::draw_text( int xpos, int ypos, UDword color, int height,
                    const Msg& msg, bool t 
                  )
{
    HDC             dc;

    unlock();
    try_ddraw(
                surf2->GetDC( &dc )
                __CT("getting DC (draw_text)")
             ); 
    
    SetTextColor( dc, color );
    if( t )
        SetBkMode( dc, TRANSPARENT );
    
    HFONT   font = CreateFont( height, 0, 0,0, 
                               FW_BOLD, FALSE, FALSE, FALSE, 
                               ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                               CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                               DEFAULT_PITCH,
                               "Courier New"
                             );
    SelectObject( dc, font );
    TextOut( dc, xpos, ypos, msg.text().c_str(), strlen(msg.text().c_str()) );
    DeleteObject( font );

    try_ddraw(
                surf2->ReleaseDC( dc )
                __CT("releasing DC (draw_text)")
             ); 

}


//------------------------------------------------------------------------------

void                    
Surface::set_src_colorkey( UByte r, UByte g, UByte b )
{
    DWORD       color = native_color(r,g,b);
    DDCOLORKEY  key;

    key.dwColorSpaceLowValue    = color; 
    key.dwColorSpaceHighValue   = color;  
    transfer                   |= DDBLTFAST_SRCCOLORKEY;
    blt_flags                  |= DDBLT_KEYSRC;
    
    try_ddraw(
               surf2->SetColorKey( DDCKEY_SRCBLT, &key )
               __CT("setting src color key for surface")
             );
}


//------------------------------------------------------------------------------

UDword                  
Surface::native_color( UDword rgb )
{
    return native_color( core::UByte(rgb&0x000FF),
                         core::UByte((rgb>>8)&0x000FF),
                         core::UByte((rgb>>16)&0x000FF)
                       ); 
}


//------------------------------------------------------------------------------

void
Surface::update_info()
{
    ZeroMemory( (void*)&info, sizeof(info) );
    info.dwSize = sizeof(info);
    try_ddraw(
               surf2->GetSurfaceDesc( &info )
               __CT("getting desc of surface")
             );

    ZeroMemory( (void*)&format, sizeof(format) );
    format.dwSize = sizeof(format);
    try_ddraw(
               surf2->GetPixelFormat( &format )
               __CT("getting pixel format of surface")
             );

    if( bpp() == 16 )
    {
     
        UDword  mask;        
        UDword  bit_offset;
            
        mask        = red_mask();
        bit_offset  = 0;

        while( !(mask&0x8000) )
        {
            mask <<= 1 ;
            ++bit_offset;
        }
        red_offset  = bit_offset;

        
        mask        = green_mask();
        bit_offset  = 0;

        while( !(mask&0x8000) )
        {
            mask <<= 1 ;
            ++bit_offset;
        }
        green_offset= bit_offset;

       
        mask        = blue_mask();
        bit_offset  = 0;

        while( !(mask&0x8000) )
        {
            mask <<= 1 ;
            ++bit_offset;
        }
        blue_offset = bit_offset;
    }    

}


//==============================================================================
//
//  Screen buffer
//

ScreenBuffer::ScreenBuffer()
{
}


//------------------------------------------------------------------------------

void 
ScreenBuffer::flip( ScreenBuffer::FlipMethod method )
{
    unlock();
    
    HRESULT     result;
    DWORD       flag;

    if( method == no_vsync )    flag = DDFLIP_NOVSYNC;
    else if( method == vsync )  flag = DDFLIP_WAIT;
    
    while( true )
    {
        switch( surf2->Flip( NULL, flag ) )
        {
            case DD_OK :
                return;
                break;
            
            case DDERR_SURFACELOST :
                if( surf2->Restore() != DD_OK )
                    return;
                break;
            
            case DDERR_WASSTILLDRAWING :
                break;
        }
    }
}


//------------------------------------------------------------------------------

void
ScreenBuffer::assign( LPDIRECTDRAWSURFACE2 s )
{
    surf2 = s;

    update_info(); 
}


} // namespace core