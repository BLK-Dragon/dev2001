//==============================================================================
//
//  Sprite realization   
//
//==============================================================================
//
//  externals: 

    #include "Sprite.hpp"
    #include "Type.hpp"

    #include <fstream>
    #include <iomanip>
    

namespace core
{
//==============================================================================
//
//  publics:

    
Sprite::Sprite( Image& img )
  : is_virtual(false),
    image( img ),
    cur_position( Vector2i(0,0) ),
    cur_frame(0),
    ani_speed(0),
    ani_length(0),
    ani_start(0),
    background( image.width(), image.height()  ),
    no_saved_background(true)
{
}


//------------------------------------------------------------------------------

Sprite::Sprite( int w, int h )
  : is_virtual(true),
    image( reinterpret_cast<Image&>(image) ),
    cur_position( Vector2i(0,0) ),
    cur_frame(0),
    ani_speed(0),
    ani_length(0),
    ani_start(0),
    background( w, h ),
    no_saved_background(true),
    v_width(w),
    v_height(h)
{
     
}


//------------------------------------------------------------------------------

Sprite::~Sprite()
{
}


//------------------------------------------------------------------------------

void 
Sprite::redraw( Surface& dst_surf, const Rect2D& clip_rectangle )
{   
    if( is_virtual )
        return;
   
    Rect2D      src_rectangle   = src_rect( dst_surf, clip_rectangle );
    int         v_offset        = (int)cur_frame * height();
    Vector2i    src_point       = Vector2i( src_rectangle.left(), 
                                            src_rectangle.top() - v_offset
                                          );

    if( src_rectangle.height() <= 0  ||
        src_rectangle.width() <= 0 
      )
    {
        return;
    }    
    
    image.image().blit_to( dst_surf, cur_position + src_point, src_rectangle );
}


//------------------------------------------------------------------------------

void            
Sprite::save_background(  Surface& src_surf, const Rect2D& clip_rectangle )
{
    //std::clog << width() << "x" << height() << "\n"
    //          << std::flush;
    Rect2D  clip_rect = clip_rectangle.is_null()
                        ? Rect2D( 0, 0,
                                  src_surf.width()-1, src_surf.height()-1
                                )
                        : clip_rectangle;
    
    
    Rect2D  spr_rectangle   = src_rect( src_surf, clip_rectangle );
    int     v_offset        = (int)cur_frame * height();
    int     x               = (cur_position[0] < clip_rect.left())  
                              ?  clip_rect.left()  :
                              (cur_position[0] < clip_rect.right())
                              ?  cur_position[0]  : clip_rect.right();

    int     y               = (cur_position[1] < clip_rect.top())  
                              ?  clip_rect.top()  :
                              (cur_position[1] < clip_rect.bottom())
                              ?  cur_position[1]  : clip_rect.bottom();

    spr_rectangle.resize( 0, -v_offset, 0, -v_offset );
    
    Rect2D  scr_rectangle   = Rect2D( x, y,
                                      x + spr_rectangle.width(),
                                      y + spr_rectangle.height()
                                    );

    if( scr_rectangle.height() <= 0  ||  
        scr_rectangle.width() <= 0   ||
        spr_rectangle.height() <= 0 ||
        spr_rectangle.width() <= 0 
      )
    {  
        saved_spr_rect      = Rect2D();
        return;
    }

    //std::clog << "save\n" << std::flush;
    src_surf.blit_to( background, spr_rectangle, scr_rectangle );    
    saved_scr_rect      = scr_rectangle;
    saved_spr_rect      = spr_rectangle;
    no_saved_background = false;
}


//------------------------------------------------------------------------------
    
void            
Sprite::restore_background( Surface& dst_surf,
                            const Vector2i& offset,
                            const Rect2D& clip_rectangle                                             
                          )
{   
    if( no_saved_background )
        return;
    
    Rect2D  clip_rect       = clip_rectangle.is_null()
                              ? Rect2D( 0, 0,
                                        dst_surf.width()-1, dst_surf.height()-1
                                      )
                              : clip_rectangle;
    if( cur_position[1] > clip_rect.bottom() )
        return;


    Rect2D  scr_rectangle   = saved_scr_rect;
    
    
    scr_rectangle += offset;
    
    if( scr_rectangle.bottom() >= clip_rect.bottom() )
    {
        scr_rectangle.resize( 0, 0, 0, -offset[1] ); 
        saved_spr_rect.resize( 0, 0, 0, -offset[1] ); 
    }
    
    if( scr_rectangle.height() <= 0  ||  
        scr_rectangle.width() <= 0   ||
        saved_spr_rect.height() <= 0 ||
        saved_spr_rect.width() <= 0 
      )
    {  
        return;
    }
    /*
    std::clog << "restore " << this << "\n" << std::flush;
    std::clog << "scr: " << scr_rectangle.width() << "x" 
              << scr_rectangle.height() << "\n" << std::flush;
    std::clog << "spr: " << saved_spr_rect.width() << "x" 
              << saved_spr_rect.height() << "\n" << std::flush;
    std::clog << "src surf: " << &background << "\n" << std::flush;
    std::clog << "dst surf: " << &dst_surf << "\n\n" << std::flush;
    */
    background.blit_to( dst_surf, scr_rectangle, saved_spr_rect );
}


//------------------------------------------------------------------------------

Rect2D
Sprite::src_rect( const Surface& dst_surf, const Rect2D& clip_rectangle )
{
    int     v_offset  = (int)cur_frame * height();

    Rect2D  clip_rect = clip_rectangle.is_null()
                        ? Rect2D( 0, 0,
                                  dst_surf.width()-1, dst_surf.height()-1
                                )
                        : clip_rectangle;
    
    int     left;
    int     right;
    int     top;
    int     bottom;

    if( cur_position[0] + width() < clip_rect.left()   ||
        cur_position[0] > clip_rect.right()                  ||
        cur_position[1] + height()-1 < clip_rect.top()   ||
        cur_position[1] > clip_rect.bottom()
      )
    {
        return Rect2D();
    }

    if( cur_position[0] < clip_rect.left() )
    {
        left    = clip_rect.left() - cur_position[0];
        right   = width() -1 ;
    }
    else if( cur_position[0]+width() > clip_rect.right() )
    {
        left    = 0;
        right   = clip_rect.right() - cur_position[0];
    }
    else
    {
        left    = 0;
        right   = width() - 1;
    }

    if( cur_position[1] < clip_rect.top() )
    {
        top     = clip_rect.top() - cur_position[1];
        bottom  = height() - 1;
    }
    else if( cur_position[1]+height() > clip_rect.bottom() )
    {
        top     = 0;
        bottom  = clip_rect.bottom() - cur_position[1];
    }
    else
    {
        top     = 0;
        bottom  = height() - 1;
    }

    return Rect2D( left, top+v_offset, right, bottom+v_offset ); 
}





} // namespace core

