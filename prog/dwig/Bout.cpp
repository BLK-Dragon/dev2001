//==============================================================================
//
//  Bout state realization   
//
//==============================================================================
//
//  externals: 

    #include "..\..\sys\Render2D.hpp"

    #include "DwigApp.hpp"


//==============================================================================
//
//  publics:

void                        
DwigApp::BoutHandler::draw()
{

    core::Surface&  back_buffer = app->videocard->back_buffer();

    if( first_draw )
    {
        core::UWord*    memory      = (core::UWord*)back_buffer.memory();
        core::UWord     color       = back_buffer.native_color( 0x00, 0x00, 0x00 );

        for( int i=0; i<back_buffer.pitch()*back_buffer.height()/2; i++ )
            *(memory+i) = color;
        
        app->dirty_rect.clear();
        app->dirty_rect.push_back( Rect2D( 0,0, 
                                           back_buffer.width()-1, back_buffer.height()-1 
                                         )
                                 );
        
        first_draw = false;
    }    
    
    
    /*
    vfx::Render2D       render2d( back_buffer );
    vfx::SolidFill1D    red( vfx::rgb(255,0,0) );
    
    back_buffer.clear();
    render2d.enable_clipping();
    render2d.v_line( 50,  0,   50,  red );
    render2d.v_line( 100, -50, 100, red );
    */

    app->matrix.restore_background( back_buffer );
    app->matrix.redraw( back_buffer );
    
    
    if( locked_in() )
    {
        app->matrix.restore_background( back_buffer );
        app->dirty_rect.clear();
        app->dirty_rect.push_back( Rect2D( 0,0,
                                           back_buffer.width()-1, back_buffer.height()-1
                                         )
                                 );
        unlock();
        first_draw = true;
    }
}


//------------------------------------------------------------------------------

core::Application::state    
DwigApp::BoutHandler::update()
{
    return core::Application::running; 
}


//------------------------------------------------------------------------------
        
void                        
DwigApp::BoutHandler::key_down( core::UDword key )
{
    if( key == core::Keyboard::esc )
        app->change_state( DwigApp::state_MainMenu );
    
//    if( key == core::key_space )
//        app->change_state( DwigApp::state_MainMenu );
}


//------------------------------------------------------------------------------

void                        
DwigApp::BoutHandler::enter_state()
{
    app->matrix.re_create( 0, 0 );     
}


//------------------------------------------------------------------------------

void                        
DwigApp::BoutHandler::leave_state()
{
    lock();
}