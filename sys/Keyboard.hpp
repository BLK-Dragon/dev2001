#if !defined _KEYBOARD_HPP_
#define _KEYBOARD_HPP_
//==============================================================================
//
//  Keyboard class decl   
//
//==============================================================================
//
//  externals:

    #include "Core.hpp"
    #include "Event.hpp"


namespace core
{
//==============================================================================
//
//  publics:

class KbdEvent;



class
Keyboard  
  : public Win32MessageHandler,
    public EventSource<KbdEvent>
{
  public:

    enum            Kind;

                    Keyboard( Kind k=win32 );
                   ~Keyboard();

    void            update_state();

    bool            pressed( UDword key );

  public:

    enum
    Kind
    {
        win32,
        dinput
    };
    

  private:
    
    LRESULT                 wnd_proc( UINT msg, WPARAM wparam, LPARAM lparam );
    
    Kind                    kind;
    char                    kbd_buffer[256];
    LPDIRECTINPUTDEVICE     kbd_device;

  
  public:

    static const UDword     esc;
    static const UDword     left;
    static const UDword     right;
    static const UDword     up;
    static const UDword     down;
    static const UDword     space;
    static const UDword     lshift;
    static const UDword     tab;
    static const UDword     enter;

};


//------------------------------------------------------------------------------




class
KbdEvent
{
  public:

    virtual void    key_down( UDword key ) {};
    virtual void    key_up( UDword key ) {};
    virtual void    character( char ch ) {};
};


//------------------------------------------------------------------------------

enum
VirtualKeyCode
{
    key_esc     = VK_ESCAPE,
    
    key_left    = VK_LEFT,
    key_right   = VK_RIGHT,
    key_up      = VK_UP,
    key_down    = VK_DOWN,

    key_enter   = VK_RETURN,
    key_space   = VK_SPACE
};


} // namespace core
#endif // _KEYBOARD_HPP_
