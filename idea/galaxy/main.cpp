#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <ddraw.h>
#include <math.h>

#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
using std::clog;
using std::flush;


#include "..\..\sys\Win32Error.hpp"
#include "..\..\sys\Noise.hpp"
#include "..\..\tool\Toster\\Toster.hpp"

#include "..\..\sys\Rect.hpp"
#include "..\..\sys\Vector.hpp"

#include "PixelShader.hpp"
#include "ColorInterpolator.hpp"


//------------------------------------------------------------------------------
//
//  globals:

HWND            Window;
HWND            TosterWin;
LPDIRECTDRAW    lpDD;
LPDIRECTDRAW2   lpDD2;
HMODULE         Toster;

LRESULT CALLBACK
WndProc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam );

bool            TosterCreated = false;


const int       Width       = 640;
const int       Height      = 480;
DWORD*          Mem         = NULL;
HDC             Back_buffer;
RECT            client_rect;

bool            Drawed      = false;

int             rnd_RedAdd;
int             Yoffset[Height];

float           grid_size[3]        = { 0.04, 0.04, 0.04 };

float           grid_start[3]       = { 0.001, 0.001, 0.001 };

int             intensity_base[3]   = { 128, 128, 128 };

int             intensity_range[3]  = { 128, 128, 128 };

core::UDword    GalaxyColor[3]      = { 0xADA590, 0xADA590, 0xADA590 };
core::UDword    GalaxyColor01       = 0xFFFFFF;
core::UDword    GlowColor           = 0x5555AA;

int             FlareSize           = 10;

int             galaxy_threshold    = 150;
int             nebula_threshold    = 150;
float           star_probability    = 0.1;

enum
{
    Noise       = 0,
    InvNoise    = 1,
    ModInvNoise = 2
};

int             GalaxyType[3]       = { 0, 0, 0 };

core::UDword    Glow_Color[256];
core::UDword    Galaxy_Color0[256];
core::UDword    Galaxy_Color1[256];
core::UDword    Galaxy_Color2[256];


std::ofstream   Log("main.log");


static core::UByte  BackgroundBuffer[Width*Height];
//static core::UDword GalaxyColor[256];

//------------------------------------------------------------------------------

void
clear_background_buffer()
{
    ZeroMemory( BackgroundBuffer, Width*Height );
}


//------------------------------------------------------------------------------

void
gradient( core::UDword* dst,
          core::UDword color1, core::UDword color2, int n
        )
{
    float   r   = ( (color1>>0) & 0xFF );
    float   g   = ( (color1>>8) & 0xFF );
    float   b   = ( (color1>>16) & 0xFF );

    float   dr  = ( float( (color2>>0) & 0xFF ) - r ) / n;
    float   dg  = ( float( (color2>>8) & 0xFF ) - g ) / n;
    float   db  = ( float( (color2>>16) & 0xFF ) - b ) / n;

    for( int i=0; i<n; i++ )
    {
        *dst++ = core::UDword(r)        | 
                 (core::UDword(g)<<8)   | 
                 (core::UDword(b)<<16);
        
        r += dr;
        g += dg;
        b += db;
    }
}

//------------------------------------------------------------------------------

void
precalc()
{
    for( int i=0; i<Height; i++ )
        Yoffset[i] = Width*i;
    
    gradient( Galaxy_Color0, 0, GalaxyColor[0], 128 );
    gradient( Galaxy_Color0+127, GalaxyColor[0], GalaxyColor01, 128 );

    gradient( Galaxy_Color1, 0, GalaxyColor[1], 256 );
    gradient( Galaxy_Color2, 0, GalaxyColor[2], 256 );
    
    gradient( Glow_Color, 0, GlowColor, 256 );
}


//------------------------------------------------------------------------------
/*
inline void
pixel( int x, int y, core::UByte color )
{
    if( x<0 || x>Width-1 || y<0 || y>Height-1 )
        return;

    *( BackgroundBuffer + Yoffset[y] + x ) = color;
}
*/

//------------------------------------------------------------------------------

inline void
pixel( int x, int y, core::UDword color )
{
    if( x<0 || x>Width-1 || y<0 || y>Height-1 )
        return;

    *( Mem + Yoffset[y] + x ) = color;
}


//------------------------------------------------------------------------------

