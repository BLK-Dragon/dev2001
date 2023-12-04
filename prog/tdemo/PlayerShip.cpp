//==============================================================================
//
//  PlayerShip realization
//
//==============================================================================
//
//  externals:

    #include "PlayerShip.hpp"

core::ParamSetManager*   PlayerShip::paramset_manager = 0;


//------------------------------------------------------------------------------

PlayerShip::PlayerShip( core::Image& img, 
                        Vector2f pos, Vector2f v, float i 
                      )
  : SpaceObject( img, pos, v, i ),
    bound( Vector2i(pos[0],pos[1]), img.width(), img.height() )
{
    core::ParamSet  paramset = paramset_manager->paramset( "ship", 0 );

    set_max_speed( Vector2f( (double)paramset["max_speed_x"],
                             (double)paramset["max_speed_y"]
                           )
                 );
    set_rest_speed( Vector2f( 0, -1 ) );
    
    acc_speed = Vector2f( (double)paramset["acc_speed_x"], 
                          (double)paramset["acc_speed_y"] 
                        );
    dec_speed = Vector2f( (double)paramset["dec_speed_x"], 
                          (double)paramset["dec_speed_y"] 
                        );

    constrain_rect = Rect2D( -4000,-4000, 4000,4000 );
}


//------------------------------------------------------------------------------

void        
PlayerShip::update()
{
    SpaceObject::update();

    Vector2i    scr_pos = Vector2i( cur_position[0] - origin[0],
                                    cur_position[1] - origin[1]
                                  );
    
    if( scr_pos[0] < constrain_rect.left() )
        cur_position[0] = origin[0] + constrain_rect.left();

    if( scr_pos[0] + bound.width() > constrain_rect.right() )
        cur_position[0] = origin[0] + constrain_rect.right() - bound.width();

    if( scr_pos[1] < constrain_rect.top() )
        cur_position[1] = origin[1] + constrain_rect.top();

    if( scr_pos[1] + bound.height() > constrain_rect.bottom() )
        cur_position[1] = origin[1] + constrain_rect.bottom() - bound.height();
    
    bound.move_to( Vector2i( position()[0], position()[1] ) );
}


//------------------------------------------------------------------------------

void    
PlayerShip::process_collision( SpaceObject* obj )
{
    //std::clog<<"ship collision\n";
    Rect2D  obj_bound   = obj->cached_bound_rect();
    Rect2D  ovr         = overlap( this->cached_bound_rect(), obj_bound );

    float   vx      = velocity()[0];
    float   vy      = velocity()[1];
    int     hsign   = (position()[0] - obj_bound.left() > 0)  ? 1  : -1;
    int     vsign   = (position()[1] - obj_bound.left() > 0)  ? 1  : -1;

    if( fabs( obj->velocity()[0] > vx ) )
        vx = fabs( obj->velocity()[0] );
    
    if( fabs( obj->velocity()[1] > vy ) )
        vy = fabs( obj->velocity()[1] );
    
    this->set_velocity( Vector2f( hsign*vx, vsign*vy ) );

    //add_velocity( obj->velocity() );  
}


//------------------------------------------------------------------------------

bool    
PlayerShip::collides_with( SpaceObject* obj )
{ 
    Rect2D  ovr = overlap( bound, obj->cached_bound_rect() );

    return (ovr.is_null())  ? false  : true;
}


//------------------------------------------------------------------------------

void            
PlayerShip::decelerate( PlayerShip::Direction d )
{
    switch( d )
    {
        case left :
            SpaceObject::decelerate( Vector2f(dec_speed[0],0) );
            break;

        case right :
            SpaceObject::decelerate( Vector2f(-dec_speed[0],0) );
            break;

        case up :
            SpaceObject::decelerate( Vector2f(0,dec_speed[1]) );
            break;

        case down :
            SpaceObject::decelerate( Vector2f(0,-dec_speed[1]) );
            break;
    }
}


//------------------------------------------------------------------------------

void            
PlayerShip::accelerate( PlayerShip::Direction d )
{
    switch( d )
    {
        case left :
            SpaceObject::accelerate( Vector2f(-acc_speed[0],0) );
            break;

        case right :
            SpaceObject::accelerate( Vector2f(acc_speed[0],0) );
            break;

        case up :
            SpaceObject::accelerate( Vector2f(0,-acc_speed[1]) );
            break;

        case down :
            SpaceObject::accelerate( Vector2f(0,acc_speed[1]) );
            break;
    }
}


//------------------------------------------------------------------------------

void         
PlayerShip::register_paramset( core::ParamSetManager* manager )
{
    paramset_manager = manager;

    manager->register_paramset( "ship", PlayerShip::bind_members ); 
}


//------------------------------------------------------------------------------

void         
PlayerShip::bind_members( core::ParamSetPool* pool )
{
    pool->start_member_binding();

        pool->bind_double_member("max_speed_x");
        pool->bind_double_member("max_speed_y");

        pool->bind_double_member("acc_speed_x");
        pool->bind_double_member("acc_speed_y");
        
        pool->bind_double_member("dec_speed_x");
        pool->bind_double_member("dec_speed_y");

    pool->finish_member_binding(); 
}
