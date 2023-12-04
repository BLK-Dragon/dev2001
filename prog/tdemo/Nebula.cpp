//==============================================================================
//
//  Nebula rasterizer realization   
//
//==============================================================================
//
//  externals: 

    #include <math.h>
    #include <iostream>
    using std::clog;
    using std::flush;
    
    #include "Nebula.hpp"
    #include "..\..\sys\BitOps.hpp"


//==============================================================================
//
//  publics:

core::ParamSetManager*   NebulaRasterizer::paramset_manager  = 0;
std::string              NebulaRasterizer::paramset_name     = "nebula";



//------------------------------------------------------------------------------


NebulaRasterizer::NebulaRasterizer()
  : shader0(0),
    shader1(0),
    shader2(0)
{
}



//------------------------------------------------------------------------------

NebulaRasterizer::~NebulaRasterizer()
{
    if( shader0 )   delete shader0;
    if( shader1 )   delete shader1;
    if( shader2 )   delete shader2;
}


//------------------------------------------------------------------------------

void 
NebulaRasterizer::generate_scanline( core::UDword* scanline, int width )
{   
    core::UDword    buf[1024];

    vfx::block_fill( buf, width, 0x00000000 );

    
    float   left_x  = ( x_pos + l_x >= 0 )  ? x_pos + l_x  : 0;
    int     offset  = (int)left_x;
    int     right_x = x_pos+(int)r_x;

    if( right_x >= width )
        right_x = width-1;
    
    shader0->start_scanline( offset, buf + offset );
    shader1->start_scanline( offset, buf + offset );
    shader2->start_scanline( offset, buf + offset );
    
    if( x_pos + r_x <= 0 )
        return;

    core::UDword*   dst = scanline + offset;
    core::UDword*   src = buf + offset;
    
    for( int x = (int)left_x; x<right_x; x++ )
    {   
        if( x < 0 )
        {   
            shader0->next_pixel();
            shader1->next_pixel();
            shader2->next_pixel();
            ++src;
            ++dst;
            continue;
        }
        
        float   diff_x = 1-fabs( (l_x+(r_x-l_x)/2-(x-x_pos)) /
                                 float((r_x-l_x)/2) );
        float   diff_y = 1-fabs( (center_y-cur_y) / float(bound_height/2) );
        float   fade_k = fade( diff_x ) * fade( diff_y );
        
        //float   fade_k = 1.0;
        
        shader0->shade( fade_k );
        shader1->shade( fade_k );
        shader2->shade( fade_k );
        
        /*
        shader0->shade();
        shader1->shade();
        shader2->shade();
        */
        shader0->next_pixel();
        shader1->next_pixel();
        shader2->next_pixel();
        /*
        vfx::scale_color( src, fade_k );
        vfx::pixel_add( dst, *src );
        ++src;
        ++dst;
        */

        /*
        UDword  c = 0xFFFFFFFF;
        scale_color( &c, fade_k );
        scanline[x_pos+x] = c;
        */
        /*
        if( intensity_of(*(scanline+x)) > 26 )
        {
            // try to put glare
            if( ((float)vfx::rnd(1000))/1000.0 > 1-glare_density
                //glare.size() < max_glares 
              )
            {
                int r = 1 + vfx::rnd(glare_max_r);
                if( x > r  &&  r < width-r )
                {
                    glare.push_back( GlareStarRasterizer( Vector2i(x,cur_y+r),
                                                          r,
                                                          glare_color
                                                        )
                                   );
                }     
            }
            else if( vfx::rnd(255) > 252 )
            {
                core::UByte c = 40 + vfx::rnd(40);
                pixel_add( scanline+x, c|(c<<8)|(c<<16) );
            }
        } // if( intensity_of(*(scanline+x)) > 26 )
        */
    }
    

    /*
    for( int i=0; i<glare.size(); i++ )
        glare[i].generate_scanline( scanline, width );

    std::vector<GlareStarRasterizer>::iterator  g;

    g = glare.begin();
    while( g != glare.end() )
    {
        if( g->is_last_scanline( cur_y ) ) 
        {
            glare.erase( g ); 
            continue;
        }

        ++g;
    }
    */

    vfx::block_add( scanline, buf, width );
    //vfx::block_blend( scanline+offset, buf+offset, right_x-offset, 0.7 );
}


//------------------------------------------------------------------------------

