//==============================================================================
//
//  WidgetView realization   
//
//==============================================================================
//
//  externals: 

    #include "WidgetView.hpp"
    #include "GUImanager.hpp"
//    #include "Graph.hpp"
    #include "BitOps.hpp"

namespace gui 
{
//==============================================================================
//
//  publics:



//==============================================================================
//
//  WidgetView:
//

WidgetView::WidgetView( GUImanager* gm, const Rect2D& bound_rect )
  : core::Sprite( bound_rect.width(), bound_rect.height() )
{
    Sprite::place( Vector2i(bound_rect.left(),bound_rect.top()) );
    gm->register_widget_view( this);
}


//------------------------------------------------------------------------------

WidgetView::~WidgetView()
{
}


//==============================================================================
//
// StaticTextElement:
//

TextElement::TextElement()
  : font(0),
    text_fill_style(gui::solid),
    text_color1(0xFFFFFF),
    text_style(gui::normal)
{
}


//------------------------------------------------------------------------------

TextElement::~TextElement()
{
}


//------------------------------------------------------------------------------

void 
TextElement::redraw( core::Surface& dst_surf, const Vector2i& pos,
                     const Rect2D& clip_rectangle
                   )
{
    if( !font )
        throw core::Error("no font specified for StaticTextView");
    
    switch( text_fill_style )
    {
        case solid :
            redraw_text_solid( dst_surf, pos, clip_rectangle );
            break;

        case v_gradient :
            redraw_text_vgrad( dst_surf, pos, clip_rectangle );
            break;
 
    }
}


//------------------------------------------------------------------------------

void 
TextElement::redraw_text_solid( core::Surface& dst_surf,
                                const Vector2i& pos,
                                const Rect2D& clip_rectangle
                              )
{
//    vfx::Graph      graph( dst_surf );

    const char*     str     = text.c_str();
    int             x_pos   = pos[0];
    int             y_pos   = pos[1];
    const UByte*    color   = (const UByte*)&text_color1;
    int             scale   = (text_style==double_interlaced)  ? 2  : 1;

    while( *str != '\0' )
    {
        const UByte*  char_data   = font->char_data( *str );
        UByte         mask[]      = {128,64,32,16,8,4,2,1};

        for( int line=0; line<font->max_height(); line++ )
        {
            for( int i=0; i<char_data[0]; i++ )
            {
                if( *(char_data + 1 + line) & mask[i] )
                {
                    dst_surf.set_pixel( x_pos + scale*i, y_pos + scale*line,
                                        color[2], color[1], color[0]
                                      );

                }
            }
        }

        x_pos += scale * ( char_data[0] + font->def_char_spacing() );

        ++str;
    }


}


//------------------------------------------------------------------------------

void 
TextElement::redraw_text_vgrad( core::Surface& dst_surf,
                                const Vector2i& pos,
                                const Rect2D& clip_rectangle
                              )
{
//    core::Graph     graph( dst_surf );

    const char*     str     = text.c_str();
    int             x_pos   = pos[0];
    int             y_pos   = pos[1];
    int             scale   = (text_style==double_interlaced)  ? 2  : 1;

    vfx::ColorInterpolation     grad( text_color1, text_color2, 
                                      font->max_height()
                                    );

    while( *str != '\0' )
    {
        const UByte*  char_data   = font->char_data( *str );
        UByte         mask[]      = {128,64,32,16,8,4,2,1};

        for( int line=0; line<font->max_height(); line++ )
        {
            core::UDword    txt_color   = grad[line];
            core::UByte*    color       = (core::UByte*)&txt_color;
            
            for( int i=0; i<char_data[0]; i++ )
            {
                if( *(char_data + 1 + line) & mask[i] )
                {
                    dst_surf.set_pixel( x_pos + scale*i, y_pos + scale*line,
                                        color[0], color[1], color[2]
                                      );      

                }
            }
        }

        x_pos += scale * ( char_data[0] + font->def_char_spacing() );

        ++str;
    }


}



//==============================================================================
//
// StaticTextView:
//

StaticTextView::StaticTextView( GUImanager* gm, const Rect2D& bound )
  : WidgetView( gm, bound )
{
}


//------------------------------------------------------------------------------

StaticTextView::~StaticTextView()
{
}


//------------------------------------------------------------------------------

void 
StaticTextView::redraw( core::Surface& dst_surf,
                        const Rect2D& clip_rectangle
                      )
{
    TextElement::redraw( dst_surf, position(), clip_rectangle );
}


//==============================================================================
//
//
//  ProgressBarElement

ProgressBarElement::ProgressBarElement( Size2i s, float v )
  : size(s),
    value(v)
{
}


//------------------------------------------------------------------------------


ProgressBarElement::~ProgressBarElement()
{
}


//------------------------------------------------------------------------------

void 
ProgressBarElement::redraw( core::Surface& dst_surf, const Vector2i& pos,
                            const Rect2D& clip_rectangle
                          )
{
    switch( fill_style )
    {
        case solid :
            redraw_bar_solid( dst_surf, pos, clip_rectangle );
            break;

        case h_gradient :
            redraw_bar_hgrad( dst_surf, pos, clip_rectangle );
            break;
    }
}


//------------------------------------------------------------------------------

void            
ProgressBarElement::redraw_bar_solid( core::Surface& dst_surf,
                                      const Vector2i& pos,
                                      const Rect2D& clip_rectangle
                                    )
{
     core::UByte*   c       = (core::UByte*)&color1;
     int            xstep   = (draw_style==v_interlaced || 
                               draw_style==double_interlaced)
                              ? 2
                              : 1;
     int            ystep   = (draw_style==h_interlaced || 
                               draw_style==double_interlaced)
                              ? 2
                              : 1;

     
     for( int y=pos[1]; y<pos[1]+size[1]; y+=ystep )
     {
         for( int x=pos[0]; x<pos[0]+value*size[0]; x+=xstep )
         {
            dst_surf.set_pixel( x, y, c[0], c[1], c[2] );
         } 
     }
}


//------------------------------------------------------------------------------

void            
ProgressBarElement::redraw_bar_hgrad( core::Surface& dst_surf,
                                      const Vector2i& pos,
                                      const Rect2D& clip_rectangle
                                    )
{
     int            xstep   = (draw_style==v_interlaced ||
                               draw_style==double_interlaced)
                              ? 2
                              : 1;
     int            ystep   = (draw_style==h_interlaced ||
                               draw_style==double_interlaced)
                              ? 2
                              : 1;
    
    vfx::ColorInterpolation     grad( color1, color2, size[0] );


     for( int y=pos[1]; y<pos[1]+size[1]; y+=ystep )
     {
         for( int x=pos[0]; x<pos[0]+value*size[0]; x+=xstep )
         {
            core::UDword    color   = grad[x-pos[0]];
            core::UByte*    c       = (core::UByte*)&color;
            
            dst_surf.set_pixel( x, y, c[0], c[1], c[2] );
         }
     }
}


//==============================================================================
//
//
//  ProgressBarView



//------------------------------------------------------------------------------

ProgressBarView::ProgressBarView( GUImanager* gm, const Rect2D& bound )
  : WidgetView( gm, bound ),
    ProgressBarElement( Size2i(bound.width(),bound.height()) )
{
}


//------------------------------------------------------------------------------


ProgressBarView::~ProgressBarView()
{
}


//------------------------------------------------------------------------------

void 
ProgressBarView::redraw( core::Surface& dst_surf,
                         const Rect2D& clip_rectangle
                       )
{
    ProgressBarElement::redraw( dst_surf, position(), clip_rectangle );
}


} // namspace gui