inline core::UDword
color_at( int x, int y )
{
    if( x<0 || x>Width-1 || y<0 || y>Height-1 )
        return 0;

    return *( Mem + Yoffset[y] + x );
}


//------------------------------------------------------------------------------

inline int
intensity_at( int x, int y )
{
    if( x<0 || x>Width-1 || y<0 || y>Height-1 )
        return 0;

    core::UByte*    color   = (core::UByte*)( Mem + Yoffset[y] + x );

    return ( color[0] + color[1] + color[2] ) / 3;
}


//------------------------------------------------------------------------------

inline void
scale( core::UDword* src_color, float s )
{
    core::UByte*    c   = (core::UByte*)src_color;
    core::UDword    res_color;
    core::UByte*    cl  = (core::UByte*)&res_color;

    cl[0] = core::UByte( s * float(c[0]) );
    cl[1] = core::UByte( s * float(c[1]) );
    cl[2] = core::UByte( s * float(c[2]) );
    cl[3] = 0x00;

    *src_color = res_color;
}


//------------------------------------------------------------------------------

inline void
pixel_add( int x ,int y, core::UDword c )
{
    core::UByte*    src_color   = (core::UByte*)( Mem + Yoffset[y] + x );
    core::UByte*    glow_color  = (core::UByte*)&c;
    core::UByte     color[4];

    __asm{
            push    EAX

            mov     EAX, src_color
            movd    MM0, dword ptr [EAX]
            paddusb MM0, qword ptr c
            movd    [EAX], MM0

            emms
            pop     EAX
         };

    /*
    color[0] = ( color_ptr[0] + glow_color[0] < 0xFF )
               ? color_ptr[0] + glow_color[0]
               : 0xFF;

    color[1] = ( color_ptr[1] + glow_color[1] < 0xFF )
               ? color_ptr[1] + glow_color[1]
               : 0xFF;

    color[2] = ( color_ptr[2] + glow_color[2] < 0xFF )
               ? color_ptr[2] + glow_color[2]
               : 0xFF;

    color[3] = 0x00;

    *( (core::UDword*)color_ptr ) = *( (core::UDword*)color );
    */
}


//------------------------------------------------------------------------------

inline void
add_glow_5x5( int x, int y )
{
    const int   glow_00 = 0.5*167;
    const int   glow_10 = 0.5*107;
    const int   glow_11 = 0.5*68;
    const int   glow_20 = 0.5*36;
    const int   glow_21 = 0.5*22;
    const int   glow_30 = 0.5*8;

    pixel_add( x,   y,   Glow_Color[glow_00] );

    pixel_add( x,   y-1, Glow_Color[glow_10] );
    pixel_add( x-1, y,   Glow_Color[glow_10] );
    pixel_add( x+1, y,   Glow_Color[glow_10] );
    pixel_add( x,   y+1, Glow_Color[glow_10] );

    pixel_add( x-1, y-1, Glow_Color[glow_11] );
    pixel_add( x+1, y-1, Glow_Color[glow_11] );
    pixel_add( x-1, y+1, Glow_Color[glow_11] );
    pixel_add( x+1, y+1, Glow_Color[glow_11] );

    pixel_add( x,   y-2, Glow_Color[glow_20] );
    pixel_add( x-2, y,   Glow_Color[glow_20] );
    pixel_add( x+2, y,   Glow_Color[glow_20] );
    pixel_add( x,   y+2, Glow_Color[glow_20] );

    pixel_add( x-1, y-2, Glow_Color[glow_21] );
    pixel_add( x+1, y-2, Glow_Color[glow_21] );
    pixel_add( x-2, y-1, Glow_Color[glow_21] );
    pixel_add( x+2, y-1, Glow_Color[glow_21] );
    pixel_add( x-1, y+2, Glow_Color[glow_21] );
    pixel_add( x+1, y+2, Glow_Color[glow_21] );
    pixel_add( x-2, y+1, Glow_Color[glow_21] );
    pixel_add( x+2, y+1, Glow_Color[glow_21] );

    pixel_add( x-2, y-2, Glow_Color[glow_30] );
    pixel_add( x+2, y-2, Glow_Color[glow_30] );
    pixel_add( x-2, y-2, Glow_Color[glow_30] );
    pixel_add( x+2, y-2, Glow_Color[glow_30] );
}


//------------------------------------------------------------------------------

