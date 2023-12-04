#if !defined _NOISELAYER_HPP_
#define _NOISELAYER_HPP_
//==============================================================================
//
//  Noise Layers   
//
//==============================================================================
//
//  externals: 


    #include <vector>
    
    #include "Vector.hpp"
    #include "Noise.hpp"


namespace vfx
{
//==============================================================================
//
//  publics:




class
NoiseLayer
{
  public:

    virtual        ~NoiseLayer() {};
    
    void            re_init( const Vector2f& org, 
                             const Vector2f& _u,
                             const Vector2f& _v
                           );
    
    void            advance_to_next_pixel();
    void            advance_to_next_line();

    Vector2f        cur_u() const { return u; };
    Vector2f        cur_v() const { return v; };
    void            change_direction( const Vector2f& _u, const Vector2f _v );
    void            scale_direction( float su, float sv );

  
  protected:
    
    float               noise_cur_val();
  
  
  private:

    Vector2f            origin;
    Vector2f            line_origin;
    Vector2f            cur_pos;
    Vector2f            u, v;
    vfx::PerlinNoise    noise;
};


//==============================================================================




class
GradNoiseLayer  
  : public NoiseLayer
{
  public:
                    GradNoiseLayer();
                   ~GradNoiseLayer();

    void            re_init( const Vector2f& org, 
                             const Vector2f& _u,
                             const Vector2f& _v,
                             core::UDword color1,
                             core::UDword color2,
                             int i_base,
                             int i_deviation
                           );
    void            re_init_polar( const Vector2f& org, 
                                   const Vector2f& _u,
                                   const Vector2f& _v,
                                   core::UDword color1,
                                   core::UDword color2,
                                   int i_base,
                                   int i_deviation
                                 );
    core::UDword    value_at_current_pixel();
  
  
  private:

    core::UDword*       color;
    int                 base;
    int                 deviation;
};


//==============================================================================




class
MultiColorNoiseLayer
  : public NoiseLayer  
{
  
  public:

                    MultiColorNoiseLayer();
                   ~MultiColorNoiseLayer();

    void            re_init( const Vector2f& org, 
                             const Vector2f& _u,
                             const Vector2f& _v,
                             const std::vector<core::UDword>& color,
                             int i_base,
                             int i_deviation
                           );
    core::UDword    value_at_current_pixel();
  
  
  private:

    int                 num_colors;
    core::UDword*       color;
    int                 base;
    int                 deviation;
};


//==============================================================================




class
HueNoiseLayer
  : public NoiseLayer  
{
  
  public:

                    HueNoiseLayer();
                   ~HueNoiseLayer();
    
    void            re_init( const Vector2f& org, 
                             const Vector2f& _u,
                             const Vector2f& _v,
                             float hue1, float hue2,
                             int i_base,
                             int i_deviation
                           );
    core::UDword    value_at_current_pixel();
  
  
  private:

    core::UDword        HSV32( float h, float s, float v );


    float               hue1, hue2;
    int                 base;
    int                 deviation;
};



//==============================================================================
//
// NoiseLayer
//

inline void 
NoiseLayer::re_init( const Vector2f& org, 
                     const Vector2f& _u,
                     const Vector2f& _v
                   )
{
    origin  = org;
    u       = _u;
    v       = _v;

    cur_pos     = org;
    line_origin = org;

    noise = vfx::PerlinNoise( origin[0], origin[1] );
}


//------------------------------------------------------------------------------

inline void
NoiseLayer::advance_to_next_pixel()
{
    cur_pos += u;
}


//------------------------------------------------------------------------------

inline void 
NoiseLayer::advance_to_next_line()
{
    line_origin += v;
    cur_pos     =  line_origin;
}


//------------------------------------------------------------------------------

inline void            
NoiseLayer::change_direction( const Vector2f& _u, const Vector2f _v )
{
    u = _u;
    v = _v;
}


//------------------------------------------------------------------------------

inline void            
NoiseLayer::scale_direction( float su, float sv )
{
    u *= su;
    v *= sv;    
}


//------------------------------------------------------------------------------

inline float               
NoiseLayer::noise_cur_val()
{
    return noise.value_at( cur_pos[0], cur_pos[1] );     
}


//==============================================================================
//
//  GradNoiseLayer
//

inline
GradNoiseLayer::GradNoiseLayer()
  : color(0)
{        
}

//------------------------------------------------------------------------------

inline
GradNoiseLayer::~GradNoiseLayer()
{
    if( color )
        delete[] color;
}


//------------------------------------------------------------------------------

inline void 
GradNoiseLayer::re_init( const Vector2f& org, 
                         const Vector2f& _u,
                         const Vector2f& _v,
                         core::UDword color1,
                         core::UDword color2,
                         int i_base,
                         int i_deviation
                       )
{
    NoiseLayer::re_init( org, _u, _v );
    
    if( !color )
        color = new core::UDword[256];

//    if( !color )
//        throw core::Error( "can't alloc mem for shader" );
    
    core::UDword    c1  = color1;
    core::UDword    c2  = color2;
    core::UByte*    _c1 = (core::UByte*)&c1;
    core::UByte*    _c2 = (core::UByte*)&c2;

    double  r   = double(_c1[0]);
    double  g   = double(_c1[1]);   
    double  b   = double(_c1[2]);
    double  dr  = ( double(_c2[0]) - double(_c1[0]) ) / 256.0;
    double  dg  = ( double(_c2[1]) - double(_c1[1]) ) / 256.0;
    double  db  = ( double(_c2[2]) - double(_c1[2]) ) / 256.0;

    for( int i=0; i<0xFF; i++ )
    {
        core::UByte* c = (core::UByte*)&color[i];

        c[0] = (core::UByte)r;
        c[1] = (core::UByte)g;
        c[2] = (core::UByte)b;
        c[3] = 0x00;

        r += dr;
        g += dg;
        b += db;
    }
    
    base        = i_base;
    deviation   = i_deviation;
}


//------------------------------------------------------------------------------

inline void            
GradNoiseLayer::re_init_polar( const Vector2f& org, 
                               const Vector2f& _u,
                               const Vector2f& _v,
                               core::UDword color1,
                               core::UDword color2,
                               int i_base,
                               int i_deviation
                             )
{
    // _u & _v are in polar coords e.g. { theta, r }
    // theta must be 0..90 degrees

    float   rad = 3.1415 / 180.0;
    
    Vector2f    u( _u[1] * sin(_u[0]*rad), _u[1] * sin(_u[0]*rad) );
    Vector2f    v( _v[1] * sin(_v[0]*rad), _v[1] * sin(_v[0]*rad) );

    GradNoiseLayer::re_init( org, u, v, color1, color2, i_base, i_deviation );
}


//------------------------------------------------------------------------------

inline core::UDword    
GradNoiseLayer::value_at_current_pixel()
{
    float   n = NoiseLayer::noise_cur_val();
    int     i = base + deviation*float(n);

    i = (i<0)  ? 0  : (i>255)  ? 255  : i;

    return color[i];
}



//==============================================================================
//
//  MultiColor NoiseLayer
//

inline
MultiColorNoiseLayer::MultiColorNoiseLayer()
  : color(0)
{
}


//------------------------------------------------------------------------------

inline
MultiColorNoiseLayer::~MultiColorNoiseLayer()
{
     
}


//------------------------------------------------------------------------------

inline void            
MultiColorNoiseLayer::re_init( const Vector2f& org, 
                               const Vector2f& _u,
                               const Vector2f& _v,
                               const std::vector<core::UDword>& color,
                               int i_base,
                               int i_deviation
                             )
{
    NoiseLayer::re_init( org, _u, _v );
    
    num_colors  = color.size();
    base        = i_base;
    deviation   = i_deviation;

    if( this->color )
        delete[] this->color;

    this->color = new core::UDword[256*color.size()];
    
    
    for( unsigned i=0; i<color.size(); i++ )
    {
        core::UDword    c1  = 0;
        core::UDword    c2  = color[i];
        core::UByte*    _c1 = (core::UByte*)&c1;
        core::UByte*    _c2 = (core::UByte*)&c2;

        double  r   = double(_c1[0]);
        double  g   = double(_c1[1]);
        double  b   = double(_c1[2]);
        double  dr  = ( double(_c2[0]) - double(_c1[0]) ) / 256.0;
        double  dg  = ( double(_c2[1]) - double(_c1[1]) ) / 256.0;
        double  db  = ( double(_c2[2]) - double(_c1[2]) ) / 256.0;

        for( int j=0; j<0xFF; j++ )
        {
            core::UByte* c = (core::UByte*)&this->color[256*i+j];

            c[0] = (core::UByte)r;
            c[1] = (core::UByte)g;
            c[2] = (core::UByte)b;
            c[3] = 0x00;

            r += dr;
            g += dg;
            b += db;
        }
    }

}


//------------------------------------------------------------------------------

inline core::UDword    
MultiColorNoiseLayer::value_at_current_pixel()
{
    float   n = NoiseLayer::noise_cur_val();
    int     i = base + deviation*float(n);

    i = (i<0)  ? 0  : (i>255)  ? 255  : i;

    return color[ ((vfx::rnd(num_colors))<<8) + i ];
}



//==============================================================================
//
//  HueNoiseLayer
//

inline
HueNoiseLayer::HueNoiseLayer()
{
}


//------------------------------------------------------------------------------

inline
HueNoiseLayer::~HueNoiseLayer()
{
}
    

//------------------------------------------------------------------------------

inline void            
HueNoiseLayer::re_init( const Vector2f& org, 
                        const Vector2f& _u,
                        const Vector2f& _v,
                        float hue1, float hue2,
                        int i_base,
                        int i_deviation
                      )
{
    NoiseLayer::re_init( org, _u, _v );

    this->hue1  = hue1;
    this->hue2  = hue2;
    base        = i_base;
    deviation   = i_deviation;
}


//------------------------------------------------------------------------------

inline core::UDword    
HueNoiseLayer::value_at_current_pixel()
{
    float   n = NoiseLayer::noise_cur_val();
//    int     i = base + deviation*float(n);
    int     i = 180 + 150*float(n);

    return HSV32( i, 1.0, 1.0 );
}


//------------------------------------------------------------------------------

inline core::UDword
HueNoiseLayer::HSV32( float H, float S, float V )
{
    if( H==360 ) H=0;
    H /= 60;

    int     i = int(H);
    float   f = H - i;
    float   p = V*( 1 - S );
    float   q = V*( 1 - S*f );
    float   t = V*( 1 - S*(1-f) );
    float   R,G,B;

    switch( i )
    {
        case 0 :
            R = V;
            G = t;
            B = p;
            break;

        case 1 :
            R = q;
            G = V;
            B = p;
            break;

        case 2 :
            R = p;
            G = V;
            B = t;
            break;

        case 3 :
            R = p;
            G = q;
            B = V;
            break;

        case 4 :
            R = t;
            G = p;
            B = V;
            break;

        case 5 :
            R = V;
            G = p;
            B = q;
            break;
    } // switch( i )

    return core::UDword(R*255)          |
           (core::UDword(G*255)<<8)     |
           (core::UDword(B*255)<<16);
}
} // namespace vfx     
#endif // _NOISELAYER_HPP_
