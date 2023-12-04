//==============================================================================
//
//  Star rasterizer realization   
//
//==============================================================================
//
//  externals: 

    #include <math.h>
    #include "..\..\sys\Random.hpp"
    
    #include "Star.hpp"
    #include "..\..\sys\BitOps.hpp"



//==============================================================================
//
//  statics:

core::ParamSetManager*   StarRasterizer::paramset_manager  = 0;
std::string              StarRasterizer::paramset_name      = "star";


//==============================================================================
//
//  publics:

StarRasterizer::StarRasterizer()
{                                     
}


//------------------------------------------------------------------------------

void
StarRasterizer::activate( int x_pos, int n )
{   
    Rasterizer::activate( x_pos, n );
    cur_paramset    = paramset_manager->paramset( paramset_name, n );
    
    x_center        = x_pos;   
    star_radius     = cur_paramset["radius"];
    dy              = -star_radius;
    color           = cur_paramset["color"];
}


//------------------------------------------------------------------------------

StarRasterizer::~StarRasterizer()
{
}


//------------------------------------------------------------------------------

void 
StarRasterizer::generate_scanline( core::UDword* scanline, int width )
{
    //std::clog << "Star line\n" << std::flush;
    
    for( int x=x_center - star_radius; 
         x < x_center + star_radius; 
         x++ 
       )
    {   
        if( x<0  ||  x>=width )
            continue;
        
        float   dx = x_center - x;
        float   d = sqrt( dx*dx + dy*dy );

        if( d > star_radius )
            continue;

        d /= star_radius;

        float   a = exp( -d*d*d*d*5 );
        //float   a = (1-d)*(1-d);

        a = (a>1.0)  ? 1.0  : (a<0)  ? 0.0  : a;

        core::UDword    c = color;

        vfx::scale_color( &c, a );
        vfx::pixel_add( (scanline+x), c );
    }
}


//------------------------------------------------------------------------------

void 
StarRasterizer::advance_to_next_scanline()
{
    if( ++dy > star_radius )
    {    
        deactivate();
        //std::clog << "glare deactivated\n" << std::flush;
    }
}


//------------------------------------------------------------------------------

void     
StarRasterizer::register_paramset( core::ParamSetManager* manager )
{
    paramset_manager = manager;
    paramset_manager->register_paramset( paramset_name, bind_members );
}


//------------------------------------------------------------------------------

void     
StarRasterizer::bind_members( core::ParamSetPool* pool )
{
    pool->start_member_binding();

        pool->bind_string_member( "tag" );
        pool->bind_int_member( "radius" );
        pool->bind_udword_member( "color" );

    pool->finish_member_binding();
}