inline void
add_glow_3x3( int x, int y )
{
    const int   glow_00 = 0.5*181;
    const int   glow_10 = 0.5*113;
    const int   glow_11 = 0.5*37;
    const int   glow_20 = 0.5*23;

    pixel_add( x,   y,   Glow_Color[glow_00] );

    pixel_add( x,   y-1, Glow_Color[glow_10] );
    pixel_add( x-1, y,   Glow_Color[glow_10] );
    pixel_add( x+1, y,   Glow_Color[glow_10] );
    pixel_add( x,   y+1, Glow_Color[glow_10] );

    pixel_add( x-1, y-1, Glow_Color[glow_11] );
    pixel_add( x+1, y-1, Glow_Color[glow_11] );
    pixel_add( x-1, y+1, Glow_Color[glow_11] );
    pixel_add( x+1, y+1, Glow_Color[glow_11] );

    pixel_add( x,   y-2, Glow_Color[glow_20] );
    pixel_add( x-2, y,   Glow_Color[glow_20] );
    pixel_add( x+2, y,   Glow_Color[glow_20] );
    pixel_add( x,   y+2, Glow_Color[glow_20] );
}


//------------------------------------------------------------------------------

void
draw_lensflare( int x_center, int y_center, float radius, core::UDword color )
{
    float   linear = 0.05;//0.03;
    float   gauss  = 0.01;//0.006;
    float   mix    = 0.55;
    
    static bool         first_time = true;
    static core::UByte  ray[256];

    if( first_time )
    {
        for( int i=0; i<256; i++ )
            ray[i] = vfx::rnd(255);
    }
    
    for( int y=y_center-radius; y<y_center+radius; y++ )
    {
        float   dy = y_center - y;
        
        for( int x=x_center-radius; x<x_center+radius; x++ )
        {
            float   dx = x_center - x;
            float   d = sqrt( dx*dx + dy*dy );

            if( d > radius )
                continue;

            d /= radius;

            //float   a = (1 - d*d) - d*linear;//exp( -d*d*gauss );
            float   a = exp( -d*d*5 );

            a = (a>1.0)  ? 1.0  : (a<0)  ? 0.0  : a;

            core::UDword    c = color;
            scale( &c, a );

            
            // add rays

            float           v = ( atan2(dx, dy) + 3.1415 ) * 128.0/3.1415;
            int             i = floor( v );
            core::UDword    ray_color = color;//GalaxyColor2;
           
            v   = ( ray[i] + (ray[i+1]-ray[i] ) * (v-i) ) / 255.0;
            v   = pow( v, 2.0 ) / ( 1 + d );
            v   = (v<0)  ? 0.0  : (v>1)  ? 1.0  : v;
            v   = (d>0.2)  ? v*0.5*(1 - d*d)  : 1.0;

            scale( &ray_color, v );
            
            pixel_add( x, y, c );
            pixel_add( x, y, ray_color );
        }
    }     
}


//------------------------------------------------------------------------------

void
draw_glow( int x_center, int y_center, float radius, core::UDword color )
{
    static core::UByte  ray[256];

    for( int y=y_center-radius; y<y_center+radius; y++ )
    {
        float   dy = y_center - y;
        
        for( int x=x_center-radius; x<x_center+radius; x++ )
        {
            float   dx = x_center - x;
            float   d = sqrt( dx*dx + dy*dy );

            if( d > radius )
                continue;

            d /= radius;

            float   a = exp( -d*d*5 );

            a = (a>1.0)  ? 1.0  : (a<0)  ? 0.0  : a;

            core::UDword    c = color;
            scale( &c, a );
            
            pixel_add( x, y, c );
        }
    }     
}


//------------------------------------------------------------------------------

inline float
fade( float x )
{
    const float     b = 0.8;

    if( x < b )
    {
        return (x/b) * (x/b);
    }
    else
    {
        return 1.0;
    }
}


//------------------------------------------------------------------------------

