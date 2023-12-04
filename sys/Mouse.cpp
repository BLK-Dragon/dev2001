//==============================================================================
//
//  Mouse realization
//
//==============================================================================
//
//  externals: 

    #include "Mouse.hpp"

namespace core
{
//==============================================================================
//
//  publics:

extern ProgramCore* Core;


//------------------------------------------------------------------------------

Mouse::Mouse()
  : cur_pos( Vector2i(0,0) )
{   
    try_dinput(
                Core->dinput()->CreateDevice( GUID_SysMouse,
                                              &mouse_device,
                                              NULL
                                            )
                __CT("creating DInput-device")
              );
    
    try_dinput(
                mouse_device->SetDataFormat( &c_dfDIMouse )
                __CT("setting mouse data format")
              );
    
    try_dinput(
                mouse_device->SetCooperativeLevel( Core->window(),
                                                   DISCL_FOREGROUND |
                                                   DISCL_NONEXCLUSIVE
                                                 )
                __CT("setting mouse coop level")
              );
    
    try_dinput(
                mouse_device->Acquire()
                __CT("acquiring mouse")
              );
    
    update_state();
}


//------------------------------------------------------------------------------

Mouse::~Mouse()
{   
    try_dinput(
                mouse_device->Unacquire()
                __CT("unacquiring mouse")
              );
    
    if( mouse_device )
        mouse_device->Release(), mouse_device = NULL;
    
}


//------------------------------------------------------------------------------

void 
Mouse::update_state()
{   /*
    cur_pos = Vector2i( Core->screen_mouse_position_x(),
                       Core->screen_mouse_position_y()
                     );
    */
    
    try_dinput
    (
        mouse_device->GetDeviceState( sizeof(mouse_state),
                                      (LPVOID)&mouse_state
                                    )
        __CT("getting mouse state")
    );
    
}


//------------------------------------------------------------------------------

bool 
Mouse::is_inside_of_app_window() const
{
    return true;
}


//------------------------------------------------------------------------------

Vector2i 
Mouse::screen_position() const
{   /*
    if( cur_pos.x() < GetSystemMetrics( SM_CXFULLSCREEN ) &&
        cur_pos.y() < GetSystemMetrics( SM_CYFULLSCREEN ) &&
      )
    */
    return cur_pos;
}


//------------------------------------------------------------------------------

Vector2i 
Mouse::window_position() const
{
    
    RECT    rect;

    GetWindowRect( Core->window(), &rect );
    
    return Vector2i( cur_pos[0] - rect.left, cur_pos[1] - rect.top );
    
}


} // namespace core
