//==============================================================================
//
//  Explosion realization   
//
//==============================================================================
//
//  externals: 

    #include "Explosion.hpp"


//==========================================================================
//
//
//  publics:



//------------------------------------------------------------------------------


Explosion::Explosion( core::Image& img, Vector2f pos, Vector2f v, float i )
  : SpaceObject( img, pos, v, i ),
    bound( Vector2i(pos[0],pos[1]), img.width(), img.height() )
{
}


//------------------------------------------------------------------------------

Explosion::~Explosion()
{
}


//------------------------------------------------------------------------------

void
Explosion::respawn( Vector2f pos, Vector2f v, float i )
{
    SpaceObject::respawn( pos, v, i );
    Sprite::invalidate_background();
    
    bound.move_to( Vector2i( pos[0], pos[1] ) );
    Sprite::set_phase(0);
}


//------------------------------------------------------------------------------

void    
Explosion::update()
{
    SpaceObject::update();

    bound.move_to( Vector2i( position()[0], position()[1] ) );
    
    if( Sprite::current_phase() == Sprite::num_phases() - 2 )
        this->kill();
}


//------------------------------------------------------------------------------

Rect2D
Explosion::cached_bound_rect() const
{
    return bound;
}


//------------------------------------------------------------------------------

bool
Explosion::collides_with( SpaceObject* obj )
{
    Rect2D  ovr = overlap( bound, obj->cached_bound_rect() );

    return (ovr.is_null())  ? false  : true;
}


//------------------------------------------------------------------------------

void
Explosion::process_collision( SpaceObject* obj )
{
}
    