void
draw_poly( Vector2f v[], int n, Rect2D clip_rect )
{
    // vertices should be in clockwise order

    using core::UDword;

    int                 top=0,bot=0;
    /*
    vfx::PerlinNoise    noise( perlin_grid_size * dir.length(),
                               perlin_grid_size
                             );
    */
    vfx::PerlinNoise    noise( 100, 100 );

    std::vector<Vector2f>    glow;

    // determine bounds

    int     l_bound         = v[0][0];
    int     r_bound         = v[0][0];
    int     t_bound         = v[0][1];
    int     b_bound         = v[0][1];
    int     bound_width;
    int     bound_height;
    int     center_x;
    int     center_y;

    for( int i=0; i<n; i++ )
    {
        if( v[i][0] < l_bound )
            l_bound = v[i][0];

        if( v[i][0] > r_bound )
            r_bound = v[i][0];

        if( v[i][1] < t_bound )
            t_bound = v[i][1];

        if( v[i][1] > b_bound )
            b_bound = v[i][1];
    }

    bound_width  = r_bound - l_bound;
    bound_height = b_bound - t_bound;
    center_x     = l_bound + bound_width/2;
    center_y     = t_bound + bound_height/2;


    // find top

    for( i=0; i<n; i++ )
    {
        if( v[i][1] < v[top][1] )
            top = i;

        if( v[i][1] > v[bot][1] )
            bot = i;
    }

    float   y       = v[top][1];
    int     l_height= 0, r_height = 0;
    float   l_dx, r_dx;
    float   l_x     = v[top][0];
    float   r_x     = v[top][0];
    int     l_i     = top;
    int     r_i     = top;

    
    // setup shaders

    PerlinNoiseShader*  perlin_shader[3]    = { 0, 0, 0 };
    FastNoiseShader*    fast_shader[3]      = { 0, 0, 0 };

    for( i=0; i<3; i++ )
    {
        switch( GalaxyType[i] )
        {
            case 0 :    // Perlin noise
                perlin_shader[i] = 
                    new PerlinNoiseShader( grid_size[i],
                                           grid_start[i], grid_start[i],
                                           GalaxyColor[i],
                                           intensity_base[i], intensity_range[i]
                                         );
                break;

            case 1 :    // fast noise
                fast_shader[i] = 
                    new FastNoiseShader( grid_size[i],
                                         grid_start[i], grid_start[i],
                                         GalaxyColor[i],
                                         intensity_base[i], intensity_range[i]
                                       );
                break;
        } 
    }


    /*
    PerlinNoiseShader   shader0( grid_size_0,
                                 grid_start_0, grid_start_0,
                                 GalaxyColor0,
                                 intensity_base_0, intensity_range_0
                               );
    
    PerlinNoiseShader   shader1( grid_size_1,
                                 grid_start_1, grid_start_1,
                                 GalaxyColor1,
                                 intensity_base_1, intensity_range_1
                               );
    
    PerlinNoiseShader   shader2( grid_size_2,
                                 grid_start_2, grid_start_2,
                                 GalaxyColor2,
                                 intensity_base_2, intensity_range_2
                               );
    
    FastNoiseShader   shader0( grid_size_0,
                               grid_start_0, grid_start_0,
                               GalaxyColor0,
                               intensity_base_0, intensity_range_0
                             );
    
    FastNoiseShader   shader1( grid_size_1,
                               grid_start_1, grid_start_1,
                               GalaxyColor1,
                               intensity_base_1, intensity_range_1
                             );
    
    FastNoiseShader   shader2( grid_size_2,
                               grid_start_2, grid_start_2,
                               GalaxyColor2,
                               intensity_base_2, intensity_range_2
                             );
    */

    // drawing goes here...

    while( (int)y < (int)v[bot][1] )
    {
        if( l_height == 0 )
        {
            int l_i2;

            while( l_height == 0 )
            {
                l_x         = v[l_i][0];
                l_i2        = (l_i>0)  ? l_i-1  : n-1;
                l_height    = v[l_i2][1] - v[l_i][1];
                l_dx        = ( v[l_i2][0] - v[l_i][0] ) / l_height;
                l_i         = l_i2;
            }
        }

        if( r_height == 0 )
        {
            int r_i2;

            while( r_height == 0 )
            {
                r_x         = v[r_i][0];
                r_i2        = (r_i<n-1)  ? r_i+1  : 0;
                r_height    = v[r_i2][1] - v[r_i][1];
                r_dx        = ( v[r_i2][0] - v[r_i][0] ) / r_height;
                r_i         = r_i2;
            }
        }


        if( y>=clip_rect.top()  &&  y<=clip_rect.bottom() )
        {
            int     x0 = (l_x>=clip_rect.left())  ? l_x  : clip_rect.left();
            int     x1 = (r_x<=clip_rect.right())  ? r_x  : clip_rect.right();

            for( int i=0; i<3; i++ )
            {
                if( GalaxyType[i] == 0 )
                {
                    perlin_shader[i]->next_scanline( x0,
                                                     (UDword*)Mem + 
                                                     Yoffset[(int)y] + 
                                                     (int)x0
                                                   );
                }
                else
                {
                    fast_shader[i]->next_scanline( x0,
                                                   (UDword*)Mem +
                                                   Yoffset[(int)y] +
                                                   (int)x0
                                                 );
                     
                }
            }
            
            for( int x=x0; x<x1; x++ )
            {
                float   diff_x = 1-fabs( (l_x+(r_x-l_x)/2-x) /
                                         float((r_x-l_x)/2) );
                float   diff_y = 1-fabs( (center_y-y) / float(bound_height/2) );
                float   fade_k = fade( diff_x ) * fade( diff_y );

                
                for( int i=0; i<3; i++ )
                {
                    if( GalaxyType[i] == 0 )
                    {
                        perlin_shader[i]->shade( fade_k );
                        perlin_shader[i]->next_pixel();
                    }
                    else
                    {
                        fast_shader[i]->shade( fade_k );
                        fast_shader[i]->next_pixel();
                    }
                }

                
                i = intensity_at(x,y);

                if( i > galaxy_threshold/*intensity_base_0*/ )
                {
                    if( vfx::rnd(255) > nebula_threshold )
                        glow.push_back( Vector2f(x,y) );
                }

                /*
                if( float(vfx::rnd(1000)) / 1000.0 > (1-star_probability) ||
                    intensity_at(x,y) > galaxy_threshold
                  )
                {
                    glow.push_back( Vector2f(x,y) );
                }*/
            }
        }

        l_x += l_dx;
        r_x += r_dx;

        ++y;
        --l_height;
        --r_height;
        //pixel( v[top][0], v[top][1]+xx, 0xFFFFFFFF );
    }

    
    // free shaders
    
    for( i=0; i<3; i++ )
    {
        if( perlin_shader[i] ) 
            delete perlin_shader[i];
        
        if( fast_shader[i] ) 
            delete fast_shader[i];
    }


    // add glows
    
    core::UDword    flare_color = GlowColor;
    core::UDword    glow_color  = GlowColor;
    scale( &glow_color, 0.4 );    

    
    for( int g=0; g<glow.size(); g++ )
    {   
        if( vfx::rnd(10) > 8 )
            draw_lensflare( glow[g][0], glow[g][1], 
                            3+vfx::rnd(3), flare_color
                          );
        else
        {
            draw_glow( glow[g][0], glow[g][1], 1+vfx::rnd(2), glow_color );
        }
    }
    
}


