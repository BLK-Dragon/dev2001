#if !defined _ASTEROID_HPP_
#define _ASTEROID_HPP_
//==============================================================================
//
//  Asteroid decl   
//
//==============================================================================
//
//  externals: 

    #include "SpaceObject.hpp"


//==============================================================================
//
//  publics:




class
Asteroid
  : public SpaceObject  
{
  public:

                    Asteroid( core::Image& img,
                              Vector2f pos, 
                              Vector2f v=Vector2f(0,0), 
                              float i=1.0 
                            );
                   ~Asteroid();
    
    virtual void    respawn( Vector2f pos, 
                             Vector2f v=Vector2f(0,0), 
                             float i=1.0 
                           );

    virtual void    update();
    virtual Rect2D  cached_bound_rect() const;
    virtual bool    collides_with( SpaceObject* obj );

    virtual void    process_collision( SpaceObject* obj );

  
  private:

    Rect2D          bound;
};




//------------------------------------------------------------------------------

inline
Asteroid::Asteroid( core::Image& img,
                    Vector2f pos, Vector2f v, float i
                  )
  : SpaceObject( img, pos, v, i ),
    bound( Vector2i(pos[0],pos[1]), img.width(), img.height() ) 
{
}


//------------------------------------------------------------------------------

inline
Asteroid::~Asteroid()
{
}


//------------------------------------------------------------------------------

inline Rect2D 
Asteroid::cached_bound_rect() const
{
    return bound;
}

#endif // _ASTEROID_HPP_