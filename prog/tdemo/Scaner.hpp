#if !defined _SCANER_HPP_
#define _SCANER_HPP_
//==============================================================================
//
//  Scaner decl   
//
//==============================================================================
//
//  externals: 

    #include <vector>
    
    #include "..\..\sys\WidgetView.hpp"


//==============================================================================
//
//  public:



  
class
ScanerView       
  : public gui::WidgetView  
{
  
  public:

                    ScanerView(  gui::GUImanager* gm, 
                                 const Rect2D& controlled_zone,
                                 const Rect2D& bound 
                              );
                   ~ScanerView();

    virtual void    redraw( core::Surface& dst_surf,
                            const Rect2D& clip_rectangle = Rect2D()
                          );

    void            set_controlled_zone( const Rect2D& rect );
    void            set_visible_zone( const Rect2D& rect );
    void            clear_all_objects();
    void            add_object( const Vector2i& pos, 
                                core::UDword color/*=def_obj_color*/
                              );

 
  private:

    Rect2D                      controlled_zone;
    Rect2D                      visible_zone;
    
    core::UDword                def_obj_color;
    core::UDword                def_bk_color;
    core::UDword                def_vis_color;

    std::vector<Vector2i>       obj_pos;
    std::vector<core::UDword>   obj_color;
};


//------------------------------------------------------------------------------

inline void            
ScanerView::set_controlled_zone( const Rect2D& rect )
{
    controlled_zone = rect; 
}


//------------------------------------------------------------------------------

inline void            
ScanerView::set_visible_zone( const Rect2D& rect )
{
    visible_zone = rect; 
}


//------------------------------------------------------------------------------

inline void            
ScanerView::clear_all_objects()
{
    obj_pos.clear();
    obj_color.clear();
}


//------------------------------------------------------------------------------

inline void 
ScanerView::add_object( const Vector2i& pos, core::UDword color )
{
    obj_pos.push_back( pos );
    obj_color.push_back( color );
}

#endif // _SCANER_HPP_