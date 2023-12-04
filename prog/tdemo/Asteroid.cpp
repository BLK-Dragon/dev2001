//==============================================================================
//
//  Asteroid realization   
//
//==============================================================================
//
//  externals: 

    #include "Asteroid.hpp"


//==============================================================================
//
//  publics:



//------------------------------------------------------------------------------

void    
Asteroid::respawn( Vector2f pos, Vector2f v, float i )
{
    SpaceObject::respawn( pos, v, i );
    Sprite::invalidate_background();
    
    bound.move_to( Vector2i( pos[0], pos[1] ) );     
}                                 


//------------------------------------------------------------------------------

bool 
Asteroid::collides_with( SpaceObject* obj )
{
    Rect2D  ovr = overlap( bound, obj->cached_bound_rect() );

    return (ovr.is_null())  ? false  : true;
}


//------------------------------------------------------------------------------

void 
Asteroid::process_collision( SpaceObject* obj )
{
    //std::clog<<"asteroid collision\n";
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
    
    //cur_position += cur_velocity;
}


//------------------------------------------------------------------------------

void 
Asteroid::update()
{
    SpaceObject::update();

    bound.move_to( Vector2i( position()[0], position()[1] ) );
}


