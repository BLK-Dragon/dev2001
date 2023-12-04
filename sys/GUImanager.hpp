#if !defined _GUIMANAGER_HPP_
#define _GUIMANAGER_HPP_
//==============================================================================
//
//  GUI manager decls   
//
//==============================================================================
//
//  externals: 

    #include <vector>    

    #include "Surface.hpp"


namespace gui
{
//==============================================================================
//
//  publics:

class WidgetView;



class
GUImanager  
{
  public:    
    
                    GUImanager();
                   ~GUImanager();

    void            register_widget_view( WidgetView* wv );

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
  
  private:
    std::vector<WidgetView*>    widget_view;
};

} // namspace gui
#endif // _GUIMANAGER_HPP_