#if !defined _NOISE_HPP_
#define _NOISE_HPP_
//==============================================================================
//
//  Noise decls   
//
//==============================================================================
//
//  externals: 

    #include "Random.hpp"
    #include "Exception.hpp"
    #include "FixedPoint.hpp"

namespace vfx
{
//==============================================================================
//
//  publics:

typedef     core::Fixed<16,16>  NumType;
//typedef     float   NumType;




class
PerlinNoise  
{
  public:

                        PerlinNoise( float x_gs = 0.040, 
                                     float y_gs = 0.040,
                                     core::UDword a = 0x11111111, 
                                     core::UDword b = 0x11111111, 
                                     core::UDword c = 0x11111111
                                   );

    float               value_at( float x, float y ) const;

    static void         init( core::UDword a = 0x11111111, 
                              core::UDword b = 0x11111111, 
                              core::UDword c = 0x11111111
                            );

  private:

    
    NumType             x_grid_size;
    NumType             y_grid_size;
    NumType             x_grid_start;
    NumType             y_grid_start;
    
    static bool         perlin_noise_precalced; 
    static int          P[0x100+0x100+2];
    static NumType      G1[0x100+0x100+2];
    static NumType      G2[0x100+0x100+2][2];
};


//==============================================================================




class
FastNoise  
{
  public:

                        FastNoise();
                       ~FastNoise();

    core::UWord         value_at( core::UDword x, core::UDword y );

  
  private:

    static int          num_objects;
    static void         init();
    static void         shutdown();

    static core::UWord  interpolation( core::UWord a, core::UWord b, 
                                       core::UWord x 
                                     );


    static core::UWord* smoothed_noise;
};


//------------------------------------------------------------------------------
//
//  Perlin noise functions
//
//------------------------------------------------------------------------------

inline void
float2int( float f, int* i )
{   /*     
    __asm{
            fld     f
            mov     EDX, i
            frndint
            fistp   dword ptr [EDX]
         }
      */   
         *i = (int)f;
}

inline float
PerlinNoise::value_at( float x, float y ) const
{
    //x = x_grid_start + x*x_grid_size;
    //y = y_grid_start + y*y_grid_size;
    
    NumType t;
    int     int_t;
    int     bx0, bx1, by0, by1, b00, b01, b10, b11;
    NumType rx0, rx1, ry0, ry1;
    NumType u, v;
    NumType a, b;
    
    t       = NumType(x) + NumType(0x100);
    int_t   = int( t );

    bx0 = (int_t) & 0xFF;
    bx1 = (int_t+1) & 0xFF;
    rx0 = t - NumType(int_t);
    rx1 = rx0 - NumType(1);
    
    t       = NumType(y) + NumType(0x100);
    int_t   = int( t );
    
    by0 = (int_t) & 0xFF;
    by1 = (int_t+1) & 0xFF;
    ry0 = t - NumType(int_t);
    ry1 = ry0 - NumType(1);

    int i = P[ bx0 ];
    int j = P[ bx1 ];

    b00 = P[ i + by0 ];
    b10 = P[ j + by0 ];
    b01 = P[ i + by1 ];
    b11 = P[ j + by1 ];

    NumType sx = ( rx0 * rx0 * ( NumType(3.0f) - NumType(2.0f) * rx0) );
    NumType sy = ( ry0 * ry0 * ( NumType(3.0f) - NumType(2.0f) * ry0) );

    u = rx0 * G2[b00][0] + ry0 * G2[b00][1];
    v = rx1 * G2[b10][0] + ry0 * G2[b10][1];
    a = u + sx  *( v - u );

    u = rx0 * G2[b01][0] + ry1 * G2[b01][1];
    v = rx1 * G2[b11][0] + ry1 * G2[b11][1];
    b = u + sx * ( v - u );

    return float( a + sy * ( b - a ) );
}


//------------------------------------------------------------------------------
//
//  Fast noise functions
//
//------------------------------------------------------------------------------

inline core::UWord  
FastNoise::interpolation( core::UWord a, core::UWord b, core::UWord x )
{
    return a + ((x*(b-a))>>16);
}


//------------------------------------------------------------------------------

inline core::UWord         
FastNoise::value_at( core::UDword x, core::UDword y )
{
    // x & y are fixed point 16.16 coordinates

    core::UWord int_x   = ((core::UWord)( x >> 16 )) & 0xFF;
    core::UWord frac_x  = (core::UWord)x;
    core::UWord int_y   = ((core::UWord)( y >> 16 )) & 0xFF;
    core::UWord frac_y  = (core::UWord)y;

    
    core::UWord v1      = smoothed_noise[((int_y  )<<8) + int_x  ];
    core::UWord v2      = smoothed_noise[((int_y  )<<8) + int_x+1];
    core::UWord v3      = smoothed_noise[((int_y+1)<<8) + int_x  ];
    core::UWord v4      = smoothed_noise[((int_y+1)<<8) + int_x+1];
    core::UDword i1    = interpolation( v1, v2, frac_x );
    core::UDword i2    = interpolation( v3, v4, frac_x );
    
    return interpolation( i1, i2, frac_y );
}






} // namespace vfx
#endif // _NOISE_HPP_