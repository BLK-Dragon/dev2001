//==============================================================================
//
//  Matrix realization
//
//==============================================================================
//
//  externals:

    #include "..\..\sys\Render2D.hpp"
    #include "..\..\sys\Fill.hpp"
    
    #include "Matrix.hpp"
    #include "Jumper.hpp"
    #include "Indicator.hpp"
    
    #include "ClipView.hpp"
    #include "CursorView.hpp"
    #include "IndicatorView.hpp"
    #include "JumperView.hpp"


//==============================================================================
//
//  publics:

Matrix::Matrix()
  : matrix_view(0)
{
}


//------------------------------------------------------------------------------

Matrix::~Matrix()
{
    clear(); 
}


//------------------------------------------------------------------------------

void                        
Matrix::re_create( int level, int rank )
{
    unsigned i;
    
    // delete existing configuration

    clear();
        
    
    // create new configuration
    
    max_node_x_coord    = 5;
    n_lines             = 5;
    
    matrix_view         = new core::Sprite( 2 * cell_width * max_node_x_coord +
                                            ( r_origin-l_origin),
                                            (num_lines() - 1) * cell_height
                                          );

    for( i=0; i<num_lines(); i++ )
    {
        add_clip( Vector2i(-1,i) );
        add_clip( Vector2i(1,i) );
        add_indicator( i );
    }

    
    add_clip( Vector2i(-5,1) );
    add_clip( Vector2i(-5,2) );
    add_clip( Vector2i(-3,2) );
    add_clip( Vector2i(-3,3) );
    add_clip( Vector2i(2,2) );
    add_clip( Vector2i(2,3) );
    add_clip( Vector2i(2,4) );
    add_clip( Vector2i(3,1) );
    add_clip( Vector2i(5,1) );
    add_clip( Vector2i(5,3) );
    add_jumper( Vector2i(-5,1), Vector2i(-1,1) );
    add_jumper( Vector2i(-5,2), Vector2i(-3,2) );
    add_jumper( Vector2i(-3,2), Vector2i(-3,3) );
    add_jumper( Vector2i(-3,3), Vector2i(-1,3) );
    add_jumper( Vector2i(1,2), Vector2i(2,2) );
    add_jumper( Vector2i(1,3), Vector2i(2,3) );
    add_jumper( Vector2i(1,4), Vector2i(2,4) );
    add_jumper( Vector2i(2,2), Vector2i(2,3) );
    add_jumper( Vector2i(2,3), Vector2i(2,4) );
    add_jumper( Vector2i(2,3), Vector2i(5,3) );
    add_jumper( Vector2i(3,1), Vector2i(5,1) );
}


//------------------------------------------------------------------------------

void                        
Matrix::bind_to_resourcepack( core::ResourcePack* rp )
{
    resource_pack = rp;
}


//------------------------------------------------------------------------------

void                        
Matrix::clear()
{
    for( unsigned i=0; i<clip.size(); i++ )
        delete clip[i];
    clip.clear();

    for( i=0; i<element.size(); i++ )
        delete element[i];
    element.clear();

    for( i=0; i<view.size(); i++ )
        delete view[i];
    view.clear();

    if( matrix_view )
        delete matrix_view;
}


//------------------------------------------------------------------------------

Clip*
Matrix::clip_at( const Vector2i& pos )
{
    for( unsigned i=0; i<clip.size(); i++ ) 
    {
        if( clip[i]->position() == pos )
            return clip[i]; 
    }

    return 0;
}


//------------------------------------------------------------------------------

void                        
Matrix::add_clip( const Vector2i& pos )
{
    Clip*   new_clip = new Clip(pos);    
    
    clip.push_back( new_clip );
    view.push_back( new ClipView( new_clip,
                                  resource_pack->image(".\\image\\clip.tga")
                                ) 
                  );
}


//------------------------------------------------------------------------------