//------------------------------------------------------------------------------

void
draw_branch( float b_angle, float b_width,
             float da, float dr
           )
{
    int     t       = 0;
    int     ct      = 0;
    float   a       = b_angle * 3.1415/180.0;
    float   r       = 0;
    bool    first   = true;

    Vector2f prev1( 0, 0 );
    Vector2f cur1;
    Vector2f prev2( 0, 0 );
    Vector2f cur2;

    while( ++t < 500 )
    {
        if( ++ct == 20 )
        {
            if( first )
                first = false;
            else
            {
                float   x_cur1  = Width/2 + r * sin( a - b_width*3.1415/180.0 );
                float   y_cur1  = Height/2 + r * cos( a - b_width*3.1415/180.0 );
                float   x_cur2  = Width/2 + r * sin( a + b_width*3.1415/180.0 );
                float   y_cur2  = Height/2 + r * cos( a + b_width*3.1415/180.0 );

                Vector2f poly[] = { prev2, Vector2f( x_cur2, y_cur2 ),
                                   Vector2f( x_cur1, y_cur1 ), prev1
                                 };

                draw_poly( poly, 4, Rect2D(0,0,Width,Height) );

                prev1   = Vector2f( x_cur1, y_cur1 );
                prev2   = Vector2f( x_cur2, y_cur2 );
            }

            ct = 0;
        }

        r += dr;
        a += da;
    }
}


