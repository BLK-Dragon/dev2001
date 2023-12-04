//==============================================================================
//
//
//
//==============================================================================
//
//  externals:

    #include "DwigApp.hpp"


//==============================================================================
//
//  publics:

core::Application*
CreateApplication()
{
    return new DwigApp; 
}


//------------------------------------------------------------------------------

DwigApp::DwigApp()
  : cur_state_handler(0)
{
    state_handler.reserve(20);
    state_handler[ state_MainMenu ]     = new MainMenuHandler(this);
    state_handler[ state_InitialState ] = new InitialStateHandler(this);
    state_handler[ state_Bout ]         = new BoutHandler(this);
}


//------------------------------------------------------------------------------

DwigApp::~DwigApp()
{
    unsigned i;
    
    for( i=0; i<state_handler.size(); i++ )
        delete state_handler[i];

    delete resource_pack;
}


//------------------------------------------------------------------------------

void    
DwigApp::init()
{
    videocard->set_mode( 800, 600, 16, 100 );
    
    resource_pack   = new core::ResourcePack( &file_system );
    window          = new core::Window( 0, 0, 640, 480 );
    
    
    
    
    change_state( state_InitialState );
}


//------------------------------------------------------------------------------
    
void                
DwigApp::change_state( int s )
{
    if( cur_state_handler )
    {
        new_state_handler = state_handler[s];
        cur_state_handler->leave_state();
    }
    else
    {
        new_state_handler = 0;
        cur_state_handler = state_handler[s];
        cur_state_handler->enter_state();
        keyboard->subscribe( cur_state_handler );
    }
}


//------------------------------------------------------------------------------

core::Application::state   
DwigApp::update()
{
    if( new_state_handler  &&  !cur_state_handler->locked_in() )
    {
        keyboard->unsubscribe( cur_state_handler );

        cur_state_handler = new_state_handler;
        new_state_handler = 0;

        cur_state_handler->enter_state();
        keyboard->subscribe( cur_state_handler );
    }
    
    return cur_state_handler->update();
}


//------------------------------------------------------------------------------

void    
DwigApp::draw()
{
    unsigned i;

    core::Surface&  front_buf   = videocard->front_buffer();
    core::Surface&  back_buf    = videocard->back_buffer();
    
    for( i=0; i<dirty_rect.size(); i++ )
         front_buf.blit_to( back_buf, dirty_rect[i], dirty_rect[i] );
    dirty_rect.clear();
    
    cur_state_handler->draw();
}


       