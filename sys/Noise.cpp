//==============================================================================
//
//  Noise realization   
//
//==============================================================================
//
//  externals: 
    
    #include <math.h>
    #include <stdlib.h>

    #include "Noise.hpp"
    /*
    #include <iostream>
    #include <fstream>
    */



namespace vfx
{
//==============================================================================
//
//  publics:

    //std::ofstream Log("noise.log");
    
    bool        PerlinNoise::perlin_noise_precalced = false;
    int         PerlinNoise::P[0x100+0x100+2];
    NumType     PerlinNoise::G1[0x100+0x100+2];
    NumType     PerlinNoise::G2[0x100+0x100+2][2];

//------------------------------------------------------------------------------

void
PerlinNoise::init( core::UDword a, core::UDword b, core::UDword c )
{
    int i,j,k;
    
    for( i=0; i<0x100; i++ ) 
    {
        P[i] = i;

        G1[i] = (NumType)((rand() % (0x100 + 0x100)) - 0x100) / NumType(0x100);
        
        float   G2_i_0  = float((rand() % (0x100 + 0x100)) - 0x100) / float(0x100);
        float   G2_i_1  = float((rand() % (0x100 + 0x100)) - 0x100) / float(0x100);
        float   len     = sqrt( G2_i_0 * G2_i_0 + G2_i_1 * G2_i_1 );
        
        G2_i_0 /= len;
        G2_i_1 /= len;
        
        G2[i][0] = G2_i_0;//(NumType)((rand() % (0x100 + 0x100)) - 0x100) / NumType(0x100);
        G2[i][1] = G2_i_1;//(NumType)((rand() % (0x100 + 0x100)) - 0x100) / NumType(0x100);
        /*
        NumType len = NumType( float(sqrt( float(G2[i][0]) * float(G2[i][0]) + 
                                           float(G2[i][1]) * float(G2[i][1]) 
                                         )) 
                             );
        G2[i][0] /= len;
        G2[i][1] /= len;
        */
    }

    while( --i )
    {
        NumType k = P[i];

        P[i] = P[j = rnd(0x100) ];
        P[j] = k;
    }

    for( i=0; i<0x100+2; i++ )
    {
        P[0x100+i] = P[i];

        G1[0x100+i] = G1[i];
                      
        G2[0x100+i][0] = G2[i][0];
        G2[0x100+i][1] = G2[i][1];
    }
    
    perlin_noise_precalced = true;         
}


//------------------------------------------------------------------------------
/*
float
perlin_noise1D( float x )
{
    if( !perlin_noise_precalced ) 
    {
        init_perlin_noise();
    }

    float   t;
    int     bx0, bx1;
    float   rx0, rx1;
    float   u, v, sx;

    t   = x + 0x1000;
    bx0 = ((int)t) & 0xFF;
    bx1 = ((int)t+1) & 0xFF;
    rx0 = t - (int)t;
    rx1 = rx0 - 1;
    
    u   = rx0 * G1[ P[bx0] ];
    v   = rx1 * G1[ P[bx1] ];
    sx  = ( rx0 * rx0 * (3.0 - 2.0 * rx0) );

    return u + sx * ( v - u );
}
*/

//------------------------------------------------------------------------------

PerlinNoise::PerlinNoise( float x_gs, float y_gs,
                          core::UDword a, core::UDword b, core::UDword c
                        )
  : x_grid_start(x_gs/2),
    x_grid_size(x_gs),
    y_grid_start(y_gs/2),
    y_grid_size(y_gs)
{
    if( !perlin_noise_precalced )
    {
        init( a, b, c );
    }
}


//==============================================================================

int          FastNoise::num_objects = 0;
core::UWord* FastNoise::smoothed_noise;


FastNoise::FastNoise()
{
    if( num_objects++ == 0 )
        init();
}


//------------------------------------------------------------------------------


FastNoise::~FastNoise()
{
    if( --num_objects == 0 )
        shutdown();
}


//------------------------------------------------------------------------------

void 
FastNoise::init()
{
    core::UWord*    noise = new core::UWord[256*256];
    int             i;

    if( !noise )
        throw core::Error( core::Msg("can't alloc mem for fast-noise init") );

    smoothed_noise = new core::UWord[256*256];
    if( !smoothed_noise )
        throw core::Error( core::Msg("can't alloc mem for fast-noise init") );
    
    for( i=0; i<256*256; i++ )
        noise[i] = vfx::rnd(0xFFFF/2);

    for( int y=0+1; y<256-1; y++ )
    {
        for( int x=0+1; x<256-1; x++ )
        {
            core::UDword  corners = noise[ (y-1)*256 + x-1 ] +
                                    noise[ (y-1)*256 + x+1 ] +
                                    noise[ (y+1)*256 + x-1 ] +
                                    noise[ (y+1)*256 + x+1 ];

            core::UDword  sides =  noise[ (y-1)*256 + x   ] +
                                   noise[ (y  )*256 + x-1 ] +
                                   noise[ (y  )*256 + x+1 ] +
                                   noise[ (y+1)*256 + x   ];

            smoothed_noise[y*256+x] = corners/16 + sides/8 +
                                      noise[y*256+x]/4;
        }
    }

    delete[] noise;
}


//------------------------------------------------------------------------------

void 
FastNoise::shutdown()
{
    delete[] smoothed_noise;
}





} // namespace vfx