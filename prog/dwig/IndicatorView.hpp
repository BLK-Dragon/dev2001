#if !defined _INDICATORVIEW_HPP_

//==============================================================================
//
//
//
//==============================================================================
//
//  externals:
    
    #include "..\..\sys\Sprite.hpp"
    #include "..\..\sys\Rect.hpp"
    
    #include "View.hpp"
    #include "Indicator.hpp"


//==============================================================================
//
//  publics:




class
IndicatorView 
  : public View
{
  public:    
  
//                    IndicatorView( Indicator* indicator, core::Image& img );
    
    virtual void    save_background();
    virtual void    redraw();
    virtual void    restore_background();
  
  
  private:
/*    
    Indicator*      indicator;
    core::Sprite    indicator_pic;
    Rect2D          bound_rect;
*/
};

#endif // _INDICATORVIEW_HPP_
