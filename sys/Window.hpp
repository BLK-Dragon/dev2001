#if !defined _WINDOW_HPP_
#define _WINDOW_HPP_
//==============================================================================
//
//  Window decl   
//
//==============================================================================
//
//  externals: 

    #include "Rect.hpp"


namespace core
{
//==============================================================================
//
//  publics:




class
Window  
{
  public:

                    Window( const Rect2D& rect );
                    Window( int x, int y, int w, int h );

    Rect2D          location() const;

  private:

    int             wnd_x;
    int             wnd_y;
    int             wnd_width;
    int             wnd_height;
};


//------------------------------------------------------------------------------

inline
Window::Window( const Rect2D& rect )
  : wnd_x( rect.left() ),
    wnd_y( rect.top() ),
    wnd_width( rect.right() - rect.left() ),
    wnd_height( rect.bottom() - rect.top() )
{
}


//------------------------------------------------------------------------------

inline
Window::Window( int x, int y, int w, int h )
  : wnd_x(x),
    wnd_y(y),
    wnd_width(w),
    wnd_height(h)
{
}


//------------------------------------------------------------------------------

inline Rect2D 
Window::location() const
{
    return Rect2D( wnd_x, wnd_y, wnd_x+wnd_width, wnd_y+wnd_height );
}


}   // namespace core


#endif // _WINDOW_HPP_