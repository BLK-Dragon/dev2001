//==============================================================================
//
//  StarField rasterizer realization   
//
//==============================================================================
//
//  externals: 

    #include "StarField.hpp"

    #include <math.h>
    #include "..\..\sys\BitOps.hpp"
    #include "..\..\sys\Random.hpp"


//==============================================================================
//
//  publics:

//core::ParamSetPool* StarFieldRasterizer::paramset_pool = 0;
core::ParamSetManager*   StarFieldRasterizer::paramset_manager  = 0;
std::string              StarFieldRasterizer::paramset_name     = "starfield";



//------------------------------------------------------------------------------


StarFieldRasterizer::StarFieldRasterizer()
  : first_warp(true)
{
    for( unsigned i=0; i<max_noise_layers; i++ )
        noise_layer[i] = 0;
}


//------------------------------------------------------------------------------


StarFieldRasterizer::~StarFieldRasterizer()
{
    for( unsigned i=0; i<max_noise_layers; i++ )
    {
        if( noise_layer[i] )
            delete noise_layer[i];
    }
}


//------------------------------------------------------------------------------

void 
StarFieldRasterizer::generate_scanline( core::UDword* scanline, int width )
{
    core::UDword*   pixel   = scanline;
    
    for( int x=0; x<width; x++ )
    {
        *pixel = noise_layer[0]->value_at_current_pixel();
        
        for( unsigned i=1; i<num_layers; i++ )
            vfx::pixel_add( pixel, noise_layer[i]->value_at_current_pixel() );

        if( float(vfx::rnd(1000))/1000.0 > 1.0 - star_density )
        {    
            core::UDword    color = star_color[ vfx::rnd(star_color.size()) ];
            
            float           i     = star_base_intensity + 
                                    (vfx::rnd(100)/100.0) *
                                    (star_max_intensity - star_base_intensity);
                                    
            vfx::scale_color( &color, i );
            
            vfx::pixel_add( pixel, color );
            //*pixel = color;
        }
        
        if( --bigstar_line == 0 )
        {
            if( double(vfx::rnd(1000))/1000.0 > 1.0 - bigstar_density )
            {
                for( int i=0; i<star.size(); i++ )
                {
                    if( !star[i].is_active() )
                    {
                        int     v = vfx::rnd( star_variant.size() );

                        float   s = bigstar_base_intensity +
                                    (vfx::rnd(100)/100.0) *
                                    (bigstar_max_intensity - bigstar_base_intensity);

                        star[i].activate( x+vfx::rnd(25)-50, star_variant[v] );
                        star[i].scale_intensity( s );
                        break;
                    }
                }
            }
            
            bigstar_line = bigstar_lines_to_skip;     
        }
        
        for( i=0; i<num_layers; i++ )
            noise_layer[i]->advance_to_next_pixel();

        
        // warp U-vector of noise layer #0
        
        for( i=1; i<num_xwarp_points; i++ )
        {
            if( x == xwarp_info[i].x_pos )
            {
                float   ux = xwarp_info[i].scale * noise_layer[0]->cur_u()[0];
                float   uy = noise_layer[0]->cur_u()[1];

                noise_layer[0]->change_direction( Vector2f(ux,uy),
                                                  noise_layer[0]->cur_v()
                                                );
                break;
            }
        }
        
        ++pixel;
    }
    
    
    for( int i=0; i<star.size(); i++ )
    {
        if( star[i].is_active() )
             star[i].generate_scanline( scanline, width );
    }

    if( first_warp )
    {
        // init warp-info

        xwarp_info[0].x_pos         = 0;
        xwarp_info[0].change_ttw    = 0;
        for( unsigned i=1; i<num_xwarp_points; i++ )
        {
            int     segment = width/num_xwarp_points;
            
            xwarp_info[i].x_pos = i * segment + ( -segment/5 + vfx::rnd(segment/5) );

            if( xwarp_info[i].x_pos < 0 )       xwarp_info[i].x_pos = 0;
            if( xwarp_info[i].x_pos > width-1 ) xwarp_info[i].x_pos = width-1;
            
            xwarp_info[i].change_ttw = 0;
        }
        
        first_warp = false; 
    }
        
    for( i=1; i<num_xwarp_points; i++ )
    {
        if( --xwarp_info[i].change_ttw <= 0 )
        {
            int     new_x   = xwarp_info[i].x_pos + ( -50 + vfx::rnd(50) );
            int     d       = 50 + vfx::rnd(200);

            if( new_x < 0 )         new_x = 0;
            if( new_x > width-1 )   new_x = width-1;

            if( new_x <= xwarp_info[i-1].x_pos )
                new_x = xwarp_info[i].x_pos + 2;
            if( i < num_xwarp_points-1  &&  new_x >= xwarp_info[i+1].x_pos )
                new_x = xwarp_info[i+1].x_pos - 2;

            xwarp_info[i].dx         = float(new_x - xwarp_info[i].x_pos) / d;
            xwarp_info[i].scale      = 0.05 + 5.0 * vfx::rnd(1000)/1000.0;
            xwarp_info[i].change_ttw = d;
        }
        else
        {
            xwarp_info[i].x_pos += xwarp_info[i].dx;
        }
    } // for( i=1; i<num_xwarp_points; i++ )
}


