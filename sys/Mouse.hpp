#if !defined _MOUSE_HPP_
#define _MOUSE_HPP_
//==============================================================================
//
//  Mouse decl   
//
//==============================================================================
//
//  externals:

    #include "Core.hpp"
    #include "Vector.hpp"

namespace core
{
//==============================================================================
//
//  publics:




class
Mouse
{
  public:

                            Mouse();
                           ~Mouse();

    void                    update_state();
    
    bool                    is_inside_of_app_window() const;
    Vector2l                relative_position() const;
    Vector2i                screen_position() const;
    Vector2i                window_position() const;
    
    bool                    pressed_button( int n ) const;

  private:

    Vector2i                cur_pos;
    Vector2i                rel_pos;
    LPDIRECTINPUTDEVICE     mouse_device;
    DIMOUSESTATE            mouse_state;
};


//------------------------------------------------------------------------------

inline Vector2l            
Mouse::relative_position() const
{
    return Vector2l( mouse_state.lX, mouse_state.lY ); 
}


//------------------------------------------------------------------------------

inline bool                    
Mouse::pressed_button( int n ) const
{
    BYTE    b =  mouse_state.rgbButtons[n];

    return  b & (0x01<<(sizeof(BYTE)*8-1));
}


}   // namespace core

#endif // _MOUSE_HPP_