void 
NebulaRasterizer::advance_to_next_scanline()
{
    //std::clog<<"cur_y = "<<cur_y<<" (top_y="<<top_y<<")\n"<<std::flush;
    if( --cur_y == top_y+1 )
    {    
        //std::clog<<"deactivated\n"<<std::flush;
        deactivate();
    }
    l_x += l_dx;
    r_x += r_dx;
    
    if( --l_sec_height == 0 )
        advance_l_index();
    
    if( --r_sec_height == 0 )
        advance_r_index();

    //for( int i=0; i<glare.size(); i++ )
    //    glare[i].advance_to_next_scanline();
    shader0->next_scanline();
    shader1->next_scanline();
    shader2->next_scanline();
}


//------------------------------------------------------------------------------

void 
NebulaRasterizer::activate( int x_pos, int n )
{   
    Rasterizer::activate( x_pos, n );
    core::ParamSet paramset = paramset_manager->paramset( paramset_name, n );
    
    this->x_pos = x_pos;

           
    generate_polygon( &vertex );
    /*
    vertex.clear();

    int t = 1+vfx::rnd(5);
    //int t = 5;
    if( t==1 )
    {
        vertex.push_back( Vector2i( 103 -147, (36 -174) ) );
        vertex.push_back( Vector2i( 109 -147, (69 -174) ) );
        vertex.push_back( Vector2i( 200 -147, (141 -174) ) );
        vertex.push_back( Vector2i( 229 -147, (190 -174) ) );
        vertex.push_back( Vector2i( 224 -147, (258 -174) ) );
        vertex.push_back( Vector2i( 123 -147, (309 -174) ) );
        vertex.push_back( Vector2i( 37 -147,  (282 -174) ) );
        vertex.push_back( Vector2i( 62 -147,  (203 -174) ) );
        vertex.push_back( Vector2i( 98 -147,  (162 -174) ) );
        vertex.push_back( Vector2i( 87 -147,  (112 -174) ) );
        vertex.push_back( Vector2i( 49 -147,  (66 -174) ) );
    }
    else if( t==2 )
    {
        vertex.push_back( Vector2i( 252 -190, (42 -32) ) );
        vertex.push_back( Vector2i( 287 -190, (125 -32) ) );
        vertex.push_back( Vector2i( 243 -190, (209 -32) ) );
        vertex.push_back( Vector2i( 109 -190, (196 -32) ) );
        vertex.push_back( Vector2i( 83 -190,  (139 -32) ) );
        vertex.push_back( Vector2i( 121 -190, (121 -32) ) );
        vertex.push_back( Vector2i( 160 -190, (62 -32) ) );
    }
    else if( t==3 )
    {
        vertex.push_back( Vector2i( 122 -115, (31 -151) ) );
        vertex.push_back( Vector2i( 178 -115, (41 -151) ) );
        vertex.push_back( Vector2i( 211 -115, (128 -151) ) );
        vertex.push_back( Vector2i( 202 -115, (306 -151) ) );
        vertex.push_back( Vector2i( 109 -115, (250 -151) ) );
        vertex.push_back( Vector2i( 83 -115,  (209 -151) ) );
        vertex.push_back( Vector2i( 25 -115,  (194 -151) ) );
        vertex.push_back( Vector2i( 5 -115,   (143 -151) ) );
        vertex.push_back( Vector2i( 94 -115,  (102 -151) ) );
    }
    else if( t==4 )
    {
        vertex.push_back( Vector2i( 92 -211,  (31 -113) ) );
        vertex.push_back( Vector2i( 188 -211, (48 -113) ) );
        vertex.push_back( Vector2i( 371 -211, (131 -113) ) );
        vertex.push_back( Vector2i( 386 -211, (168 -113) ) );
        vertex.push_back( Vector2i( 254 -211, (198 -113) ) );
        vertex.push_back( Vector2i( 80 -211,  (135 -113) ) );
        vertex.push_back( Vector2i( 31 -211,  (78 -113) ) );
    }
    else if( t==5 )
    {
        vertex.push_back( Vector2i( 250 -207, (5 -173) ) );
        vertex.push_back( Vector2i( 321 -207, (28 -173) ) );
        vertex.push_back( Vector2i( 241 -207, (108 -173) ) );
        vertex.push_back( Vector2i( 328 -207, (196 -173) ) );
        vertex.push_back( Vector2i( 252 -207, (338 -173) ) );
        vertex.push_back( Vector2i( 166 -207, (312 -173) ) );
        vertex.push_back( Vector2i( 186 -207, (252 -173) ) );
        vertex.push_back( Vector2i( 106 -207, (185 -173) ) );
        vertex.push_back( Vector2i( 106 -207, (88 -173) ) );
        vertex.push_back( Vector2i( 138 -207, (37 -173) ) );
    }
    */
    
    if( !shader0 )
    {
        shader0 = new vfx::PerlinNoiseShaderSet( (double)paramset["grid_size_0"],
                                                 (double)paramset["x_origin_0"],
                                                 (double)paramset["y_origin_0"],
                                                 (core::UDword)paramset["color_0"],
                                                 (int)paramset["intensity_base_0"],
                                                 (int)paramset["intensity_range_0"]
                                               );
    }
    else
    {
        shader0->re_init( (double)paramset["grid_size_0"],
                          (double)paramset["x_origin_0"],
                          (double)paramset["y_origin_0"],
                          (core::UDword)paramset["color_0"],
                          (int)paramset["intensity_base_0"],
                          (int)paramset["intensity_range_0"]
                        );
    }
    
    if( !shader1 )
    {
        shader1 = new vfx::PerlinNoiseShaderAdd( (double)paramset["grid_size_1"],
                                                 (double)paramset["x_origin_1"],
                                                 (double)paramset["y_origin_1"],
                                                 (core::UDword)paramset["color_1"],
                                                 (int)paramset["intensity_base_1"],
                                                 (int)paramset["intensity_range_1"]
                                               );
    }
    else
    {
        shader1->re_init( (double)paramset["grid_size_1"],
                          (double)paramset["x_origin_1"],
                          (double)paramset["y_origin_1"],
                          (core::UDword)paramset["color_1"],
                          (int)paramset["intensity_base_1"],
                          (int)paramset["intensity_range_1"]
                        );
    }
    
    if( !shader2 )
    {
        shader2 = new vfx::PerlinNoiseShaderAdd( (double)paramset["grid_size_2"],
                                                 (double)paramset["x_origin_2"],
                                                 (double)paramset["y_origin_2"],
                                                 (core::UDword)paramset["color_2"],
                                                 (int)paramset["intensity_base_2"],
                                                 (int)paramset["intensity_range_2"]
                                               );
    }
    else
    {
        shader2->re_init( (double)paramset["grid_size_2"],
                          (double)paramset["x_origin_2"],
                          (double)paramset["y_origin_2"],
                          (core::UDword)paramset["color_2"],
                          (int)paramset["intensity_base_2"],
                          (int)paramset["intensity_range_2"]
                        );
    }

    
    // vertices must be in clockwise order
    
    int i;
    int top     = 0;
    int bottom  = 0;
    
    for( i=0; i<vertex.size(); i++ )
    {
        if( vertex[i][1] < vertex[top][1] )
            top = i;
        
        if( vertex[i][1] > vertex[bottom][1] )
            bottom = i;
    }

    l_cur_index = r_cur_index = bottom;

    l_x = vertex[l_cur_index][0];
    r_x = vertex[r_cur_index][0];
    
    advance_l_index();
    advance_r_index();
    
    
    // find bounds
    
    int l_bound = vertex[0][0];
    int r_bound = vertex[0][0];
    int t_bound = vertex[0][1];
    int b_bound = vertex[0][1];
    
    for( i=0; i<vertex.size(); i++ )
    {
        if( vertex[i][0] < l_bound )
            l_bound = vertex[i][0];

        if( vertex[i][0] > r_bound )
            r_bound = vertex[i][0];

        if( vertex[i][1] < t_bound )
            t_bound = vertex[i][1];

        if( vertex[i][1] > b_bound )
            b_bound = vertex[i][1];
    }

    bound_width  = r_bound - l_bound;
    bound_height = b_bound - t_bound;
    center_x     = l_bound + bound_width/2;
    center_y     = t_bound + bound_height/2;

    cur_y = b_bound;
    top_y = t_bound;
}


