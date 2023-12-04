#if !defined _CLIPVIEW_HPP_
#define _CLIPVIEW_HPP_
//==============================================================================
//
//  Clip View decl
//
//==============================================================================
//
//  externals:
    
    #include "..\..\sys\Sprite.hpp"
    #include "..\..\sys\Rect.hpp"
    
    #include "View.hpp"
    #include "Clip.hpp"


//==============================================================================
//
//  publics:




class
ClipView 
  : public View
{
  public:    
    
//                    ClipView( Clip* clip, core::Image& img );
    
    virtual void    save_background();
    virtual void    redraw();
    virtual void    restore_background();
    
  private:
/*    
    Clip*           clip;
    core::Sprite    clip_pic;
    Rect2D          bound_rect;
*/
};

#endif // _CLIPVIEW_HPP_
