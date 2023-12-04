#if !defined _WIDGETVIEW_HPP_
#define _WIDGETVIEW_HPP_
//==============================================================================
//
//  WidgetView decl   
//
//==============================================================================
//
//  externals: 

    #include <string>
    
    #include "Sprite.hpp"
    #include "Font.hpp"


namespace gui
{
//==============================================================================
//
//  publics:

class GUImanager;


enum
FillStyle
{
    transparent,
    solid,
    h_gradient,
    v_gradient
};

enum
DrawStyle
{
    normal,
    h_interlaced,
    v_interlaced,
    double_interlaced   
};


//------------------------------------------------------------------------------
  



class
WidgetView 
  : public core::Sprite
{
  public:

                    WidgetView( GUImanager* gm, const Rect2D& bound_rect );
                   ~WidgetView();

    virtual void    redraw( core::Surface& dst_surf,
                            const Rect2D& clip_rectangle = Rect2D()
                          ) =0;
  
  private:

};


//------------------------------------------------------------------------------




class
TextElement
{
  
  public:
                    TextElement();
                   ~TextElement();

    void            redraw( core::Surface& dst_surf, const Vector2i& pos,
                            const Rect2D& clip_rectangle = Rect2D()
                          );

    void            set_text( const std::string& txt );
    void            set_font( core::RasterFont* fnt );
    void            set_text_color( gui::FillStyle style, 
                                    core::UDword color1=0x000000,
                                    core::UDword color2=0x000000
                                  );
    void            set_text_style( gui::DrawStyle style );

  private:
    
    void            redraw_text_solid( core::Surface& dst_surf,
                                       const Vector2i& pos,
                                       const Rect2D& clip_rectangle
                                     );
    void            redraw_text_vgrad( core::Surface& dst_surf,
                                       const Vector2i& pos,
                                       const Rect2D& clip_rectangle
                                     );

    
    std::string         text;
    core::RasterFont*   font;

    FillStyle           text_fill_style;
    DrawStyle           text_style;
    core::UDword        text_color1;
    core::UDword        text_color2;
};


//------------------------------------------------------------------------------




class
ProgressBarElement
{
  public:
                    ProgressBarElement( Size2i s, float v=0.0 );
                   ~ProgressBarElement();
    
    void            set_color( gui::FillStyle style, 
                               core::UDword color1=0x000000,
                               core::UDword color2=0x000000
                             );
    void            set_style( gui::DrawStyle s );
    void            set_value( float v );
    
    void            redraw( core::Surface& dst_surf, const Vector2i& pos,
                            const Rect2D& clip_rectangle = Rect2D()
                          );

  private:
    
    void            redraw_bar_solid( core::Surface& dst_surf,
                                      const Vector2i& pos,
                                      const Rect2D& clip_rectangle
                                    );
    void            redraw_bar_hgrad( core::Surface& dst_surf,
                                      const Vector2i& pos,
                                      const Rect2D& clip_rectangle
                                    );
    
    Size2i              size;
    float               value;
    
    DrawStyle           draw_style;
    FillStyle           fill_style;
    core::UDword        color1;
    core::UDword        color2;
};


//------------------------------------------------------------------------------




class
StaticTextView
  : public WidgetView,
    public TextElement
{
                    
  public:
                      
                    StaticTextView( GUImanager* gm, const Rect2D& bound );
                   ~StaticTextView();
    
    
    virtual void    redraw( core::Surface& dst_surf,
                            const Rect2D& clip_rectangle = Rect2D()
                          );
};


//------------------------------------------------------------------------------




class
ProgressBarView  
  : public WidgetView,
    public ProgressBarElement
{

  public:                    
                    
                    ProgressBarView( GUImanager* gm, const Rect2D& bound );
                   ~ProgressBarView();
    
    
    virtual void    redraw( core::Surface& dst_surf,
                            const Rect2D& clip_rectangle = Rect2D()
                          );
};



//==============================================================================
//
//  WidgetView:
//


//==============================================================================
//
//  TextView:
//

inline void            
TextElement::set_text( const std::string& txt )
{
    text = txt; 
}


//------------------------------------------------------------------------------

inline void            
TextElement::set_font( core::RasterFont* fnt )
{
     font = fnt;
}
    

//------------------------------------------------------------------------------

inline void            
TextElement::set_text_color( gui::FillStyle style, 
                                core::UDword color1, core::UDword color2 
                              )
{
    text_fill_style = style;
    text_color1     = color1;
    text_color2     = color2;
}


//------------------------------------------------------------------------------

inline void            
TextElement::set_text_style( gui::DrawStyle style )
{
    text_style = style; 
}



//==============================================================================
//
//
//  ProgressBarElement
    

inline void            
ProgressBarElement::set_color( gui::FillStyle style, 
                            core::UDword color1, core::UDword color2 
                          )
{
    fill_style   = style;
    this->color1 = color1;
    this->color2 = color2;
}


//------------------------------------------------------------------------------

inline void            
ProgressBarElement::set_style( gui::DrawStyle s )
{
    draw_style = s;     
}


//------------------------------------------------------------------------------

inline void            
ProgressBarElement::set_value( float v )
{
    value = (v<1.0)  ? v  : 1.0;
} 

} // namespace gui
#endif // _WIDGETVIEW_HPP_