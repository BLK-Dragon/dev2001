#if !defined _PIXELSHADER_HPP_
#define _PIXELSHADER_HPP_
//==============================================================================
//
//  Pixel shader decls   
//
//==============================================================================
//
//  externals: 

    #include "Type.hpp"
    #include "Noise.hpp"

    using core::UByte;
    using core::UWord;
    using core::UDword;
    using vfx::PerlinNoise;
    using vfx::FastNoise;

namespace vfx
{
//==============================================================================
//
//  publics:


//==============================================================================
//
//  Perlin noise shader
//




class
PerlinNoiseShader  
{
  public:

                    PerlinNoiseShader( float grid_size,
                                       float x_origin, float y_origin,
                                       UDword _color,
                                       int intensity_base, int intensity_range
                                     );
                   ~PerlinNoiseShader();

    void            re_init( float grid_size,
                             float x_origin, float y_origin,
                             UDword _color,
                             int intensity_base, int intensity_range
                           );
    
    void            start_scanline( int x_offset, UDword* pixel );
    void            next_scanline();
    void            next_pixel();
    //void            shade( float fade );

  
  protected:
    PerlinNoise     noise;
    float           x0;                         // Perlin grid x-origin
    float           x, y;                       // Perlin grid coords
    float           delta;                      // Perlin grid size
    UDword*         color;                      // color table by intensity
    UDword*         pixel;                      // current pixel to shade
    int             index_base, index_range;    // color base & range (in table)
};


//------------------------------------------------------------------------------

inline void 
PerlinNoiseShader::start_scanline( int x_offset, UDword* _pixel )
{
    pixel  = _pixel;
    x      = x0 + x_offset*delta;
}


//------------------------------------------------------------------------------

inline void 
PerlinNoiseShader::next_scanline()
{
    y += delta;
}



//------------------------------------------------------------------------------

inline void 
PerlinNoiseShader::next_pixel()
{
    x += delta;
    ++pixel;
}


//==============================================================================




class
PerlinNoiseShaderSet
  : public PerlinNoiseShader  
{
    public:                    
                    
                    PerlinNoiseShaderSet( float grid_size,
                                          float x_origin, float y_origin,
                                          UDword _color,
                                          int intensity_base, 
                                          int intensity_range
                                        );

    void            shade( float fade_k );
    void            shade();

};


//------------------------------------------------------------------------------

inline void 
PerlinNoiseShaderSet::shade( float fade )
{
    float   f = noise.value_at( x, y );
    int     i = index_base + f*index_range;
    
    i *= fade;
    i  = (i<0)  ? 0  : (i>0xFF)  ? 0xFF  : i;

    *pixel = color[i];
}


//------------------------------------------------------------------------------

inline void 
PerlinNoiseShaderSet::shade()
{
    float   f = noise.value_at( x, y );
    int     i = index_base + f*index_range;
    
    i  = (i<0)  ? 0  : (i>0xFF)  ? 0xFF  : i;

    *pixel = color[i];
}


//==============================================================================




class
PerlinNoiseShaderAdd
  : public PerlinNoiseShader  
{
    public:                    
                    
                    PerlinNoiseShaderAdd( float grid_size,
                                          float x_origin, float y_origin,
                                          UDword _color,
                                          int intensity_base, 
                                          int intensity_range
                                        );

    void            shade( float fade_k );
    void            shade();

};


//------------------------------------------------------------------------------

inline void 
PerlinNoiseShaderAdd::shade( float fade )
{
    float   f = noise.value_at( x, y );
    int     i = index_base + f*index_range;
    
    i *= fade;
    i  = (i<0)  ? 0  : (i>0xFF)  ? 0xFF  : i;

    UByte*  src_color   = (UByte*)pixel;
    UDword  add_color   = color[i];

    __asm{
            push    EAX

            mov     EAX, src_color
            movd    MM0, dword ptr [EAX]
            paddusb MM0, qword ptr add_color
            movd    [EAX], MM0

            emms
            pop     EAX
         };
}


//------------------------------------------------------------------------------

inline void 
PerlinNoiseShaderAdd::shade()
{
    float   f = noise.value_at( x, y );
    int     i = index_base + f*index_range;
    
    i  = (i<0)  ? 0  : (i>0xFF)  ? 0xFF  : i;

    UByte*  src_color   = (UByte*)pixel;
    UDword  add_color   = color[i];

    __asm{
            push    EAX

            mov     EAX, src_color
            movd    MM0, dword ptr [EAX]
            paddusb MM0, qword ptr add_color
            movd    [EAX], MM0

            emms
            pop     EAX
         };
}



//==============================================================================
//
//  fast noise shader




class
FastNoiseShader  
{
  public:

                    FastNoiseShader( float grid_size,
                                     float x_origin, float y_origin,
                                     UDword _color,
                                     int intensity_base, int intensity_range
                                   );
                   ~FastNoiseShader();
    void            re_init( float grid_size,
                             float x_origin, float y_origin,
                             UDword _color,
                             int intensity_base, int intensity_range
                           );

    void            next_scanline( int x_offset, UDword* pixel );
    void            next_pixel();
    void            shade( float fade );

  
  private:
    FastNoise       noise;
    UDword          x0;                         // Perlin grid x-origin
    UDword          x, y;                       // Perlin grid coords
    UDword          delta;                      // Perlin grid size
    UDword*         color;                      // color table by intensity
    UDword*         pixel;                      // current pixel to shade
    int             index_base, index_range;    // color base & range (in table)
};


//------------------------------------------------------------------------------

inline void 
FastNoiseShader::next_scanline( int x_offset, UDword* _pixel )
{
    pixel  = _pixel;
    y     += delta;
    x      = x0 + ((UDword)x_offset)*delta;
}


//------------------------------------------------------------------------------

inline void 
FastNoiseShader::next_pixel()
{
    x += delta;
    ++pixel;
}


//------------------------------------------------------------------------------

inline void 
FastNoiseShader::shade( float fade )
{
    float   f = ( float( noise.value_at( x, y ) ) / 16384.0 ) - 1.0;
    int     i = index_base + f*index_range;
    
    i *= fade;
    i  = (i<0)  ? 0  : (i>0xFF)  ? 0xFF  : i;

    UByte*  src_color   = (UByte*)pixel;
    UDword  add_color   = color[i];

    __asm{
            push    EAX

            mov     EAX, src_color
            movd    MM0, dword ptr [EAX]
            paddusb MM0, qword ptr add_color
            movd    [EAX], MM0

            emms
            pop     EAX
         };
}

} // namespace vfx

#endif // _PIXELSHADER_HPP_