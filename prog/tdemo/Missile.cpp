//==============================================================================
//
//  Missile realisations   
//
//==============================================================================
//
//  externals: 

    #include "Missile.hpp"


//==============================================================================
//
//  publics:



//------------------------------------------------------------------------------


Missile::Missile( core::Image& img,
                  Vector2f pos, 
                  Vector2f v
                )
  : SpaceObject( img, pos, v, 1.0 ),
    bound( Vector2i(pos[0],pos[1]), img.width(), img.height() ) 
{
}


//------------------------------------------------------------------------------

void    
Missile::respawn( Vector2f pos, Vector2f v, float i )
{
    SpaceObject::respawn( pos, v, 1.0 );
    Sprite::invalidate_background();

    bound.move_to( Vector2i( pos[0], pos[1] ) );
}


//------------------------------------------------------------------------------

bool 
Missile::collides_with( SpaceObject* obj )
{
    Rect2D  ovr = overlap( bound, obj->cached_bound_rect() );

    return (ovr.is_null())  ? false  : true;
}


//------------------------------------------------------------------------------

void 
Missile::process_collision( SpaceObject* obj )
{
//    std::clog<<"missile hit\n";
    //if( dynamic_cast<PlayerShip*>(obj) )
    //    return;
    
    obj->change_integrity( -0.2 );
    //obj->kill();
    //this->change_integrity( -1.0 );
    this->kill();
}


//------------------------------------------------------------------------------

void 
Missile::update()
{
    SpaceObject::update();

    bound.move_to( Vector2i( position()[0], position()[1] ) );
}
