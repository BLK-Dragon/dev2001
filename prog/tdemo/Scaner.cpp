//==============================================================================
//
//  Scaner realization   
//
//==============================================================================
//
//  externals: 

    #include "Scaner.hpp"

    #include "..\..\sys\BitOps.hpp"


//==============================================================================
//
//  publics:

ScanerView::ScanerView( gui::GUImanager* gm, 
                        const Rect2D& controlled_zone,
                        const Rect2D& bound 
                      )
  : WidgetView( gm, bound ),
    controlled_zone( controlled_zone ),
    def_bk_color( vfx::rgb(0x30,0x30,0x30) ),
    def_obj_color( vfx::rgb(0xFF,0x00,0x00) ),
    def_vis_color( vfx::rgb(0x40,0x40,0x40) )
{
}


//------------------------------------------------------------------------------

ScanerView::~ScanerView()
{
}


//------------------------------------------------------------------------------

void 
ScanerView::redraw( core::Surface& dst_surf,
                    const Rect2D& clip_rectangle
                  )
{
    int     top     = Sprite::bound_rect().top();
    int     left    = Sprite::bound_rect().left();
    int     width   = Sprite::bound_rect().width() / 4;
    int     height  = Sprite::bound_rect().height() / 4;

    int     v_top    = top +
                       ( ( visible_zone.top() - controlled_zone.top() ) /
                         (float)controlled_zone.height()
                       ) * Sprite::bound_rect().height();
    int     v_bottom = top +
                       ( ( visible_zone.bottom() - controlled_zone.top() ) /
                         (float)controlled_zone.height()
                       ) * Sprite::bound_rect().height();
    int     v_left   = left +
                       ( ( visible_zone.left() - controlled_zone.left() ) /
                         (float)controlled_zone.width()
                       ) * Sprite::bound_rect().width();
    int     v_right  = left +
                       ( ( visible_zone.right() - controlled_zone.left() ) /
                         (float)controlled_zone.width()
                       ) * Sprite::bound_rect().width();


    core::UByte*    c = (core::UByte*)&def_bk_color;

    for( int y=top; y<top+height*4; y+=4 )
    {
        for( int x=left; x<left+width*4; x+=4 ) 
        {
            if( x>=v_left  &&  x<v_right  &&  y>v_top  && y<v_bottom )
                c = (core::UByte*)&def_vis_color;
            else
                c = (core::UByte*)&def_bk_color;
            
            dst_surf.set_pixel( x,   y,   c[0], c[1], c[2] );
            dst_surf.set_pixel( x+2, y,   c[0], c[1], c[2] );
            dst_surf.set_pixel( x,   y+2, c[0], c[1], c[2] );
            dst_surf.set_pixel( x+2, y+2, c[0], c[1], c[2] );
        }
    }

    
    for( int i=0; i<obj_pos.size(); i++ )
    {
        float xk = ( obj_pos[i][0] - controlled_zone.left() ) / 
                   (float)controlled_zone.width();
        float yk = ( obj_pos[i][1] - controlled_zone.top() ) / 
                   (float)controlled_zone.height();

        if( xk<0.0  ||  xk>1.0  ||  yk<0.0  ||  yk>1.0 )
            continue;

        int x = Sprite::bound_rect().left() + 4*xk * (width-4);
        int y = Sprite::bound_rect().top() + 4*yk * (height-4);

        c = (core::UByte*)&obj_color[i];
        
        dst_surf.set_pixel( x,   y,   c[0], c[1], c[2] );
        dst_surf.set_pixel( x+2, y,   c[0], c[1], c[2] );
        dst_surf.set_pixel( x,   y+2, c[0], c[1], c[2] );
        dst_surf.set_pixel( x+2, y+2, c[0], c[1], c[2] );
    }
}