void                        
Matrix::add_jumper( const Vector2i& in_pos, const Vector2i& out_pos )
{
    Clip*   in_clip  = clip_at( in_pos );
    Clip*   out_clip = clip_at( out_pos );

    if( !in_clip  ||  !out_clip )
    {
        throw MatrixError( 
                         );
    }

    if(    (in_pos[0] < 0  &&  out_pos[0] > 0) 
        || (in_pos[0] > 0  &&  out_pos[0] < 0)
        || in_pos[0] == 0
        || out_pos[0] == 0
      )
    {
        throw MatrixError( 
                         );
    }

    Jumper* jumper = new Jumper;

    element.push_back( jumper );
    jumper->connect_input( in_clip );
    jumper->connect_output( out_clip );
    out_clip->connect( jumper );
    
    core::Image&    img     = ( in_pos[1] == out_pos[1] )
                              ? resource_pack->image(".\\image\\h_jumper.tga")
                              : resource_pack->image(".\\image\\v_jumper.tga");
    
    
    view.push_back( new JumperView( jumper, img ) );
}


//------------------------------------------------------------------------------

void                        
Matrix::add_indicator( int line )
{
    Clip*   l_clip  = clip_at( Vector2i( -1, line) ); 
    Clip*   r_clip  = clip_at( Vector2i( 1, line) ); 

    if( !l_clip  ||  !r_clip )
    {
        throw MatrixError( DBG((core::Msg("can't add indicator, no clips (%08X %08X)",
                                          l_clip, r_clip
                                        ))
                              )
                         ); 
    }

    Indicator*  indicator = new Indicator;

    indicator->connect_input( 0, l_clip );
    indicator->connect_input( 1, r_clip );
    
    element.push_back( indicator );
    view.push_back( new IndicatorView( indicator,
                                       resource_pack->image(".\\image\\indicator.tga")
                                     ) 
                  );
}


//------------------------------------------------------------------------------

Cursor*                     
Matrix::add_cursor( const Vector2i& pos )
{
    Cursor*     new_cursor = new Cursor( this, pos );
    
    cursor.push_back( new_cursor );
    view.push_back( new CursorView( new_cursor, 
                                    resource_pack->image(".\\image\\cursor.tga")
                                  ) 
                  ); 
    
    return new_cursor;
}


//------------------------------------------------------------------------------

void                        
Matrix::save_background( core::Surface& surf )
{
    matrix_view->save_background( surf );
}


//------------------------------------------------------------------------------

void                        
Matrix::redraw( core::Surface& surf )
{
    unsigned            i;
    vfx::Render2D       render( surf );
    core::UDword        grid_color      = vfx::rgb(20,20,20);
    vfx::SolidFill1D    grid_fill( grid_color );

    for( i=0; i<max_node_x_coord; i++ )
    {
         render.v_line( l_origin - i*cell_width, t_origin, 
                        (num_lines()-1)*cell_height, grid_fill );
         render.v_line( r_origin + i*cell_width, t_origin,
                        (num_lines()-1)*cell_height, grid_fill );
    }

    for( i=0; i<num_lines(); i++ )
    {
        render.h_line( l_origin - (max_node_x_coord-1)*cell_width, 
                       t_origin + i*cell_height,  
                       (max_node_x_coord-1)*cell_width, grid_fill
                     ); 
        render.h_line( r_origin, 
                       t_origin + i*cell_height,
                       (max_node_x_coord-1)*cell_width, grid_fill
                     ); 
    }

    for( i=0; i<view.size(); i++ )
        view[i]->save_background( surf );
    
    
    // draw other things first than clips 

    std::vector<ClipView*>  clip_view;
    std::vector<View*>      other_view;

    for( i=0; i<view.size(); i++ )
    {
        ClipView*   cv = dynamic_cast<ClipView*>(view[i]);

        if( cv )
            clip_view.push_back( cv );
        else
            other_view.push_back( view[i] );
    }

    for( i=0; i<other_view.size(); i++ )
        other_view[i]->redraw( surf );
    for( i=0; i<clip_view.size(); i++ )
        clip_view[i]->redraw( surf );
}


//------------------------------------------------------------------------------

void                        
Matrix::restore_background( core::Surface& surf )
{
    matrix_view->restore_background( surf );

    for( unsigned i=0; i<view.size(); i++ )
        view[i]->restore_background( surf );
}
