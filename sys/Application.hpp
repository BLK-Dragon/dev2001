#if !defined _APPLICATION_HPP_
#define _APPLICATION_HPP_
//==============================================================================
//
//  Application decl   
//
//==============================================================================
//
//  externals:

    #include "VideoCard.hpp" 
    #include "Keyboard.hpp"
    #include "Mouse.hpp"
    #include "Window.hpp"


namespace core
{
//==============================================================================
//
//  publics:


class
Application  
{
  public:

    enum
    state
    {
        running,
        paused,
        finished
    };

                    Application(){};
    virtual        ~Application(){};

    virtual void                        init() {};
    virtual state                       update() =0;
    virtual void                        draw() =0;
    virtual ScreenBuffer::FlipMethod    flip_method();

    virtual void                        deactivate(){};
    virtual void                        activate(){};

    Rect2D                              window_location() const;
    
  protected:
    core::Window*       window;

  
  public:
    static VideoCard*   videocard;
    static Keyboard*    keyboard;
    static Mouse*       mouse;
};


//------------------------------------------------------------------------------

inline ScreenBuffer::FlipMethod    
Application::flip_method()
{
    return ScreenBuffer::vsync;
}


//------------------------------------------------------------------------------

inline Rect2D
Application::window_location() const
{
    return window->location(); 
}


}   // namespace core


//==============================================================================

extern core::Application*
CreateApplication();


#endif // _APPLICATION_HPP_