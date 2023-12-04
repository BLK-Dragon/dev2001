//==============================================================================
//
//  Main Menu realization
//
//==============================================================================
//
//  externals: 

    #include "DwigApp.hpp"


//==============================================================================
//
//  

DwigApp::MainMenuHandler::~MainMenuHandler()
{
    unsigned i;
    
    for( i=0; i<menu_item.size(); i++ )
        delete menu_item[i];
}


//------------------------------------------------------------------------------

void
DwigApp::MainMenuHandler::enter_state()
{
    if( first_entrance )
    {
        menu_item.push_back( new core::Sprite( app->resource_pack->image(".\\image\\mainmenu_options.tga") ) );
        menu_item.push_back( new core::Sprite( app->resource_pack->image(".\\image\\mainmenu_playbout.tga") ) );
        menu_item.push_back( new core::Sprite( app->resource_pack->image(".\\image\\mainmenu_quit.tga") ) );
        
        first_entrance      = true;
        menu_item_changed   = true;
    }
    
    
    menu_item[options]->place( Vector2i( 150, 50 ) );     
    menu_item[play_bout]->place( Vector2i( 150, 150 ) );     
    menu_item[quit]->place( Vector2i( 150, 250 ) );

    first_draw = true;
}


//------------------------------------------------------------------------------

void
DwigApp::MainMenuHandler::leave_state()
{
     lock();
}


//------------------------------------------------------------------------------

void                        
DwigApp::MainMenuHandler::draw()
{
    core::Surface&  back_buf = videocard->back_buffer();

    if( first_draw )
    {
        menu_item[options]->save_background( back_buf );
        menu_item[options]->redraw( back_buf );
        app->dirty_rect.push_back( menu_item[options]->bound_rect() );
        
        menu_item[play_bout]->save_background( back_buf );
        menu_item[play_bout]->redraw( back_buf );
        app->dirty_rect.push_back( menu_item[play_bout]->bound_rect() );
        
        menu_item[quit]->save_background( back_buf );
        menu_item[quit]->redraw( back_buf );
        app->dirty_rect.push_back( menu_item[quit]->bound_rect() );
        
        first_draw = false;
        return;
    }
    
    
    if( locked_in() )
    {
        menu_item[options]->restore_background( back_buf );
        menu_item[play_bout]->restore_background( back_buf );
        menu_item[quit]->restore_background( back_buf );
        unlock();
    }
    else
    {
        if( menu_item_changed )
        {
            menu_item[cur_item]->restore_background( back_buf );
            menu_item[cur_item]->set_phase( item_selected );
            menu_item[cur_item]->save_background( back_buf );
            menu_item[cur_item]->redraw( back_buf );
            app->dirty_rect.push_back( menu_item[cur_item]->bound_rect() );
            
            
            if( prev_item != -1 )
            {
                menu_item[prev_item]->restore_background( back_buf );
                menu_item[prev_item]->set_phase( item_normal );
                menu_item[prev_item]->save_background( back_buf );
                menu_item[prev_item]->redraw( back_buf );
                app->dirty_rect.push_back( menu_item[prev_item]->bound_rect() );
            }
        } // if( menu_item_changed )
    }

}


//------------------------------------------------------------------------------

core::Application::state    
DwigApp::MainMenuHandler::update()
{
    
    if( /*keyboard->pressed( core::Keyboard::esc )  ||*/  exiting )
        return core::Application::finished;

    return core::Application::running;
    
}


//------------------------------------------------------------------------------

void
DwigApp::MainMenuHandler::key_down( core::UDword key )
{
    int item = cur_item;

    if( key == core::Keyboard::down ) ++item;
    if( key == core::Keyboard::up )   --item;

    item = (item<0)  ? 0  : (item>max_item)  ? max_item  : item;

    if( item != cur_item )
    {
        prev_item           = cur_item;
        cur_item            = item;
        menu_item_changed   = true;
    }
    else
    {
        prev_item           = -1; 
        menu_item_changed   = false;
    }
    
    if( key == core::Keyboard::enter  &&  cur_item == quit )
        exiting = true;

    if( key == core::Keyboard::enter  &&  cur_item == play_bout )
        app->change_state( DwigApp::state_Bout );

}