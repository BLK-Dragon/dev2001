//==============================================================================
//
//
//
//==============================================================================
//
//  externals:

    #include "ClipView.hpp"


//==============================================================================
//
//  publics:
/*
ClipView::ClipView( Clip* _clip, core::Image& img )
  : clip_pic( img ),
    clip(_clip)
{
    int x  = cell_width*(abs(clip->position()[0])-1);
    int y  = t_origin + cell_height*clip->position()[1];

    if( clip->position()[0] < 0 )
        x = l_origin - x;
    else
        x += r_origin;
    
    bound_rect = Rect2D( x-cell_dw, y-cell_dw, x+cell_dw, y+cell_dw );    
    
    clip_pic.place( Vector2i( bound_rect.left(), bound_rect.top() ) );
}
*/
