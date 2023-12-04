//==============================================================================
//
//  Core realization   
//
//==============================================================================
//
//  externals: 

    #include "Core.hpp"

namespace core
{
//------------------------------------------------------------------------------
//
//  publics:

    ProgramCore*    Core;


//------------------------------------------------------------------------------

ProgramCore::ProgramCore( HINSTANCE i, HWND w )
  : _instance(i),
    _window(w),
    ddraw2(NULL),
    dinput3(NULL),
    dragging(false)
{
    LPDIRECTDRAW    ddraw;

    try_ddraw(
               DirectDrawCreate( NULL, &ddraw, NULL )
               __CT("creatin' DDraw")
             );

    try_ddraw(
                ddraw->SetCooperativeLevel( _window, DDSCL_NORMAL )
                __CT("setting coop level")
             );

    try_ddraw(
               ddraw->QueryInterface( IID_IDirectDraw2, (LPVOID *)&ddraw2 )
               __CT("gettin' DDraw2")
             );

    try_dinput(
                DirectInputCreate( _instance, 
                                   DIRECTINPUT_VERSION, 
                                   &dinput3,
                                   NULL
                                 )
                __CT("creatin' DInput")
              );
}


//------------------------------------------------------------------------------

LRESULT         
ProgramCore::wnd_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
    int     x_pos;
    int     y_pos;
    RECT    rect;
    RECT    client_rect;
    
    switch( msg )
    {   
        case WM_PAINT :
            break;

        case WM_CREATE :
        case WM_GETMINMAXINFO :
            return 0;
            break;

        case WM_NCCREATE :
            return TRUE;
            break;

        case WM_DESTROY :
            PostQuitMessage( NULL );
            break;
        /*
        case WM_NCHITTEST :
            std::clog << "hittest\n" << std::flush;
            x_pos = LOWORD(lparam);
            y_pos = HIWORD(lparam);

            GetWindowRect( _window, &rect );
            GetClientRect( _window, &client_rect );

            scr_mouse_pos_x = x_pos;// + client_rect.left - rect.left;
            scr_mouse_pos_y = y_pos;// + client_rect.top - rect.top;

            if( dragging )
            {
                int x_corner = x_pos - drag_point.x();
                int y_corner = y_pos - drag_point.y();

                SetWindowPos( _window, HWND_TOPMOST,
                              x_corner, y_corner,
                              0, 0,
                              SWP_NOSIZE
                            );
                
                
                SHORT   st = GetAsyncKeyState( VK_LBUTTON );

                if( !( (st >> ( 8*sizeof(SHORT) - 1 )) & 0x1) )
                {
                    dragging    = false;

                    //ReleaseCapture();
                    std::clog << "dragging done\n";
                }
                
                return HTCLIENT;
            }
            else
            {
                SHORT   st              = GetAsyncKeyState( VK_LBUTTON );
                bool    dragzone_hit    = false;
                Point2D hit_point( x_pos, y_pos );

                if( (st >> ( 8*sizeof(SHORT) - 1 )) & 0x1 )
                {
                    for( int i=0; i<drag_region.size(); i++ )
                    {
                        if( drag_region[i].includes_point( hit_point ) )
                        {
                            dragzone_hit = true;
                            break; 
                        }
                    }                    
                    
                    if( dragzone_hit )
                    {
                        drag_point  = wnd_coords_of( hit_point );
                        dragging    = true;
                        //SetCapture( _window );

                        std::clog << drag_point.x() << "," << drag_point.y()
                                  << "  dragging...\n" << std::flush;
                    }
                }

                return HTCLIENT;
            }
            break; // case WM_NCHITTEST
        */
        case WM_SETCURSOR :
            return TRUE; 
            break;
        
        case WM_KEYDOWN :
        case WM_KEYUP :
        case WM_CHAR :
        {    
            for( unsigned i=0; i<msg_handler.size(); i++)
            {
                if( msg_handler[i]->wnd_proc( msg, wparam, lparam ) == 0 ) 
                    break;
            }
            
        }
        
    }

    return 0;
}

/*
//------------------------------------------------------------------------------

void            
Program::add_drag_region( const Rect2D& rgn )
{
    drag_region.push_back( rgn );     
}



//------------------------------------------------------------------------------

void            
Program::remove_all_drag_regions()
{
    drag_region.clear(); 
}
*/

} // namespace core