//------------------------------------------------------------------------------
/*
void
add_random_noise()
{
    for( int y=1; y<Height-1; y++ )
    {
        for( int x=1; x<Width-1; x++ )
        {
            int color = color_at( x, y ) -
                        rnd_noise_max/2 + vfx::rnd(rnd_noise_max);

            color = (color<0)  ? 0  : color;

            pixel( x, y,  color  );
        }
    }
}
*/

//------------------------------------------------------------------------------

void
draw()
{   //return;
    FILETIME    creation;
    FILETIME    exit;
    FILETIME    kernel_time1, kernel_time2;
    FILETIME    user_time1, user_time2;


    if( !Drawed )
    {
        precalc();

        ZeroMemory( Mem, Width*Height*4 );
        //clear_background_buffer();
        /*
        draw_front_branch( galaxy_a, galaxy_width, 400,
                           galaxy_da/5, galaxy_dr/5,
                           a_multiplier, t_scale
                         );
        */

        GetThreadTimes( GetCurrentThread(),
                        &creation, &exit, &kernel_time1, &user_time1
                      );

        //draw_universe();
        //add_random_noise();
        //add_glows( nebula_max/2+star_min_i+0.5*star_range_i );


        float   sx = 3.0;
        float   sy = 3.0;
        /*
        Vector2f     v[] = { Vector2f( 85*sx,  25*sy ),
                            Vector2f( 155*sx, 41*sy ),
                            Vector2f( 173*sx, 102*sy ),
                            Vector2f( 139*sx, 156*sy ),
                            Vector2f( 49*sx, 160*sy ),
                            Vector2f( 19*sx, 104*sy ),
                            Vector2f( 28*sx, 45*sy )
                          };
        */
        Vector2f     v[] = { Vector2f( 143*sx,  7*sy ),
                            Vector2f( 109*sx, 80*sy ),
                            Vector2f( 160*sx, 134*sy ),
                            Vector2f( 118*sx, 167*sy ),
                            Vector2f( 39*sx, 119*sy ),
                            Vector2f( 35*sx, 58*sy ),
                            Vector2f( 67*sx, 25*sy )
                          };
        /*
        Vector2f     v[] = { Vector2f( 70*sx,  50*sy ),
                            Vector2f( 124*sx, 41*sy ),
                            Vector2f( 158*sx, 69*sy ),
                            Vector2f( 95*sx, 173*sy ),
                            Vector2f( 25*sx, 188*sy ),
                            Vector2f( 19*sx, 106*sy )
                          };
        */
        for( int i=0; i<7; i++ )
        {
            v[i] += Vector2f( 50, 50 );
        }


        Vector2f    dir( 1.2, 0.0 );

        draw_poly( v, 7,
                   Rect2D( 0, 0, Width-1, Height-1 )
                 );
        
        
        sx = 1.0; sy = 1.0;
        Vector2f     v2[] = { Vector2f( 104*sx,  20*sy ),
                             Vector2f( 168*sx,  37*sy ),
                             Vector2f( 154*sx,  211*sy ),
                             Vector2f( 120*sx,   248*sy ),
                             Vector2f( 113*sx,  348*sy ),
                             Vector2f( 111*sx,  426*sy ),
                             Vector2f( 43*sx,   385*sy ),
                             Vector2f( 25*sx,   296*sy ),
                             Vector2f( 41*sx,   205*sy ),
                             Vector2f( 83*sx,   154*sy ),
                             Vector2f( 76*sx,   94*sy ),
                           };
        draw_poly( v2, 11,
                   Rect2D( 0, 0, Width-1, Height-1 )
                 );
        
        
        
        for( int y=10; y<Height-10; y++ )
        {
                if( vfx::rnd(100)/100.0 > 1.0-star_probability )
                    draw_lensflare( vfx::rnd(Width-1), y, 1+vfx::rnd(7), GlowColor );
        }
        
        //draw_lensflare( 50, 50, FlareSize, GlowColor );

        //draw_branch( galaxy_a, galaxy_width, galaxy_da, galaxy_dr );

        //add_glows( galaxy_threshold+50 );

        //remap_background_buffer();
        Drawed = true;

        GetThreadTimes( GetCurrentThread(),
                        &creation, &exit, &kernel_time2, &user_time2
                      );

        DWORD   time = user_time2.dwLowDateTime - user_time1.dwLowDateTime;
        char    buf[100];

        sprintf( buf, "draw() took %1.3f seconds", (float)time/10000000.0 );
        //MessageBox( NULL, buf, "Info", MB_OK|MB_ICONINFORMATION );
        PrintConsole( buf );
    }
}