//------------------------------------------------------------------------------

void 
StarFieldRasterizer::advance_to_next_scanline()
{
    unsigned i;

    for( i=0; i<num_layers; i++ )
        noise_layer[i]->advance_to_next_line(); 
    
    
    for( i=0; i<star.size(); i++ )
    {
        if( star[i].is_active() )
             star[i].advance_to_next_scanline();
    } 
    
    
    // add some chaos to noise layer generation
    
    if( --layer0_change_ttw <= 0 )
    {
        int     distance    = 50 + vfx::rnd(100);
        float   min_uy      = 0.0;
        float   max_uy      = 2 * (double)cur_paramset["u_x_0"];
        float   duy_range   = 0.4 * (double)cur_paramset["u_x_0"];
        float   min_vx      = 0.0;
        float   max_vx      = 2 * (double)cur_paramset["v_y_0"];
        float   dvx_range   = 0.4 * (double)cur_paramset["v_y_0"];

        float   u_y         = -duy_range + 2*duy_range * vfx::rnd(1000)/1000.0;
        float   v_x         = -dvx_range + 2*dvx_range * vfx::rnd(1000)/1000.0;

        u_y = (u_y < min_uy)  ? min_uy  : (u_y > max_uy)  ? max_uy  : u_y;
        v_x = (v_x < min_vx)  ? min_vx  : (v_x > max_vx)  ? max_vx  : v_x;

        layer0_du[1]        = ( u_y - layer0_u[1] ) / distance;
        layer0_dv[0]        = ( v_x - layer0_v[0] ) / distance;
        layer0_change_ttw   = distance;
    }
    else
    {
        layer0_u += layer0_du;
        layer0_v += layer0_dv;
        noise_layer[0]->change_direction( layer0_u, layer0_v );
    }
    
    /*
    if( --layer0_scale_ttw <= 0 )
    {
        int     distance    = 50 + vfx::rnd(100);
        float   min_s       = 0.7;
        float   max_s       = 2.0;
        float   range_s     = 0.25;

        float   s;

        s           = -range_s + 2*range_s * vfx::rnd(1000)/1000.0;
        s           = (s<min_s)  ? min_s  : (s>max_s)  ? max_s  : s;
        layer0_dsu  = ( s - layer0_su ) / distance;

        s           = -range_s + 2*range_s * vfx::rnd(1000)/1000.0;
        s           = (s<min_s)  ? min_s  : (s>max_s)  ? max_s  : s;
        layer0_dsv  = ( s - layer0_sv ) / distance;

        layer0_scale_ttw = distance;
    }
    else
    {
        layer0_su += layer0_dsu;
        layer0_sv += layer0_dsv;

        Vector2f    u = Vector2f( layer0_su * (double)cur_paramset["u_x_0"],
                                  layer0_su * (double)cur_paramset["u_y_0"]
                                );
        Vector2f    v = Vector2f( layer0_sv * (double)cur_paramset["v_x_0"],
                                  layer0_sv * (double)cur_paramset["v_y_0"]
                                );
        noise_layer[0]->change_direction( u, v );
    }
    */
}


//------------------------------------------------------------------------------

inline float
interp( float x )
{
    return 3*x*x - 2*x*x*x; 
}


//------------------------------------------------------------------------------

