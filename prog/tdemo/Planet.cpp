//==============================================================================
//
//  Planet rasterizer realization   
//
//==============================================================================
//
//  externals: 

    #include <math.h>
    
    #include "..\..\sys\Random.hpp"
    
    #include "Planet.hpp"
    #include "..\..\sys\BitOps.hpp"


//==============================================================================
//
//  publics:

core::ParamSetPool  PlanetRasterizer::paramset_pool( "planet" );



//------------------------------------------------------------------------------

PlanetRasterizer::PlanetRasterizer( RasterizerPool* rp )
  : Rasterizer( &PlanetRasterizer::paramset_pool, rp )
{
    paramset_pool.start_member_binding();
    paramset_pool.bind_int_member( "radius" );
    paramset_pool.bind_double_member( "grid_size_0" );
    paramset_pool.bind_udword_member( "color_0" );
    paramset_pool.bind_int_member( "base_intensity_0" );
    paramset_pool.bind_int_member( "intensity_range_0" );
    paramset_pool.bind_double_member( "grid_size_1" );
    paramset_pool.bind_udword_member( "color_1" );
    paramset_pool.bind_int_member( "base_intensity_1" );
    paramset_pool.bind_int_member( "intensity_range_1" );
    paramset_pool.finish_member_binding();
}


//------------------------------------------------------------------------------

PlanetRasterizer::~PlanetRasterizer()
{
    delete shader0;
    delete shader1;
}


//------------------------------------------------------------------------------

void 
PlanetRasterizer::generate_scanline( core::UDword* scanline, int width )
{
    float   cz  = -10;
    
    shader0->next_scanline( x_pos-radius, scanline + x_pos-radius );
    shader1->next_scanline( x_pos-radius, scanline + x_pos-radius );
    
    for( int x = x_pos-radius; x<x_pos+radius; x++ )
    {
        if( x<0  ||  x>width-1 )
            continue;
        
        float   xs  = x - x_pos;
        float   ys  = y_pos;
        float   D   = radius*radius - xs*xs - ys*ys;
        float   k   = 0.0;

        if( D >= 0 )
        {
            //scanline[x] = 0xFFFFFF;
            
            float   cos_phi = (1.0/radius) * 
                              ( lx*xs + ly*ys + lz * ( sqrt(D) ) );
            //std::clog << "cos_phi = " << cos_phi << "\n" << std::flush;

            k += (float)vfx::rnd(40) / 100.0;
            k  = (cos_phi>0.0)  ? cos_phi  : 0.0;
            k  = (cos_phi>1.0)  ? 1.0  : cos_phi;
            
        }

        shader0->shade( k );
        shader0->next_pixel();
        shader1->shade( k );
        shader1->next_pixel();
        /*
        float   r   = sqrt( xs*xs + ys*ys );
        float   dr  = r - radius;
        
        if( dr > 0  &&  fabs(dr) < 1.0 )
        {
            float phi = (180.0/3.14) * atan2( ys, xs );

            if( phi>45 && phi<90 )
            {
                scanline[x+1] = scanline[x];
                scale_color( scanline+x, dr );
                scale_color( scanline+x+1, 1.0-dr );
            }
        }    
        */
        
        //scale( &color, k );
        
        //std::clog << ".";
        //scanline[x] = 0xFFFFFF;
    }
    //std::clog << "\n";
    
}


//------------------------------------------------------------------------------

void 
PlanetRasterizer::advance_to_next_scanline()
{
    //std::clog << y_pos << "\n";
    if( ++y_pos > radius )
        deactivate();
}


//------------------------------------------------------------------------------

void 
PlanetRasterizer::activate( int x_pos, int n )
{
    Rasterizer::activate( x_pos, n );


    // create shaders
    
    shader0 = new PerlinNoiseShader( (double)paramset["grid_size_0"], 0,0, 
                                     (core::UDword)paramset["color_0"],
                                     (int)paramset["base_intensity_0"],
                                     (int)paramset["intensity_range_0"]
                                   );
    
    shader1 = new PerlinNoiseShader( (double)paramset["grid_size_1"], 0,0, 
                                     (core::UDword)paramset["color_1"],
                                     (int)paramset["base_intensity_1"],
                                     (int)paramset["intensity_range_1"]
                                   );


    
    // compute light
    
    lx  = 1;//0.1 + vfx::rnd(50) / 100.0;
    ly  = 0;//0.1 + vfx::rnd(50) / 100.0;;
    lz  = 0;
    
    float   ay  = vfx::rnd(40) * 3.1415/180.0;
    float   az  = vfx::rnd(80) * 3.1415/180.0;

    float   _lx, _ly, _lz;

    
    // rotate about Z
    
    _lx = lx * cos(az) - ly * sin(az);
    _ly = lx * sin(az) + ly * cos(az);
    lx  = _lx;
    ly  = _ly;
    

    // rotate about Y
    
    _lx = lx * cos(ay) + lz * sin(ay);
    _lz = -lx * sin(ay) + lz * cos(ay);
    lx  = _lx;
    lz  = _lz;
    
    
    // normalize l-vector
    
    float   len = sqrt( lx*lx + ly*ly + lz*lz );
    
    lx /= len;
    ly /= len;
    lz /= len;

    radius      = paramset["radius"];
    this->x_pos = x_pos;
    y_pos       = -radius;
}
