//------------------------------------------------------------------------------

void
regen()
{
    core::UByte*    c_rgb;
    core::UByte     tmp;

    c_rgb    = (core::UByte*)&GalaxyColor[0];
    tmp      = c_rgb[0];
    c_rgb[0] = c_rgb[2];
    c_rgb[2] = tmp;

    c_rgb    = (core::UByte*)&GalaxyColor[1];
    tmp      = c_rgb[0];
    c_rgb[0] = c_rgb[2];
    c_rgb[2] = tmp;

    c_rgb    = (core::UByte*)&GalaxyColor[2];
    tmp      = c_rgb[0];
    c_rgb[0] = c_rgb[2];
    c_rgb[2] = tmp;

    c_rgb    = (core::UByte*)&GlowColor;
    tmp      = c_rgb[0];
    c_rgb[0] = c_rgb[2];
    c_rgb[2] = tmp;


    Drawed = false;
    draw();
    InvalidateRect( Window, NULL, FALSE );
}



//==============================================================================
//
//  Entry point here
//
//==============================================================================

int WINAPI
WinMain( HINSTANCE hInstance,
         HINSTANCE hPrevInstance,
         LPSTR lpCmdLine,
         int nCmdShow
       )
{
    WNDCLASS    wnd_class;
    MSG         msg;

    wnd_class.lpszClassName = "prog";
    wnd_class.hInstance     = hInstance;
    wnd_class.lpfnWndProc   = WndProc;
    wnd_class.hCursor       = NULL;
    wnd_class.hIcon         = NULL;
    wnd_class.lpszMenuName  = NULL;
    wnd_class.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH );
    wnd_class.style         = 0;
    wnd_class.cbClsExtra    = 0;
    wnd_class.cbWndExtra    = 0;

    if( !RegisterClass( &wnd_class) )
    {
        MessageBox( NULL, "can't register window class", "Error",
                    MB_OK|MB_ICONEXCLAMATION
                  );
        return 666;
    }

    Window = CreateWindow( "prog",
                           "galaxy",
                           WS_POPUP|WS_CAPTION,
                           20,
                           20,
                           Width,
                           Height,
                           NULL,
                           NULL,
                           hInstance,
                           NULL
                         );

    if( !Window )
    {
        MessageBox( NULL, "can't create window", "Error",
                    MB_OK|MB_ICONEXCLAMATION
                  );
        return 666;
    }

    ShowWindow( Window, SW_SHOWNORMAL );
    //SetWindowPos( Window, HWND_TOPMOST, 20,20, Width,Height, SWP_SHOWWINDOW );

    SetTimer( Window, 13, 20, NULL );

    /*
    SetPriorityClass( GetCurrentProcess(), REALTIME_PRIORITY_CLASS );
    SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL );
    */

    // DX crap

    HRESULT ddrval;

    ddrval = DirectDrawCreate( NULL, &lpDD, NULL );
    if( ddrval != DD_OK )
        return 666;

    ddrval = lpDD->SetCooperativeLevel( Window, DDSCL_NORMAL );
    if( ddrval != DD_OK )
        return 666;

    ddrval = lpDD->QueryInterface( IID_IDirectDraw2,
             (LPVOID *)&lpDD2);
    if( ddrval != DD_OK )
        return 666;


    // create back buffer

    HBITMAP     bitmap;
    BITMAPINFO  info;
    VOID*       ptr;

    Back_buffer = CreateCompatibleDC( NULL );
    GetClientRect( Window, &client_rect );

    ZeroMemory( &info.bmiHeader, sizeof(BITMAPINFOHEADER) );

    info.bmiHeader.biSize           = sizeof( BITMAPINFOHEADER );
    info.bmiHeader.biWidth          = Width;
    info.bmiHeader.biHeight         = -Height;
    info.bmiHeader.biPlanes         = 1;
    info.bmiHeader.biBitCount       = 32;
    info.bmiHeader.biCompression    = BI_RGB;
    info.bmiHeader.biSizeImage      = 0;

    bitmap = CreateDIBSection( Back_buffer,
                               &info,
                               DIB_RGB_COLORS,
                               &ptr,
                               NULL,
                               0
                             );

    SelectObject( Back_buffer, bitmap );
    Mem = (DWORD*)ptr;


    // startup

    Toster = LoadLibrary( "..\\..\\tool\\Toster\\toster.dll" );

    //AddFloatVariable( &galaxy_a, 0, 360.0, 360, "galaxy a" );
    //AddIntVariable( &galaxy_width, 1, 150, "galaxy width" );

    //AddFloatVariable( &galaxy_da, 0.001, 0.05, 200, "galaxy da" );
    AddIntVariable( &galaxy_threshold, 1, 255, "galaxy threshold" );
    AddIntVariable( &nebula_threshold, 1, 255, "nebula threshold" );
    //AddFloatVariable( &star_probability, 0.05, 0.15, 255, "star probability" );
 
    //AddIntVariable( &galaxy_period, 1, 100, "galaxy period" );
    //AddIntVariable( &branch_width, 1, 100, "branch width" );

    //AddFloatVariable( &a_multiplier, 0.01, 8, 200, "a multiplier" );
    //AddFloatVariable( &t_scale, 0.001, 0.05, 200, "t scale" );

    for( int i=0; i<3; i++ )
    {
        char    buf[50];
        
        sprintf( buf, "[%i] grid size", i );
        AddFloatVariable( &grid_size[i], 0.001, 0.5, 500, buf );

        sprintf( buf, "[%i] grid start", i );
        AddFloatVariable( &grid_start[i], 0.001, 5, 500, buf );

        sprintf( buf, "[%i] intensity base", i );
        AddIntVariable( &intensity_base[i], 0, 255, buf );

        sprintf( buf, "[%i] intensity range", i );
        AddIntVariable( &intensity_range[i], 0, 255, buf );
        
        sprintf( buf, "[%i] layer color", i );
        AddColorVariable( &GalaxyColor[i], buf );
        
        sprintf( buf, "[%i] kind of noise", i );
        AddList( &GalaxyType[i], "perlin;fast", buf );
    }
    AddColorVariable( &GalaxyColor01, "[0]1 galaxy color" );

    //AddIntVariable( &FlareSize, 4, 50, "frare size" );


    AddColorVariable( &GlowColor, "glow color" );

    RegisterApplyCallback( (VOID*)regen );

    std::clog = Log;

    //load_galaxy_palette( "gray.act" );


    while( GetMessage( &msg, NULL, 0, 0) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }

    DeleteObject( bitmap );
    DeleteObject( Back_buffer );

    KillTimer( Window, 13 );
    KillTimer( Window, 666 );

    return 0;

    FreeLibrary( Toster );

}