//------------------------------------------------------------------------------

void
NebulaRasterizer::advance_l_index()
{
    int new_index;
    
    while( true )
    {     
        new_index      = ( l_cur_index < vertex.size()-1 )  
                         ? l_cur_index+1  
                         : 0;
        l_sec_height   = (int)vertex[l_cur_index][1] -
                         (int)vertex[new_index][1];

        if( l_sec_height > 0 )
        {
            l_dx        = ( vertex[new_index][0] - vertex[l_cur_index][0] ) /
                          l_sec_height;
            l_cur_index = new_index;
            break;
        }
    }
}


//------------------------------------------------------------------------------

void
NebulaRasterizer::advance_r_index()
{
    int new_index;
     
    while( true )
    {     
        new_index      = ( r_cur_index > 0 )  
                         ? r_cur_index - 1
                         : vertex.size() - 1;
        r_sec_height   = (int)vertex[r_cur_index][1] -
                         (int)vertex[new_index][1];

        if( r_sec_height > 0 )
        {
            r_dx        = ( vertex[new_index][0] - vertex[r_cur_index][0] ) / 
                          r_sec_height;
            r_cur_index = new_index;
            break;
        }
    }
}


//------------------------------------------------------------------------------

void
NebulaRasterizer::register_paramset( core::ParamSetManager* manager )
{
    paramset_manager = manager;
    manager->register_paramset( paramset_name, bind_members );
}


