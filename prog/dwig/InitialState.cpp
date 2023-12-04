//==============================================================================
//
//  Initial State handler   
//
//==============================================================================
//
//  externals: 

    #include "DwigApp.hpp"


//==============================================================================
//
//  publics:

void                        
DwigApp::InitialStateHandler::enter_state()
{
    app->matrix.bind_to_resourcepack( app->resource_pack );
}


//------------------------------------------------------------------------------

void                        
DwigApp::InitialStateHandler::draw()
{
    core::Surface&  back_buffer = app->videocard->back_buffer();
    core::UWord*    memory      = (core::UWord*)back_buffer.memory();
    core::UWord     color       = back_buffer.native_color( 0x10, 0x10, 0x20 );

    for( int i=0; i<back_buffer.pitch()*back_buffer.height()/2; i++ )
        *(memory+i) = color;

    app->dirty_rect.clear();
    app->dirty_rect.push_back( Rect2D( 0,0, 
                                       back_buffer.width()-1, back_buffer.height()-1 
                                     )
                             );
    
    app->change_state( DwigApp::state_MainMenu );
}


//------------------------------------------------------------------------------

core::Application::state    
DwigApp::InitialStateHandler::update()
{
    return core::Application::running; 
}

        
//------------------------------------------------------------------------------

void                        
DwigApp::InitialStateHandler::key_down( core::UDword key )
{
//    if( key == core::key_space )
//        app->change_state( DwigApp::state_MainMenu );
}


