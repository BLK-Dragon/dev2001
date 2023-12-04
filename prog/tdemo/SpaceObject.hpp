#if !defined _SPACEOBJECT_HPP_
#define _SPACEOBJECT_HPP_
//==============================================================================
//
//  Base class for space objects   
//
//==============================================================================
//
//  externals: 

    #include "..\..\sys\Sprite.hpp"


//==============================================================================
//
//  publics:




class
SpaceObject  
  : public core::Sprite
{
  public:

                    SpaceObject( core::Image& img,
                                 Vector2f pos, 
                                 Vector2f v=Vector2f(0,0), 
                                 float i=1.0 
                               );
                   ~SpaceObject();

    virtual void    respawn( Vector2f pos, 
                             Vector2f v=Vector2f(0,0), 
                             float i=1.0 
                           );
    bool            is_dead() const;
    bool            respawn_enabled() const;
    void            resurrect();
    void            kill();
    
    Vector2f        position() const;
    Vector2f        velocity() const;
    float           integrity() const;
    virtual Rect2D  cached_bound_rect() const =0;
    virtual bool    collides_with( SpaceObject* obj ) =0;

    void            accelerate( const Vector2f& v );
    void            decelerate( const Vector2f& v );
    
    void            set_velocity( const Vector2f& v );
    void            add_velocity( const Vector2f& dv );
    void            scale_velocity( float sv );
    virtual void    change_integrity( float di );
    
    virtual void    update();
    
    virtual void    process_collision( SpaceObject* obj ) =0;

    void            redraw( core::Surface& dst_surf, 
                            const Rect2D& clip_rectangle = Rect2D()
                          );
    void            save_background( core::Surface& src_surf,
                                     const Rect2D& clip_rectangle=Rect2D() 
                                   );
    void            restore_background( core::Surface& dst_surf,
                                        const Vector2i& offset,
                                        const Rect2D& clip_rectangle 
                                            = Rect2D()
                                      );

    static void     set_origin( const Vector2i& o );
    static void     change_origin( const Vector2i& delta );

    void            set_max_speed( const Vector2f& s );
    void            set_rest_speed( const Vector2f& s );
  

  protected:

    static Vector2i origin;

    bool            dead; 
    bool            reborn;
    Vector2f        reborn_pos;
    
    Vector2f        cur_position;
    Vector2f        cur_velocity;
    float           cur_integrity;

    int             x_dir;
    int             y_dir;

    Vector2f        max_speed;
    Vector2f        rest_speed;
};



//------------------------------------------------------------------------------

inline
SpaceObject::SpaceObject( core::Image& img, 
                          Vector2f pos, Vector2f v, float i 
                        )
  : Sprite( img ),
    dead(true)
{
    SpaceObject::respawn( pos, v, i);
    reborn = false;
}


//------------------------------------------------------------------------------

inline
SpaceObject::~SpaceObject()
{
}


//------------------------------------------------------------------------------

inline void 
SpaceObject::respawn( Vector2f pos, Vector2f v, float i )
{
    dead        = false;
    reborn      = true;
    reborn_pos  = cur_position;
    
    cur_position    = pos;
    cur_velocity    = v;
    cur_integrity   = i;

    if( v[0] != 0 )
        x_dir = (v[0]>0)  ? 1  : -1;
    
    if( v[1] != 0 )
        y_dir = (v[1]>0)  ? 1  : -1;

    max_speed = Vector2f(5,5);
}


//------------------------------------------------------------------------------

inline bool            
SpaceObject::is_dead() const
{
    return dead; 
}


//------------------------------------------------------------------------------

inline bool            
SpaceObject::respawn_enabled() const
{
    return (!reborn);
}


//------------------------------------------------------------------------------

inline void            
SpaceObject::resurrect()
{
    dead = false; 
}


//------------------------------------------------------------------------------

inline void            
SpaceObject::kill()
{
    dead        = true; 
    reborn      = true;
    reborn_pos  = cur_position;
}


//------------------------------------------------------------------------------

inline Vector2f 
SpaceObject::position() const
{
    return cur_position;
}


//------------------------------------------------------------------------------

inline Vector2f 
SpaceObject::velocity() const
{
    return cur_velocity;
}


//------------------------------------------------------------------------------

inline float 
SpaceObject::integrity() const
{
    return cur_integrity;
}


//------------------------------------------------------------------------------

inline void            
SpaceObject::accelerate( const Vector2f& v )
{
     cur_velocity += v;
     
     int    sign;

     sign = (cur_velocity[0] >= 0)  ? 1  : -1;
     
     if( fabs(cur_velocity[0]) > max_speed[0] )
            cur_velocity[0] = sign * max_speed[0];

     sign = (cur_velocity[1] >= 0)  ? 1  : -1;
     
     if( fabs(cur_velocity[1]) > max_speed[1] )
            cur_velocity[1] = sign * ( max_speed[1] );
}