//------------------------------------------------------------------------------

void
NebulaRasterizer::bind_members( core::ParamSetPool* pool )
{
    pool->start_member_binding();

        pool->bind_double_member("grid_size_0");
        pool->bind_double_member("x_origin_0");
        pool->bind_double_member("y_origin_0");
        pool->bind_udword_member("color_0");
        pool->bind_int_member("intensity_base_0");
        pool->bind_int_member("intensity_range_0");

        pool->bind_double_member("grid_size_1");
        pool->bind_double_member("x_origin_1");
        pool->bind_double_member("y_origin_1");
        pool->bind_udword_member("color_1");
        pool->bind_int_member("intensity_base_1");
        pool->bind_int_member("intensity_range_1");

        pool->bind_double_member("grid_size_2");
        pool->bind_double_member("x_origin_2");
        pool->bind_double_member("y_origin_2");
        pool->bind_udword_member("color_2");
        pool->bind_int_member("intensity_base_2");
        pool->bind_int_member("intensity_range_2");

    pool->finish_member_binding();
}


//------------------------------------------------------------------------------

void                
NebulaRasterizer::generate_polygon( std::vector<Vector2i>* vec )
{
    int                     n_bones     = 2;
    int                     n_subdivs   = 8;
    std::vector<Vector2f>   bone;
    std::vector<Vector2i>   left_side;
    std::vector<Vector2i>   right_side;
    int                     i;

    Size2i  size = Size2i( 150+vfx::rnd(200), 150+vfx::rnd(200) );
    
    
    // generate 'sceleton'

    bone.push_back( Vector2f( -0.5*size[0] + vfx::rnd( size[0] ), 0 ) );

    //std::clog<<bone[0][0]<<","<<bone[0][1]<<"\n"<<std::flush;
    for( i=1; i<n_bones; i++ )
    {
        float x  = -0.5*size[0] + vfx::rnd( size[0] );
        float y  = bone[i-1][1] + i * ( size[1] / (n_bones-1) );

        bone.push_back( Vector2f(x,y) );
        //std::clog<<bone[i][0]<<","<<bone[i][1]<<"\n"<<std::flush;
    }


    // generate sides

    int     max_deviation   = (size[0] > size[1])  ? size[1]  : size[0];
    float   y               = bone[0][1];

    max_deviation /= 2;
    
    for( i=0; i<n_bones-1; i++ )
    {
        float   dx = ( bone[i+1][0] - bone[i][1] ) / n_subdivs;
        float   dy = ( bone[i+1][1] - bone[i][1] ) / n_subdivs;

        float   x  = bone[i][0];

        for( int j=0; j<n_subdivs; j++ )
        {
            float   xl = x - max_deviation/2 - vfx::rnd( max_deviation/2 );
            float   xr = x + max_deviation/2 + vfx::rnd( max_deviation/2 );

            left_side.push_back( Vector2i(xl,y) );
            right_side.push_back( Vector2i(xr,y+1) );
//            std::clog<<y<<":\t"<<xl<<"  "<<xr<<"\n"<<std::flush;
            
            
            y += dy;
            x += dx;
        }
    }


    // write resulting polygon
    
    vec->clear();
    for( i=0; i<right_side.size(); i++ )
        vec->push_back( right_side[i] );
    for( i=left_side.size()-1; i>=0; i-- )
        vec->push_back( left_side[i] );
    
    /*
    vec->clear();
    vec->push_back( Vector2i(0,-50) );
    vec->push_back( Vector2i(80,0) );
    vec->push_back( Vector2i(-70,20) );
    */
}







