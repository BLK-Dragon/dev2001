//==============================================================================
//
//  Jumper View eralization
//
//==============================================================================
//
//  externals:

    #include "JumperView.hpp"
//    #include "Clip.hpp"


//==============================================================================
//
//  publics:
/*
JumperView::JumperView( Jumper* _jumper, core::Image& img )
  : jumper_pic( img ),
    jumper(_jumper)
{
    Vector2i    in_pos  = jumper->inputs()[0]->position();
    Vector2i    out_pos = jumper->outputs()[0]->position();
    
    int     lx  = cell_width*(abs(in_pos[0])-1);
    int     rx  = cell_width*(abs(out_pos[0])-1);
    int     ty  = t_origin + cell_height*in_pos[1];
    int     by  = t_origin + cell_height*out_pos[1];

    if( in_pos[0] < 0 )
    {
        lx  = l_origin - lx;
        rx  = l_origin - rx; 
    }
    else
    {
        lx += r_origin;
        rx += r_origin; 
    }

    if( rx < lx )
    {
        int     tmp = lx;

        lx = rx;
        rx = tmp;
    }

    if( by < ty )
    {
        int     tmp = by;

        by = ty;
        ty = tmp;
    }

    Rect2D      bound       = ( in_pos[1] == out_pos[1] )
                              ? Rect2D( lx, ty-cell_dw, rx, by+cell_dw )
                              : Rect2D( lx-cell_dw, ty, lx+cell_dw, by );
    
    this->bound_rect = bound;
    jumper_pic.place( Vector2i( bound.left(), bound.top() ) );
}
*/