//------------------------------------------------------------------------------

inline void            
SpaceObject::decelerate( const Vector2f& v )
{
    int     v_sign;
    int     old_sign;
    int     new_sign;
    int     old_delta_sign;
    int     new_delta_sign;


    if( cur_velocity[0] != 0 )
    {
        v_sign   = (v[0]>=0)  ? 1  : -1;
        old_sign = (cur_velocity[0] >= 0)  ? 1  : -1;

        if( old_sign != v_sign )
        {
            cur_velocity[0] += v[0];
            new_sign        = (cur_velocity[0] >= 0)  ? 1  : -1;

            if( old_sign != new_sign )
                cur_velocity[0] = 0.0;
        }
    }
    
    if( cur_velocity[1] != rest_speed[1] )
    {
        v_sign          = (v[1]>=0)  ? 1  : -1;
        old_sign        = (cur_velocity[1] >= 0)  ? 1  : -1;
        old_delta_sign  = ( (cur_velocity[1]-rest_speed[1]) >= 0)  ? 1  : -1;

        if( old_sign != v_sign )
        {
            cur_velocity[1] += v[1];
            //new_sign        = (cur_velocity[1] >= 0)  ? 1  : -1;
            new_delta_sign  = ( (cur_velocity[1]-rest_speed[1]) >= 0)  ? 1  : -1;
            
            if( old_delta_sign != new_delta_sign )
                cur_velocity[1] = rest_speed[1];
        }
    } 
}


//------------------------------------------------------------------------------

inline void            
SpaceObject::set_velocity( const Vector2f& v )
{
    cur_velocity = v;
}


//------------------------------------------------------------------------------

inline void            
SpaceObject::add_velocity( const Vector2f& dv )
{
    cur_velocity += dv;
}


//------------------------------------------------------------------------------

inline void
SpaceObject::scale_velocity( float sv )
{
    cur_velocity *= sv;     
}


//------------------------------------------------------------------------------

inline void 
SpaceObject::change_integrity( float di )
{
    cur_integrity += di;

    cur_integrity = (cur_integrity<0.0)  ? 0.0  : 
                    (cur_integrity>1.0)  ? 1.0  : cur_integrity;
}


//------------------------------------------------------------------------------

inline void    
SpaceObject::update()
{
    if( dead ) return;
    
    cur_position += cur_velocity;
    Sprite::update();     
}


//------------------------------------------------------------------------------

inline void            
SpaceObject::redraw( core::Surface& dst_surf, const Rect2D& clip_rectangle )
{
    if( dead )  return;
    
    core::Sprite::place( Vector2i( cur_position[0], 
                                   cur_position[1] - origin[1]
                                 ) 
                       );
    core::Sprite::redraw( dst_surf, clip_rectangle );
}


//------------------------------------------------------------------------------
    
inline void            
SpaceObject::save_background( core::Surface& src_surf, 
                              const Rect2D& clip_rectangle 
                            )
{
    if( dead )  return;
    
    core::Sprite::place( Vector2i( cur_position[0], 
                                   cur_position[1] - origin[1]
                                 ) 
                       );
    core::Sprite::save_background( src_surf, clip_rectangle ); 
}


//------------------------------------------------------------------------------
    
inline void            
SpaceObject::restore_background( core::Surface& dst_surf,
                                 const Vector2i& offset,
                                 const Rect2D& clip_rectangle 
                               )
{
    if( dead  &&  (!reborn) ) return;
    
    if( reborn )
    {
        core::Sprite::place( Vector2i( reborn_pos[0],
                                       reborn_pos[1] - origin[1]
                                     )
                           );
        cur_integrity   = -1.0;
        reborn          = false;
    }
    else
    {
        core::Sprite::place( Vector2i( cur_position[0],
                                       cur_position[1] - origin[1]
                                     )
                           );
    }
    
    core::Sprite::restore_background( dst_surf, offset, clip_rectangle );
}


//------------------------------------------------------------------------------

inline void     
SpaceObject::set_origin( const Vector2i& o )
{
    origin = o;
}


//------------------------------------------------------------------------------

inline void     
SpaceObject::change_origin( const Vector2i& delta )
{
    origin += delta;     
}


//------------------------------------------------------------------------------

inline void
SpaceObject::set_max_speed( const Vector2f& s )
{
    max_speed = s; 
}


//------------------------------------------------------------------------------

inline void
SpaceObject::set_rest_speed( const Vector2f& s )
{
    rest_speed = s; 
}




#endif // _SPACEOBJECT_HPP_