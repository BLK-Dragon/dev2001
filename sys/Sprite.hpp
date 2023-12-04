#if !defined _SPRITE_HPP_
#define _SPRITE_HPP_
//==============================================================================
//
//  Sprite decl   
//
//==============================================================================
//
//  externals: 

    #include "Image.hpp"
    #include "Rect.hpp"

namespace core
{
//==============================================================================
//
//  publics:




class
Sprite  
{
                    Sprite( const Sprite& );
    Sprite&         operator=( const Sprite& );
  
  public:

                    Sprite( Image& img );
                    Sprite( int w, int h );
                   ~Sprite();

    void            place( const Vector2i& pos );
    void            move( const Vector2i& delta );
    
    void            redraw( Surface& dst_surf, 
                            const Rect2D& clip_rectangle = Rect2D()
                          );
    void            save_background( Surface& src_surf,
                                     const Rect2D& clip_rectangle=Rect2D() 
                                   );
    void            restore_background( Surface& dst_surf,
                                        const Vector2i& offset = Vector2i(0,0),
                                        const Rect2D& clip_rectangle 
                                            = Rect2D()
                                      );
    void            invalidate_background();

    int             num_phases() const;
    int             current_phase() const;
    void            set_phase( int p );

    void            set_animation_length( int n );
    void            set_animation_speed( float s );

    void            update();

    Vector2i        position() const;
    Rect2D          bound_rect() const;

  
  private:
    
    bool            is_virtual;
    int             v_width;
    int             v_height;

    int             width() const;
    int             height() const;
    Image&          image;
    Surface         background;
    bool            no_saved_background;
    Rect2D          saved_spr_rect;
    Rect2D          saved_scr_rect;
    Vector2i        cur_position;

    float           cur_frame;
    float           ani_speed; 
    int             ani_length;
    int             ani_start;

    Rect2D          src_rect( const Surface& dst_surf, 
                              const Rect2D& clip_rectangle 
                            );
};
    

//------------------------------------------------------------------------------

inline int             
Sprite::current_phase() const
{
    return cur_frame;
}


//------------------------------------------------------------------------------

inline void 
Sprite::place( const Vector2i& pos )
{
    cur_position = pos;
}


//------------------------------------------------------------------------------

inline void
Sprite::move( const Vector2i& delta )
{
    cur_position += delta; 
}


//------------------------------------------------------------------------------

inline void            
Sprite::set_phase( int p )
{
    while( p > image.num_images()-1 )
        p -= image.num_images();

    cur_frame   = p;
    ani_start   = p;
}


//------------------------------------------------------------------------------

inline void            
Sprite::set_animation_length( int n )
{
    ani_length = n;
}


//------------------------------------------------------------------------------

inline void            
Sprite::set_animation_speed( float s )
{
    ani_speed = s;
}


//------------------------------------------------------------------------------

inline void            
Sprite::update()
{
    cur_frame += ani_speed;

    if( cur_frame > image.num_images()-1 )
    {
        while( cur_frame > image.num_images()-1 )
            cur_frame -= image.num_images();
        
        return;     
    }
    
    if( cur_frame < 0 )
    {
        while( cur_frame < 0 )
            cur_frame += image.num_images();

        return;
    }
        
    /*
    if( cur_frame > ani_start+ani_length )
        cur_frame -= ani_length;    
    */
}


//------------------------------------------------------------------------------

inline Vector2i 
Sprite::position() const
{
    return cur_position;
}


//------------------------------------------------------------------------------

inline Rect2D
Sprite::bound_rect() const
{
    if( !is_virtual )
    {
        return Rect2D( cur_position, image.width(), image.height() );
    }
    else
    {
        return Rect2D( cur_position, v_width, v_height );
    }
}


//------------------------------------------------------------------------------

inline int
Sprite::num_phases() const
{
    return image.num_images(); 
}


//------------------------------------------------------------------------------

inline int             
Sprite::width() const
{
    return (is_virtual)  ? v_width  : image.width(); 
}


//------------------------------------------------------------------------------

inline int             
Sprite::height() const
{
    return (is_virtual)  ? v_height  : image.height();
}


//------------------------------------------------------------------------------

inline void            
Sprite::invalidate_background()
{
    saved_scr_rect  = Rect2D(); 
    saved_spr_rect  = Rect2D(); 
}




} // namespace core
#endif // _SPRITE_HPP_