void 
StarFieldRasterizer::activate( int x_pos, int n )
{
    Rasterizer::activate( x_pos, n );
    cur_paramset = paramset_manager->paramset( paramset_name, n );
    
    star.clear();
    for( int i=0; i<(int)cur_paramset["bigstar_count"]; i++ )
        star.push_back( StarRasterizer() );

    star_variant.clear();
    for( i=0; i<paramset_manager->paramset_count( "star" ); i++ )
    {
        core::ParamSet  p = paramset_manager->paramset( "star", i );

        if( (std::string)p["tag"] == paramset_name )
            star_variant.push_back(i);
    }

    num_layers              = cur_paramset["num_layers"];
    num_layers              = (num_layers<1)
                              ? 1
                              : (num_layers>max_noise_layers-1)  
                                ? num_layers>max_noise_layers  
                                : num_layers;

    std::clog << "starfield> num_layers = " << num_layers << "\n" << std::flush;
    
    for( i=0; i<num_layers; i++ )
    {
        char            field[120];
        Vector2f        org, u, v;
        core::UDword    color1, color2;
        int             base, deviation;

        sprintf( field, "org_x_%i", i );
        org[0] = (double)cur_paramset[field];
        sprintf( field, "org_y_%i", i );
        org[1] = (double)cur_paramset[field];

        sprintf( field, "u_x_%i", i );
        u[0] = (double)cur_paramset[field];
        sprintf( field, "u_y_%i", i );
        u[1] = (double)cur_paramset[field];

        sprintf( field, "v_x_%i", i );
        v[0] = (double)cur_paramset[field];
        sprintf( field, "v_y_%i", i );
        v[1] = (double)cur_paramset[field];

        sprintf( field, "color1_%i", i );
        color1 = (core::UDword)cur_paramset[field];
        sprintf( field, "color2_%i", i );
        color2 = (core::UDword)cur_paramset[field];

        sprintf( field, "intensity_base_%i", i );
        base = (int)cur_paramset[field];
        sprintf( field, "intensity_deviation_%i", i );
        deviation = (int)cur_paramset[field];

        if( !noise_layer[i] )
            noise_layer[i] = new vfx::GradNoiseLayer;
        noise_layer[i]->re_init( org, u, v, color1, color2, base, deviation );
    }
    
    if( core::UDword(cur_paramset["star_color0"]) != 0x00000000 )
        star_color.push_back( cur_paramset["star_color0"] );
    if( core::UDword(cur_paramset["star_color1"]) != 0x00000000 )
        star_color.push_back( cur_paramset["star_color1"] );
    if( core::UDword(cur_paramset["star_color2"]) != 0x00000000 )
        star_color.push_back( cur_paramset["star_color2"] );
    if( core::UDword(cur_paramset["star_color3"]) != 0x00000000 )
        star_color.push_back( cur_paramset["star_color3"] );
    if( core::UDword(cur_paramset["star_color4"]) != 0x00000000 )
        star_color.push_back( cur_paramset["star_color4"] );
    
    star_density            = cur_paramset["star_density"];
    star_base_intensity     = cur_paramset["star_base_intensity"];
    star_max_intensity      = cur_paramset["star_max_intensity"];
    
    //bigstar_threshold       = cur_paramset["bigstar_threshold"];
    bigstar_lines_to_skip   = cur_paramset["bigstar_lines_to_skip"];
    bigstar_density         = cur_paramset["bigstar_density"];
    bigstar_base_intensity  = cur_paramset["bigstar_base_intensity"];
    bigstar_max_intensity   = cur_paramset["bigstar_max_intensity"];

    bigstar_line            = bigstar_lines_to_skip;
    layer0_u                = Vector2f( (double)cur_paramset["u_x_0"], 
                                        (double)cur_paramset["u_y_0"] 
                                      );
    layer0_v                = Vector2f( (double)cur_paramset["v_x_0"], 
                                        (double)cur_paramset["v_y_0"] 
                                      );
    layer0_change_ttw       = 0;
    
    layer0_su               = 1.0;
    layer0_sv               = 1.0;
    layer0_scale_ttw        = 0;
}


//------------------------------------------------------------------------------

void         
StarFieldRasterizer::register_paramset( core::ParamSetManager* manager )
{
    paramset_manager = manager;
    manager->register_paramset( paramset_name, bind_members );

    StarRasterizer::register_paramset( manager );
}


//------------------------------------------------------------------------------

