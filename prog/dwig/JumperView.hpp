#if !defined _JUMPERVIEW_HPP_
#define _JUMPERVIEW_HPP_
//==============================================================================
//
//  Jumper View decl
//
//==============================================================================
//
//  externals:

    #include "..\..\sys\Sprite.hpp"
    #include "..\..\sys\Rect.hpp"
    
    #include "View.hpp"
    #include "Jumper.hpp"


//==============================================================================
//
//  publics:




class
JumperView 
  : public View
{
        
  public:

//                    JumperView( Jumper* jumper, core::Image& img );
    
    virtual void    save_background();
    virtual void    redraw();
    virtual void    restore_background();

  
  private:
/*
    Jumper*         jumper;
    core::Sprite    jumper_pic;
    Rect2D          bound_rect;
*/
};

#endif // _JUMPERVIEW_HPP_
