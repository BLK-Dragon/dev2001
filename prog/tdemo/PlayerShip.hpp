#if !defined _PLAYERSHIP_HPP_
//==============================================================================
//
//  PlayerShip decls
//
//==============================================================================
//
//  externals:
    
    #include "..\..\sys\ParamSet.hpp"
    
    #include "SpaceObject.hpp"


//==============================================================================
//
//  publics:




class PlayerShip
  : public SpaceObject
{
  public:
    
    enum Direction;                        
                        
                        PlayerShip( core::Image& img,
                                    Vector2f pos, 
                                    Vector2f v=Vector2f(0,0), 
                                    float i=1.0 
                                  );
    
    virtual void        update();
    virtual Rect2D      cached_bound_rect() const;
    virtual bool        collides_with( SpaceObject* obj );
    virtual void        process_collision( SpaceObject* obj );
    
    void                accelerate( Direction d );
    void                decelerate( Direction d );

    void                set_constrain_rect( const Rect2D& rect );

    static void         register_paramset( core::ParamSetManager* manager );    
    
    enum
    Direction
    {
         left,
         right,
         up,
         down
    };

  private:
    
    static void                     bind_members( core::ParamSetPool* pool );
    static core::ParamSetManager*   paramset_manager;
    

    Rect2D              bound;
    Rect2D              constrain_rect;
    Vector2f            acc_speed;
    Vector2f            dec_speed;
};


//------------------------------------------------------------------------------

inline Rect2D
PlayerShip::cached_bound_rect() const
{
    return bound;
}


//------------------------------------------------------------------------------

inline void                
PlayerShip::set_constrain_rect( const Rect2D& rect )
{
     constrain_rect = rect;
}


#endif // _PLAYERSHIP_HPP_

