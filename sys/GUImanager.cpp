//==============================================================================
//
//  GUI manager realization   
//
//==============================================================================
//
//  externals: 

    #include "GUImanager.hpp"
    #include "WidgetView.hpp"


namespace gui
{
//==============================================================================
//
//  publics:


//------------------------------------------------------------------------------

GUImanager::GUImanager()
{
}


//------------------------------------------------------------------------------

GUImanager::~GUImanager()
{
}


//------------------------------------------------------------------------------

void 
GUImanager::register_widget_view( WidgetView* wv )
{
    widget_view.push_back( wv );
}


//------------------------------------------------------------------------------

void 
GUImanager::redraw( core::Surface& dst_surf, 
                    const Rect2D& clip_rectangle
                  )
{
    for( int i=0; i<widget_view.size(); i++ )
        widget_view[i]->redraw( dst_surf, clip_rectangle );         
}


//------------------------------------------------------------------------------
                    
void            
GUImanager::save_background( core::Surface& src_surf,
                             const Rect2D& clip_rectangle
                           )
{
    for( int i=0; i<widget_view.size(); i++ )
        widget_view[i]->save_background( src_surf, clip_rectangle );
}


//------------------------------------------------------------------------------

void            
GUImanager::restore_background( core::Surface& dst_surf,
                                const Vector2i& offset,
                                const Rect2D& clip_rectangle 
                              )
{
    for( int i=0; i<widget_view.size(); i++ )
        widget_view[i]->restore_background( dst_surf, offset, clip_rectangle );         
}


} // namspace gui