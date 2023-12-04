#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <ddraw.h>
#include <math.h>

#include <iostream>
#include <fstream>
#include <iomanip>
using std::clog;
using std::flush;

#include "Color.hpp"
using core::UDword;

#include "..\..\sys\Win32Error.hpp"
#include "..\..\sys\Noise.hpp"
#include "..\..\tool\Toster\Toster.hpp"


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

int             FreqBase    = 60;
int             FreqRange   = 30;
int             NoiseBase   = -10;
int             NoiseRange  = 20;

int             plasma_R_base   = 0;
int             plasma_R_range  = 0;
int             plasma_B_base   = 20;
int             plasma_B_range  = 60;

float           perlin_grid_size    = 0.015;
float           perlin_persistence  = 0.5;
int             perlin_base         = 130;
int             perlin_range        = 50;

int             Bk_Base = 30;
int             Bk_Range = 100;

typedef Vec32 Color32;

std::ofstream   Log("main.log");


//------------------------------------------------------------------------------

void
precalc()
{
    for( int i=0; i<Height; i++ ) 
        Yoffset[i] = Width*i;
}


//------------------------------------------------------------------------------

inline Color32
color_at( int x, int y )
{
    return *( Mem + Yoffset[y] + x );
    //return Color32();
}


//------------------------------------------------------------------------------

inline void
pixel( int x, int y, Color32 color )
{
    *(Mem + Yoffset[y] + x ) = color;
}


//------------------------------------------------------------------------------

void
blur( int x1, int y1, int x2, int y2 )
{
    for( int y=y1; y<=y2; y++ ) 
    {
        for( int x=x1; x<=x2; x++ ) 
        {
            pixel( x, y,
                   (color_at(x-1,y-1)>>2) +
                   (color_at(x+1,y-1)>>2) +
                   (color_at(x-1,y+1)>>2) +
                   (color_at(x+1,y+1)>>2) 
                 );
        }
    }
}


//------------------------------------------------------------------------------

void
noise( int x1, int y1, int x2, int y2 )
{
    //vfx::init_perlin_noise( 0x8888888, 0x12121212, 0xAABBAACC );
    
    vfx::PerlinNoise    noise( perlin_grid_size, perlin_grid_size );


    float   Amp[8];
    float   freq[8];
    
    for( int i=0; i<8; i++ )
    {
        freq[i] = pow( 2, i );
        Amp[i]  = (vfx::rnd(4)/4.0);// pow( i, perlin_persistence );
    }

    
    for( int y=y1; y<=y2; y++ ) 
    {
        for( int x=x1; x<=x2; x++ ) 
        {   
            
            float   n = Amp[0] * noise.value_at( 1*x,  1*y ) +
                        Amp[1] * noise.value_at( 2*x,  2*y ) +
                        Amp[2] * noise.value_at( 4*x,  4*y ) +
                        Amp[3] * noise.value_at( 8*x,  8*y ) ;
                        //Amp[4] * noise.value_at( 16*x, 16*y ) +
                        //Amp[5] * noise.value_at( 32*x, 32*y );

            /*
            float   n1 = 1 * noise.value_at( 1*x, 1*y ); 
            float   n2 = 0.5 * noise.value_at( 2*x, 2*y );
            float   n3 = 0.25 * noise.value_at( 4*x, 4*y );

            float   n = n1+n2+n3;
            */
            int     c = perlin_base + n*perlin_range;

            c = (c>0xFF)  ? 0xFF  : (c<0)  ? 0  : c;
            
            pixel( x, y, Color32(c,c,c) );
            //std::clog << vfx::perlin_noise1D(x) << "  ";
        }
    }
}


//------------------------------------------------------------------------------

void
noise_blur( int x1, int y1, int x2, int y2 )
{
    for( int y=y1; y<=y2; y++ ) 
    {
        for( int x=x1; x<=x2; x++ ) 
        {
            int b = plasma_B_base + vfx::rnd(plasma_B_range); 
            int r = plasma_R_base + vfx::rnd(plasma_R_range); 
            //int r = -20 + rand()%80;

            pixel( x, y,
                   (color_at(x-1,y-1)>>2) +
                   (color_at(x+1,y-1)>>2) +
                   (color_at(x-1,y+1)>>2) +
                   (color_at(x+1,y+1)>>2) +
                   (Color32( b, 0, r )) 
                   //(color_at(x,y))
                 );
        }
    }
}



//------------------------------------------------------------------------------

inline Color32
rnd()
{
    static int N    = 0;
    static int Max  = 100;
    
    const int m = 15;
    const int n = 30;
    
    if( ++N == Max )
    {
        N = FreqBase+vfx::rnd(FreqRange);

        int b = plasma_B_base + vfx::rnd(plasma_B_range); 
        int r = plasma_R_base + vfx::rnd(plasma_R_range); 

        return Color32( b, b, r ); 
    }
    else
    {
        return 0;
    }
}


//------------------------------------------------------------------------------

