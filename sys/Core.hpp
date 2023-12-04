#if !defined _CORE_HPP_
#define _CORE_HPP_
//==============================================================================
//
//  DDraw object   
//
//==============================================================================
//
//  externals: 

    #include <iostream>
    #include <vector>
    
    #include "DDrawError.hpp"
    #include "DInputError.hpp"
    #include "Type.hpp"

    #include "Vector.hpp"
    #include "Rect.hpp"

namespace core
{
//------------------------------------------------------------------------------
//
//  publics:


// pre-declarations

class VideoCard;
class Surface;
class ScreenBuffer;


//------------------------------------------------------------------------------

using std::vector;

class ProgramCore;



class
Win32MessageHandler  
{
    friend class ProgramCore;

    virtual LRESULT     wnd_proc( UINT msg, WPARAM wparam, LPARAM lparam ) =0;
};


//------------------------------------------------------------------------------




class
ProgramCore  
{

  public:
                    
                    ProgramCore( HINSTANCE i, HWND w );
                   ~ProgramCore();

    HINSTANCE       instance();
    HWND            window();
    LPDIRECTDRAW2   ddraw();
    LPDIRECTINPUT   dinput();

    LRESULT         wnd_proc( HWND wnd, UINT msg, 
                              WPARAM wparam, LPARAM lparam 
                            );

    
    void            register_msg_handler( Win32MessageHandler* handler );
    
    /*
    int             window_mouse_position_x() const;
    int             window_mouse_position_y() const;
    int             screen_mouse_position_x() const;
    int             screen_mouse_position_y() const;

    void            add_drag_region( const Rect2D& rgn );
    void            remove_all_drag_regions();
    */
  private:

    HINSTANCE       _instance;
    HWND            _window;
    LPDIRECTDRAW2   ddraw2;
    LPDIRECTINPUT   dinput3;

    Vector2i        wnd_coords_of( const Vector2i& p );
    Vector2i        scr_coords_of( const Vector2i& p );
    
    
    int             wnd_mouse_pos_x;
    int             wnd_mouse_pos_y;
    int             scr_mouse_pos_x;
    int             scr_mouse_pos_y;
    
    bool            dragging;
    vector<Rect2D>  drag_region;
    Vector2i        drag_point;     // wnd coords

    vector<Win32MessageHandler*>    msg_handler;
};


//------------------------------------------------------------------------------

inline
ProgramCore::~ProgramCore()
{
    if( ddraw2 != NULL )
        ddraw2->Release(), ddraw2=NULL;
    
    if( dinput3 != NULL )
        dinput3->Release(), dinput3=NULL;
}


//------------------------------------------------------------------------------

inline void            
ProgramCore::register_msg_handler( Win32MessageHandler* handler )
{
     msg_handler.push_back( handler );
}


//------------------------------------------------------------------------------

inline HINSTANCE
ProgramCore::instance()
{
    return _instance; 
}


//------------------------------------------------------------------------------

inline HWND
ProgramCore::window()
{
    return _window; 
}


//------------------------------------------------------------------------------

inline LPDIRECTDRAW2 
ProgramCore::ddraw()
{
    return ddraw2;
}


//------------------------------------------------------------------------------

inline LPDIRECTINPUT
ProgramCore::dinput()
{
    return dinput3;
}

/*
//------------------------------------------------------------------------------

inline int
ProgramCore::window_mouse_position_x() const
{
    return wnd_mouse_pos_x; 
}


//------------------------------------------------------------------------------

inline int
ProgramCore::window_mouse_position_y() const
{
    return wnd_mouse_pos_y; 
}


//------------------------------------------------------------------------------

inline int
ProgramCore::screen_mouse_position_x() const
{
    return scr_mouse_pos_x; 
}


//------------------------------------------------------------------------------

inline int
ProgramCore::screen_mouse_position_y() const
{
    return scr_mouse_pos_y; 
}


//------------------------------------------------------------------------------

inline Vector2i             
ProgramCore::scr_coords_of( const Vector2i& p )
{
    RECT    rect;

    GetWindowRect( _window, &rect ); 

    return Vector2i( rect.left + p.x(), rect.top + p.y() );
}


//------------------------------------------------------------------------------

inline Vector2i             
ProgramCore::wnd_coords_of( const Vector2i& p )
{
    RECT    rect;

    GetWindowRect( _window, &rect );

    return Vector2i( p.x() - rect.left, p.y() - rect.top );
}
*/

extern ProgramCore* Core;

} // namespace core
#endif // _CORE_HPP_