void                 
StarFieldRasterizer::bind_members( core::ParamSetPool* pool )
{
    pool->start_member_binding();

        pool->bind_double_member("org_x_0");
        pool->bind_double_member("org_y_0");
        pool->bind_double_member("u_x_0");
        pool->bind_double_member("u_y_0");
        pool->bind_double_member("v_x_0");
        pool->bind_double_member("v_y_0");
        pool->bind_udword_member("color1_0");
        pool->bind_udword_member("color2_0");
        pool->bind_int_member("intensity_base_0");
        pool->bind_int_member("intensity_deviation_0");

        pool->bind_double_member("org_x_1");
        pool->bind_double_member("org_y_1");
        pool->bind_double_member("u_x_1");
        pool->bind_double_member("u_y_1");
        pool->bind_double_member("v_x_1");
        pool->bind_double_member("v_y_1");
        pool->bind_udword_member("color1_1");
        pool->bind_udword_member("color2_1");
        pool->bind_int_member("intensity_base_1");
        pool->bind_int_member("intensity_deviation_1");

        pool->bind_double_member("org_x_2");
        pool->bind_double_member("org_y_2");
        pool->bind_double_member("u_x_2");
        pool->bind_double_member("u_y_2");
        pool->bind_double_member("v_x_2");
        pool->bind_double_member("v_y_2");
        pool->bind_udword_member("color1_2");
        pool->bind_udword_member("color2_2");
        pool->bind_int_member("intensity_base_2");
        pool->bind_int_member("intensity_deviation_2");

        pool->bind_double_member("org_x_3");
        pool->bind_double_member("org_y_3");
        pool->bind_double_member("u_x_3");
        pool->bind_double_member("u_y_3");
        pool->bind_double_member("v_x_3");
        pool->bind_double_member("v_y_3");
        pool->bind_udword_member("color1_3");
        pool->bind_udword_member("color2_3");
        pool->bind_int_member("intensity_base_3");
        pool->bind_int_member("intensity_deviation_3");

        pool->bind_double_member("org_x_4");
        pool->bind_double_member("org_y_4");
        pool->bind_double_member("u_x_4");
        pool->bind_double_member("u_y_4");
        pool->bind_double_member("v_x_4");
        pool->bind_double_member("v_y_4");
        pool->bind_udword_member("color1_4");
        pool->bind_udword_member("color2_4");
        pool->bind_int_member("intensity_base_4");
        pool->bind_int_member("intensity_deviation_4");

        pool->bind_double_member("org_x_5");
        pool->bind_double_member("org_y_5");
        pool->bind_double_member("u_x_5");
        pool->bind_double_member("u_y_5");
        pool->bind_double_member("v_x_5");
        pool->bind_double_member("v_y_5");
        pool->bind_udword_member("color1_5");
        pool->bind_udword_member("color2_5");
        pool->bind_int_member("intensity_base_5");
        pool->bind_int_member("intensity_deviation_5");

        pool->bind_double_member("org_x_6");
        pool->bind_double_member("org_y_6");
        pool->bind_double_member("u_x_6");
        pool->bind_double_member("u_y_6");
        pool->bind_double_member("v_x_6");
        pool->bind_double_member("v_y_6");
        pool->bind_udword_member("color1_6");
        pool->bind_udword_member("color2_6");
        pool->bind_int_member("intensity_base_6");
        pool->bind_int_member("intensity_deviation_6");

        pool->bind_double_member("org_x_7");
        pool->bind_double_member("org_y_7");
        pool->bind_double_member("u_x_7");
        pool->bind_double_member("u_y_7");
        pool->bind_double_member("v_x_7");
        pool->bind_double_member("v_y_7");
        pool->bind_udword_member("color1_7");
        pool->bind_udword_member("color2_7");
        pool->bind_int_member("intensity_base_7");
        pool->bind_int_member("intensity_deviation_7");

        pool->bind_double_member("star_density");
        pool->bind_double_member("star_base_intensity");
        pool->bind_double_member("star_max_intensity");
        
        pool->bind_int_member("bigstar_count");
        pool->bind_int_member("bigstar_lines_to_skip");
        pool->bind_double_member("bigstar_threshold");
        pool->bind_double_member("bigstar_density");
        pool->bind_double_member("bigstar_base_intensity");
        pool->bind_double_member("bigstar_max_intensity");

        pool->bind_udword_member("star_color0");
        pool->bind_udword_member("star_color1");
        pool->bind_udword_member("star_color2");
        pool->bind_udword_member("star_color3");
        pool->bind_udword_member("star_color4");

        pool->bind_int_member("num_layers");

    pool->finish_member_binding();
}









