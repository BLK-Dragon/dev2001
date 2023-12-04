#if !defined _EXPLOSION_HPP_
#define _EXPLOSION_HPP_
//==============================================================================
//
//  Explosion.hpp   
//
//==============================================================================
//
//  externals: 

    #include "SpaceObject.hpp"


//==============================================================================
//
//  publics:




class
Explosion  
  : public SpaceObject
{
  
  public:

                    Explosion( core::Image& img,
                               Vector2f pos, 
                               Vector2f v=Vector2f(0,0), 
                               float i=1.0 
                             );
                   ~Explosion();

    virtual void    respawn( Vector2f pos, 
                             Vector2f v=Vector2f(0,0), 
                             float i=1.0 
                           );

    virtual void    update();
    virtual Rect2D  cached_bound_rect() const;
    virtual bool    collides_with( SpaceObject* obj );
    
    virtual void    change_integrity( float di );

    virtual void    process_collision( SpaceObject* obj );
  
  
  private:

    Rect2D          bound;
};


//------------------------------------------------------------------------------

inline void    
Explosion::change_integrity( float di )
{
}

#endif // _EXPLOSION_HPP_