void
midpoint_displacement( int x1, int y1, int x2, int y2 )
{
    int     dx = (x2 - x1) / 2;
    int     dy = (y2 - y1) / 2;
    
    if( dx == 0  &&  dy == 0 ) 
        return;

    
    if( color_at( x1+dx, y1 ) == 0x00000000 )
        pixel( x1+dx, y1, 
               (color_at(x1,y1)>>1) + (color_at(x2,y1)>>1) + rnd() 
             );
    if( color_at( x1+dx, y2 ) == 0x00000000 )
        pixel( x1+dx, y2, 
               (color_at(x1,y2)>>1) + (color_at(x2,y2)>>1) + rnd() 
             );
    if( color_at( x1, y1+dy ) == 0x00000000 )
        pixel( x1, y1+dy, 
               (color_at(x1,y1)>>1) + (color_at(x1,y2)>>1) + rnd()
             );
    if( color_at( x2, y1+dy ) == 0x00000000 )
        pixel( x2, y1+dy, 
               (color_at(x2,y1)>>1) + (color_at(x2,y2)>>1) + rnd()
             );

    //mid_pixel( x1, y1, x2, y1 );
    //mid_pixel( x1, y2, x2, y2 );
    //mid_pixel( x1, y1, x1, y2 );
    //mid_pixel( x2, y1, x2, y2 );
    
    if( color_at( x1+dx, y1+dy ) == 0x00000000 )
    {
        Color32 c = (color_at(x1,y1)>>2) + (color_at(x2,y1)>>2) +
                    (color_at(x1,y2)>>2) + (color_at(x2,y2)>>2) + rnd();
        //c += Color32( rand()%5, rand()%5, 0 );

        pixel( x1+dx, y1+dy, c );
    }
    
    
    midpoint_displacement( x1, y1, x1+dx, y1+dy ); 
    midpoint_displacement( x1+dx, y1, x2, y1+dy );
    midpoint_displacement( x1, y1+dy, x1+dx, y2 );
    midpoint_displacement( x1+dx, y1+dy, x2, y2 );
}


//------------------------------------------------------------------------------

void
scale( float k, int x1, int y1, int x2, int y2 )
{
    for( int y=y1; y<=y2; y++ ) 
    {
        for( int x=x1; x<=x2; x++ ) 
        {
            Color32 c = color_at( x, y );
            c *= k;
            pixel( x, y, c );
        }
    }
}


//------------------------------------------------------------------------------

void
draw( UDword* mem )
{
    Color32 Color1( 0xFF, 0, 0 );
    Color32 Color2( 0, 0xFF, 0 );
    Color32 Color3( 0, 0, 0xFF );
    Color32 Color4( 0xFF, 0xFF, 0xFF );

    //vfx::randomize( 0x11111111, 0x22222222, 0x33333333 );
    vfx::randomize( 0xFF111111, 0x22222222, 0x33333333 );

    
    if( !Drawed )
    {   
        ZeroMemory( Mem, Width*Height*4 );
        precalc();
        
        /*
        for( int i=0; i<20; i++ )
            pixel( rand()%(Width-1), rand()%(Height-1), Color32(0x00FF0000) );
        */

        /*
        pixel( 0, 0, Color1 );
        pixel( Width-1, 0, Color2 );
        pixel( 0, Height-1, Color3 );
        pixel( Width-1, Height-1, Color4 );
        */
        
        pixel( 0, 0, Color32( Bk_Base+vfx::rnd(Bk_Range), 0, 0 ) );
        pixel( Width-1, 0, Color32( Bk_Base+vfx::rnd(Bk_Range), 0, 0 ) );
        pixel( 0, Height-1, Color32( Bk_Base+vfx::rnd(Bk_Range), 0, 0 ) );
        pixel( Width-1, Height-1, Color32( Bk_Base+vfx::rnd(Bk_Range), 0, 0 ) );

        //midpoint_displacement( 0, 0, Width-1, Height-1 );
        //blur( 1, 1, Width-2, Height-2 );
        //noise_blur( 1, 1, Width-2, Height-2 );
        //blur( 1, 1, Width-2, Height-2 );
        //scale( 0.5, 0, 0, Width-1, Height-1 );
        noise( 0, 0, Width-1, Height-1 );
        Drawed = true;
    }
}


//------------------------------------------------------------------------------

void
regen()
{
    Drawed = false; 
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
                           "midpoint_displacement",
                           WS_POPUP,
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


    //AddIntVariable( &Bk_Base, 1, 255, "background min" );
    //AddIntVariable( &Bk_Range, 1, 255, "background range" );
    
    //AddIntVariable( &plasma_B_base, 1, 255, "plasma B base" );
    //AddIntVariable( &plasma_B_range, 1, 255, "plasma B range" );
    
    //AddIntVariable( &plasma_R_base, 1, 255, "plasma R base" );
    //AddIntVariable( &plasma_R_range, 1, 255, "plasma R range" );
    
    AddFloatVariable( &perlin_grid_size, 0.001, 0.1, 200, "perlin grid size" );
    AddFloatVariable( &perlin_persistence, 0.001, 1.0, 250, "perlin persistence" );
    AddIntVariable( &perlin_base, 1, 255, "perlin base" );
    AddIntVariable( &perlin_range, 1, 255, "perlin range" );
    
    RegisterApplyCallback( (VOID*)regen );

    std::clog = Log;

    
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
            if( wparam == VK_ESCAPE )
            {
                PostQuitMessage( NULL ); 
            }
            else if( wparam == VK_SPACE ) 
            {
                if( !TosterCreated )
                {
                    OpenToster(); 
                    TosterCreated = true;
                }
            }
            break;

        case WM_DESTROY :
            PostQuitMessage( NULL );
            break;

        case WM_TIMER :
            
            // drawin' goes here

            //ZeroMemory( Mem, Width*Height*4 );
            //FillMemory( Mem, Width*Height*4, 0xFFFFFFFF );
            
            //GdiFlush();
            
            //mem[2000] = 0x00FF0000;
            
            draw( (UDword*)Mem );
            
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

            // drawin's finished here
            break;

    }

    ReleaseDC( wnd, dc );

    return DefWindowProc( wnd, msg, wparam, lparam );
}


