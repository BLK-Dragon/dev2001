//==============================================================================
//
//  GlareStar rasterizer realization   
//
//==============================================================================
//
//  externals: 

    #include <math.h>
    #include "..\..\sys\Random.hpp"
    
    #include "GlareStar.hpp"
    #include "..\..\sys\BitOps.hpp"



//==============================================================================
//
//  statics:

core::ParamSetManager*  GlareStarRasterizer::paramset_manager  = 0;
std::string             GlareStarRasterizer::paramset_name     = "glarestar";
bool                    GlareStarRasterizer::first_creation = true;
core::UByte             GlareStarRasterizer::ray[512];
//float                   GlareStarRasterizer::sqrt_value[50000];



//==============================================================================
//
//  publics:

GlareStarRasterizer::GlareStarRasterizer()
{                                     
}


//------------------------------------------------------------------------------

void
GlareStarRasterizer::activate( int x_pos, int n )
{
    Rasterizer::activate( x_pos, n );
    cur_paramset = paramset_manager->paramset( paramset_name, n );
    
    x_center        = x_pos;   
    star_radius     = (int)cur_paramset["radius"];
    dy              = -star_radius;
    ray_start       = vfx::rnd(254);
    color           = (core::UDword)cur_paramset["color"];//0x00FFFFFF;

    if( first_creation )
    {
        for( int i=0; i<512; i++ )
            ray[i] = vfx::rnd(255);

//        for( i=0; i<10000; i++ )
//            sqrt_value[i] = sqrt(i);

        first_creation = false;
    }
}


//------------------------------------------------------------------------------

GlareStarRasterizer::~GlareStarRasterizer()
{
}


//------------------------------------------------------------------------------

void 
GlareStarRasterizer::generate_scanline( core::UDword* scanline, int width )
{
    //std::clog << "glarestar line\n" << std::flush;
    
    for( int x=x_center - star_radius; 
         x < x_center + star_radius; 
         x++ 
       )
    {   
        if( x<0  ||  x>=width )
            continue;
        
        int     dx = x_center - x;
        float   d = /*sqrt_value[ dx*dx + dy*dy ];*/sqrt( dx*dx + 4*(dy*dy) );

        if( d > star_radius )
            continue;

        d /= star_radius;

        //float   a = (1 - d*d) - d*linear;//exp( -d*d*gauss );
        float   a = exp( -d*d*5 );

        a = (a>1.0)  ? 1.0  : (a<0)  ? 0.0  : a;

        core::UDword    c = color;
        vfx::scale_color( &c, a );
        
        
        // add rays
        
        float           v = ( atan2(dx, dy) + 3.1415 ) * 64.0/3.1415;
        int             i = floor( v );
        core::UDword    ray_color = 0xFFFFFF;
       
        v   = ( ray[ray_start+i] + (ray[ray_start+i+1]-ray[ray_start+i] ) * 
              (v-i) ) / 255.0;
        v   = pow( v, 2.0 ) / ( 1 + d );
        v   = (v<0)  ? 0.0  : (v>1)  ? 1.0  : v;
        v   = (d>0.1)  ? v*0.5*(1 - d*d)  : 1.0;

        vfx::scale_color( &ray_color, v );
        
        vfx::pixel_add( &c, ray_color );
        if( vfx::intensity_of(c) > vfx::intensity_of(color)+20 )
            c = color;
        vfx::pixel_add( (scanline+x), c );

        /*
        vfx::pixel_add( (scanline+x), c );
        //if( vfx::intensity_of(ray_color) < vfx::intensity_of(color) )
        //    vfx::pixel_add( (scanline+x), ray_color );
        vfx::pixel_add( (scanline+x), ray_color );
        */
    }
}


//------------------------------------------------------------------------------

void 
GlareStarRasterizer::advance_to_next_scanline()
{
    if( ++dy > star_radius )
    {    
        deactivate();
        //std::clog << "glare deactivated\n" << std::flush;
    }
}


//------------------------------------------------------------------------------

void         
GlareStarRasterizer::register_paramset( core::ParamSetManager* manager )
{
    paramset_manager = manager;
    manager->register_paramset( paramset_name, bind_members );
}


//------------------------------------------------------------------------------

void                 
GlareStarRasterizer::bind_members( core::ParamSetPool* pool )
{
    pool->start_member_binding();

        pool->bind_int_member( "radius" );
        pool->bind_udword_member( "color" );

    pool->finish_member_binding();
}














