#if !defined _MISSILE_HPP_
#define _MISSILE_HPP_
//==============================================================================
//
//  Missile decls   
//
//==============================================================================
//
//  externals:

    #include "SpaceObject.hpp"


//==============================================================================
//
//
//  publics:




class
Missile
  : public SpaceObject
{
  
  public:

                    Missile( core::Image& img,
                             Vector2f pos=Vector2f(0,0), 
                             Vector2f v=Vector2f(0,0)
                           );
                   ~Missile();

    virtual void    respawn( Vector2f pos, 
                             Vector2f v=Vector2f(0,0),
                             float i = 1.0
                           );
    
    virtual void    update();
    
    virtual Rect2D  cached_bound_rect() const;
    virtual bool    collides_with( SpaceObject* obj );
    virtual void    process_collision( SpaceObject* obj );
  
  
  private:

    Rect2D          bound;

};


//------------------------------------------------------------------------------

inline Rect2D 
Missile::cached_bound_rect() const
{
    return bound;
}


#endif // _MISSILE_HPP_