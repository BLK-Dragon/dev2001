//==============================================================================
//
//  Keyboard realization   
//
//==============================================================================
//
//  externals: 

    #include <algorithm>
    
    #include "Keyboard.hpp"


namespace core
{
//==============================================================================
//
//  publics:

    extern ProgramCore* Core;

    const UDword        Keyboard::esc   = (DIK_ESCAPE<<16) | VK_ESCAPE;
    const UDword        Keyboard::left  = (DIK_LEFT<<16) | VK_LEFT;
    const UDword        Keyboard::right = (DIK_RIGHT<<16) | VK_RIGHT;
    const UDword        Keyboard::up    = (DIK_UP<<16) | VK_UP;
    const UDword        Keyboard::down  = (DIK_DOWN<<16) | VK_DOWN;
    const UDword        Keyboard::space = (DIK_SPACE<<16) | VK_SPACE;
    const UDword        Keyboard::lshift= (DIK_LSHIFT<<16) | VK_LSHIFT;
    const UDword        Keyboard::tab   = (DIK_TAB<<16) | VK_TAB;
    const UDword        Keyboard::enter = (DIK_RETURN<<16) | VK_RETURN;


//------------------------------------------------------------------------------

Keyboard::Keyboard( Kind k )
  : kind(k)
{
    if( kind == dinput )
    {    
        try_dinput
        (
            Core->dinput()->CreateDevice( GUID_SysKeyboard,
                                          &kbd_device,
                                          NULL
                                        )
            __CT("creatin' DInput-device")
        );
        
        try_dinput
        (
            kbd_device->SetDataFormat( &c_dfDIKeyboard )
            __CT("settin' keyboard data format")
        );
        
        try_dinput
        (
            kbd_device->SetCooperativeLevel( Core->window(),
                                             DISCL_FOREGROUND |
                                             DISCL_NONEXCLUSIVE
                                           )
            __CT("setting keyboard coop level")
        );
        
        try_dinput
        (
            kbd_device->Acquire()
            __CT("acquirin' keyboard")
        );

    } // if( k == dinput )
    else
    {
        Core->register_msg_handler( this );
    }
}


//------------------------------------------------------------------------------


Keyboard::~Keyboard()
{
    if( kind == dinput )
    {
        try_dinput
        (
            kbd_device->Unacquire()
            __CT("unacquirin' keyboard")
        );
        
        if( kbd_device )
            kbd_device->Release(), kbd_device = NULL;
    }
}       


//------------------------------------------------------------------------------

void
Keyboard::update_state()
{
    if( kind == dinput )
    {
        try_dinput
        (
            kbd_device->GetDeviceState( sizeof(kbd_buffer),
                                        (LPVOID)kbd_buffer
                                      )
            __CT("getting keyboard state")
        );
    }
}


//------------------------------------------------------------------------------

bool 
Keyboard::pressed( UDword key )
{
    if( kind == dinput )
    {
        return (kbd_buffer[ (key>>16)&0xFFFF ] & 0x80)  ? true
                                                        : false;
    }
    else
    {
        SHORT   state = GetAsyncKeyState( key&0xFFFF );

        return ( (state >> ( 8*sizeof(SHORT) - 1 )) & 0x1 )  ? true
                                                             : false;
    }
}


//------------------------------------------------------------------------------

LRESULT                 
Keyboard::wnd_proc( UINT msg, WPARAM wparam, LPARAM lparam )
{   
    UDword  key;

    if( msg == WM_KEYDOWN  || msg == WM_KEYUP )
    {
        switch( wparam )
        {
            case VK_ESCAPE :
                key = esc; 
                break;

            case VK_LEFT :
                key = left; 
                break;
            
            case VK_RIGHT :
                key = right; 
                break;

            case VK_UP :
                key = up; 
                break;

            case VK_DOWN :
                key = down; 
                break;

            case VK_RETURN :
                key = enter; 
                break;

            default :
                key = 0;                           
        } 
    }
    
    
    switch( msg )
    {
        case WM_KEYDOWN :
            EventSource<KbdEvent>::fire( KbdEvent::key_down, key );
            return 0;
            break;
 
        case WM_KEYUP :
            EventSource<KbdEvent>::fire( KbdEvent::key_up, key );
            return 0;
            break;
 
    }
    
    return 1;
}

} // namespace core