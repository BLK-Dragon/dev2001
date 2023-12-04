//==============================================================================
//
//  Shader realizations
//
//==============================================================================
//
//  externals: 

    #include "PixelShader.hpp"

namespace vfx
{
//==============================================================================
//
//  Pelin noise shader (base)


PerlinNoiseShader::PerlinNoiseShader( float grid_size,
                                      float x_origin, float y_origin,
                                      UDword _color,
                                      int intensity_base, int intensity_range
                                    )
  : color(0)
{
    re_init( grid_size, x_origin, y_origin,
             _color,
             intensity_base, intensity_range
           );
}


//------------------------------------------------------------------------------

void
PerlinNoiseShader::re_init( float grid_size,
                            float x_origin, float y_origin,
                            UDword _color,
                            int intensity_base, int intensity_range
                          )
{
    x           = x_origin;
    x0          = x_origin;
    y           = y_origin;
    delta       = grid_size;
    index_base  = intensity_base;
    index_range = intensity_range;
    
    if( !color )
        color = new UDword[256];

    if( !color )
        throw core::Error( "can't alloc mem for shader" );
    
    for( int i=0; i<256; i++ )
    {
        UByte*    _c = (UByte*)&_color;
        UByte*    c  = (UByte*)&color[i];

        c[0] = UByte( (i/255.0) * (float)_c[0] );
        c[1] = UByte( (i/255.0) * (float)_c[1] );
        c[2] = UByte( (i/255.0) * (float)_c[2] );
        c[3] = 0x00;
    }
     
}


//------------------------------------------------------------------------------

PerlinNoiseShader::~PerlinNoiseShader()
{
    delete color;     
}


//------------------------------------------------------------------------------

PerlinNoiseShaderSet::PerlinNoiseShaderSet( float grid_size,
                                            float x_origin, float y_origin,
                                            UDword _color,
                                            int intensity_base, 
                                            int intensity_range
                                          )
  : PerlinNoiseShader( grid_size, x_origin, y_origin, _color, 
                       intensity_base, intensity_range
                     )
{
}


//------------------------------------------------------------------------------

PerlinNoiseShaderAdd::PerlinNoiseShaderAdd( float grid_size,
                                            float x_origin, float y_origin,
                                            UDword _color,
                                            int intensity_base, 
                                            int intensity_range
                                          )
  : PerlinNoiseShader( grid_size, x_origin, y_origin, _color, 
                       intensity_base, intensity_range
                     )
{
}






//==============================================================================
//
//  fast noise shader


FastNoiseShader::FastNoiseShader( float grid_size,
                                  float x_origin, float y_origin,
                                  UDword _color,
                                  int intensity_base, int intensity_range
                                )
  : color(0)
{
    re_init( grid_size, x_origin, y_origin, _color, 
             intensity_base, intensity_range
           );     
}


//------------------------------------------------------------------------------

void
FastNoiseShader::re_init( float grid_size,
                          float x_origin, float y_origin,
                          UDword _color,
                          int intensity_base, int intensity_range
                        )
{
    x           = x_origin*32768.0;
    x0          = x_origin*32768.0;
    y           = y_origin*32768.0;
    delta       = grid_size*32768.0;
    index_base  = intensity_base;
    index_range = intensity_range;
    
    if( !color )
        color = new UDword[256];

    //> check pointer for emptiness

    for( int i=0; i<256; i++ )
    {
        UByte*    _c = (UByte*)&_color;
        UByte*    c  = (UByte*)&color[i];

        c[0] = UByte( (i/255.0) * (float)_c[0] );
        c[1] = UByte( (i/255.0) * (float)_c[1] );
        c[2] = UByte( (i/255.0) * (float)_c[2] );
        c[3] = 0x00;
    }
}


//------------------------------------------------------------------------------

FastNoiseShader::~FastNoiseShader()
{
    delete color;     
}

} // namespace vfx