//------------------------------------------------------------------------------

LRESULT CALLBACK
WndProc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam )
{

    HDC dc = GetDC( wnd );

    switch( msg )
    {
        case WM_KEYDOWN :
            switch( wparam )
            {
                case VK_ESCAPE :
                    FreeLibrary( Toster );
                    PostQuitMessage(NULL);
                    break;

                case VK_SPACE :
                    if( !TosterCreated )
                    {
                        OpenToster();
                        TosterCreated = true;
                    }
                    break;
            }
            break;

        case WM_DESTROY :
            PostQuitMessage( NULL );
            break;

        case WM_PAINT :
            draw();
            if( !BitBlt( dc,
                    0,
                    0,
                    Width,
                    Height,
                    Back_buffer,
                    0,
                    0,
                    SRCCOPY
                  )) PostQuitMessage(NULL);
            break;


        case WM_TIMER :
            /*
            // drawin' goes here

            //ZeroMemory( Mem, Width*Height*4 );
            //FillMemory( Mem, Width*Height*4, 0xFFFFFFFF );

            //GdiFlush();

            //mem[2000] = 0x00FF0000;

            draw();

            BOOL    vsync = FALSE;

            while( !vsync )
            {
                lpDD2->GetVerticalBlankStatus( &vsync );
            }


            if( !BitBlt( dc,
                    0,
                    0,
                    Width,
                    Height,
                    Back_buffer,
                    0,
                    0,
                    SRCCOPY
                  )) PostQuitMessage(NULL);
            */
            // drawin's finished here
            break;

    }

    ReleaseDC( wnd, dc );

    return DefWindowProc( wnd, msg, wparam, lparam );
}


