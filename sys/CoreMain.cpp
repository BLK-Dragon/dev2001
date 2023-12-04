#include <windows.h>
#include <ddraw.h>

#include <iostream>
#include <fstream>
#include <iomanip>
using std::clog;
using std::flush;
#include <stdio.h>


#include "Win32Error.hpp"
using core::try_win32;
#include "VideoCard.hpp"
#include "Keyboard.hpp"
using core::UByte;
using core::UWord;
using core::UDword;

#include "Application.hpp"

//------------------------------------------------------------------------------
//
//  global windows crap:

HWND            Window;
HINSTANCE       Instance;

LRESULT CALLBACK
WndProc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam );


//------------------------------------------------------------------------------
//
//  my globals:

int                 Width   = 100;
int                 Height  = 100;
std::ofstream       Log;

namespace core
{
Application*        App;
VideoCard*          Application::videocard;
Keyboard*           Application::keyboard;
Mouse*              Application::mouse;

bool            AppPaused   = false;

DWORD           Time        = GetTickCount();
int             Frames      = 0;
DWORD           TotalTime   = 0;
float           CurFPS      = 0;

}   // namespace core

//------------------------------------------------------------------------------

void
pre_init()
{
    SYSTEMTIME  time;
    char        log_name[80];
    
    ::GetLocalTime( &time );
    sprintf( log_name, "%02i.%02i.%04i %02i%02i'%02i'%03i.log", 
             time.wDay, time.wMonth, time.wYear,
             time.wHour, time.wMinute, time.wSecond, time.wMilliseconds
           );
    Log.open( log_name );
    
    
    std::clog = Log;

    core::Core                   = new core::ProgramCore( Instance, Window );
    core::Application::videocard = new core::VideoCard; 
    core::Application::keyboard  = new core::Keyboard;
    core::Application::mouse     = new core::Mouse;
}


//------------------------------------------------------------------------------

void
shutdown()
{
    std::clog << "clearing system..." << flush;

    delete core::App;
    delete core::Application::mouse;
    delete core::Application::keyboard;
    delete core::Application::videocard;     
    delete core::Core;
    std::clog << "done.\nexiting" << flush;
}


//------------------------------------------------------------------------------

void
WriteSysInfoToLog()
{
    // OS info

    using namespace core;

    OSVERSIONINFO   os_info;

    os_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    ::GetVersionEx( &os_info );

    std::clog << "------------------------------------------------------------\n"
                 "System Info:\n\n";
    std::clog << Msg( "Windows %i.%i %s build %i\n",
                      os_info.dwMajorVersion,
                      os_info.dwMinorVersion,
                      os_info.szCSDVersion,
                      (os_info.dwMajorVersion<5)  ? LOWORD(os_info.dwBuildNumber)
                                                  : os_info.dwBuildNumber
                    ).text()
              << std::endl;


    // memory info

    MEMORYSTATUS  mem_info;

    //mem_info.dwLength = sizeof(MEMORYSTATUSEX);
    ::GlobalMemoryStatus( &mem_info );

    std::clog << Msg( "Total Phys Mem  %iMb\n"
                      "Avail Phys Mem  %iMb\n"
                      "Memory Load     %02i%%\n",
                      mem_info.dwTotalPhys/(1024*1024),
                      mem_info.dwAvailPhys/(1024*1024),
                      mem_info.dwMemoryLoad
                    ).text()
              << std::endl;


    // CPU info

    char    firm_id[15];
    char*   firm_id_ptr = firm_id;
    UDword  version,capability;

    __asm{
            pusha

            ; get firm ID

            xor     EAX,EAX
            cpuid

            mov     EDI,firm_id_ptr

            mov     byte ptr [EDI],BL
            inc     EDI
            shr     EBX,8
            mov     byte ptr [EDI],BL
            inc     EDI
            shr     EBX,8
            mov     byte ptr [EDI],BL
            inc     EDI
            shr     EBX,8
            mov     byte ptr [EDI],BL

            inc     EDI
            mov     byte ptr [EDI],DL
            inc     EDI
            shr     EDX,8
            mov     byte ptr [EDI],DL
            inc     EDI
            shr     EDX,8
            mov     byte ptr [EDI],DL
            inc     EDI
            shr     EDX,8
            mov     byte ptr [EDI],DL

            inc     EDI
            mov     byte ptr [EDI],CL
            inc     EDI
            shr     ECX,8
            mov     byte ptr [EDI],CL
            inc     EDI
            shr     ECX,8
            mov     byte ptr [EDI],CL
            inc     EDI
            shr     ECX,8
            mov     byte ptr [EDI],CL

            ; get CPU capabilities

            mov     EAX,1
            cpuid
            mov     dword ptr version,EAX
            mov     dword ptr capability,EDX

            popa
        }

    firm_id[12] = '\0';

    int         num_clocks      = 300000000;
    int         c               = num_clocks/2;
    FILETIME    time1, time2, dummy;

    ::GetProcessTimes( ::GetCurrentProcess(), 
                       &dummy, &dummy, &dummy, &time1 
                     );
    
    __asm{
            pushad

            mov     ECX, c
            
lloop:
            mov     EAX, EBX
            mov     EBP, EDX
            
            dec     ECX
            jnz     lloop
            
            popad
         } 

    ::GetProcessTimes( ::GetCurrentProcess(), 
                       &dummy, &dummy, &dummy, &time2 
                     );
    
    ULARGE_INTEGER  t;
    __int64         t1, t2;

    t.LowPart   = time1.dwLowDateTime;
    t.HighPart  = time1.dwHighDateTime;
    t1          = t.QuadPart;
    
    t.LowPart   = time2.dwLowDateTime;
    t.HighPart  = time2.dwHighDateTime;
    t2          = t.QuadPart;


    double  sec = double(t2-t1)/10000000.0;
    double  f   = (double)num_clocks / sec;


    std::clog << Msg( "CPU: \"%s\" %i.%i.%i @%4.2fMHz %s %s %s %s\n",
                      firm_id,
                      ((version&mask_Family)>>8)&0x0000000F,
                      ((version&mask_Model)>>4)&0x0000000F,
                      version&mask_Stepping_ID,
                      f/1000000.0,
                      (capability&FPU_on_chip)  ? "FPU"  : "",
                      (capability&MMX_Instructions)  ? "MMX"  : "",
                      (capability&Time_Stamp_Counter)  ? "TSC"  : "",
                      (capability&CMOV_Instructions)  ? "CMOV"  : ""
                    ).text()
              << std::endl;
    
    
    // videocard info

    DDCAPS  driver_caps;
    DDCAPS  hel_caps;


    driver_caps.dwSize  = sizeof(DDCAPS);
    hel_caps.dwSize     = sizeof(DDCAPS);
    try_ddraw
    (
        Core->ddraw()->GetCaps( &driver_caps, &hel_caps )
    );
    
    DDDEVICEIDENTIFIER2 identifier;
    /*
    try_ddraw
    (
        Core->ddraw()->GetDeviceIdentifier( &identifier, 0 );
    );
    */
//    std::clog << identifier.szDriver << " " << identifier.szDescription << "\n";
    
    Surface&    front_buf   = Application::videocard->front_buffer();
    Surface&    back_buf    = Application::videocard->back_buffer();
    int         vid_mem     = driver_caps.dwVidMemTotal;

    vid_mem += front_buf.pitch() * front_buf.height();
    vid_mem += back_buf.pitch() * back_buf.height();

    std::clog << Msg( "VideoCard: \"%s\" %iMb VidMem\n",
                      "",
                      vid_mem / (1024*1024)
                    ).text()
              << std::endl;

    std::clog << "------------------------------------------------------------\n\n";
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
    //using namespace core;
    using core::Application;
    using core::VideoCard;
    using core::Keyboard;
    
    try        
    {    
        Instance = hInstance;
        
        WNDCLASS    wnd_class;     
        MSG         msg;

        ZeroMemory( &msg, sizeof(msg) );

        wnd_class.lpszClassName = "core win";
        wnd_class.hInstance     = hInstance;
        wnd_class.lpfnWndProc   = WndProc;
        wnd_class.hCursor       = NULL;
        wnd_class.hIcon         = NULL;
        wnd_class.lpszMenuName  = NULL;
        wnd_class.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH );
        wnd_class.style         = CS_BYTEALIGNWINDOW;
        wnd_class.cbClsExtra    = 0;
        wnd_class.cbWndExtra    = 0;
        
        if( !RegisterClass( &wnd_class) )
            throw core::Win32Error( "can't register window class" );
        
        Window = CreateWindow( "core win",
                               "1",
                               WS_POPUP,
                               0,
                               0,
                               Width,
                               Height,
                               NULL,
                               NULL,
                               hInstance,
                               NULL
                             );
        
        if( Window == NULL )
        {    
            DWORD   code = GetLastError();
            
            throw core::Win32Error( "can't create window" );
            /*
            throw core::Error( core::Msg( (core::win32_error(code)).c_str() 
                                        ) 
                             );*/
        }

        
        ShowWindow( Window, SW_SHOWNORMAL );
        UpdateWindow( Window );
        /*
        SetWindowPos( Window, HWND_TOPMOST, 20,20, 
                      Width,Height, SWP_SHOWWINDOW 
                    );
        */
        SetCursor( NULL );
        
        pre_init();
        core::App = CreateApplication();

        core::App->init();
        WriteSysInfoToLog();
        
        Rect2D  win = core::App->window_location();
        SetWindowPos( core::Core->window(), HWND_TOPMOST,
                      win.left(), win.top(),
                      win.right() - win.left(), win.bottom() - win.top(),
                      0
                    );

  
        
        while(true)
        {  
            if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
            {
                if( GetMessage( &msg, NULL, 0, 0) )
                {
                    if( msg.message == WM_QUIT )
                        break;
                    
                    TranslateMessage( &msg );
                    DispatchMessage( &msg );
                }
            }
            
            
            Application::keyboard->update_state();
            Application::mouse->update_state();
            Rect2D  win = core::App->window_location();
            
            SetWindowPos( core::Core->window(), HWND_TOPMOST,
                          win.left(), win.top(),
                          win.right() - win.left(), win.bottom() - win.top(),
                          0
                        );
            
            Application::videocard->resize_wnd_back_buffer();
            
            
            core::Application::state    state;
            
            state = (core::AppPaused)  
                    ? Application::paused
                    : core::App->update();
            
            
            if( state == Application::finished )
                break;
            else if( state == Application::paused )
                core::AppPaused = true;

            if( !core::AppPaused ) 
            {    
                core::App->draw();

                DWORD   cur_time = GetTickCount();
                int     dt       = cur_time - core::Time;

                core::Time        = cur_time;
                core::TotalTime  += dt;

                ++core::Frames;
                //VideoCard->wait_for_vsync();
                if( core::TotalTime > 1000 )
                {
                    core::CurFPS     = core::Frames / 
                                       ( (float)core::TotalTime / 1000.0 );
                    core::TotalTime -= 1000;
                    core::Frames     = 0;
                }

                
                Application::videocard->flip_pages( core::App->flip_method() );
            }
        } // while(true)
        
        shutdown();
        return 0;
    }
    catch( const core::Error& e )
    {
        clog << e.comment() << "> " << e.description() << "\n" << flush;
        return -13;
    }
    catch( ... )
    {
        clog << "uknown error, aborting\n" << flush;
        return -13;
    }
    
    delete core::App;
    clog << "correct exit\n" << flush;
}




//------------------------------------------------------------------------------

LRESULT CALLBACK
WndProc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
    return core::Core->wnd_proc( wnd, msg, wparam, lparam );
    //PAINTSTRUCT ps;    

    switch( msg ) 
    {   /*
        case WM_PAINT :
            BeginPaint( Window, &ps );
            EndPaint( Window, &ps );
            break;
        */

        case WM_CREATE :
        case WM_GETMINMAXINFO :
            return 0; 
            break;

        case WM_NCCREATE :
            return TRUE;
            break;

        case WM_ACTIVATE :
            if( wparam == WA_ACTIVE  ||  wparam == WA_CLICKACTIVE )
            {
                core::AppPaused = false;
                return 0;
            }
            else if( wparam == WA_INACTIVE )
            {
                core::AppPaused = true;
                return 0; 
            }
            break;



        case WM_DESTROY :
            shutdown();
            PostQuitMessage( NULL );
            break;
        
        case WM_KEYDOWN :
            switch( wparam )
            {
                case VK_ESCAPE :
                    shutdown();
                    PostQuitMessage( NULL );
                    break;
                /*
                case VK_LEFT :
                    MovingLeft  = true;
                    break;

                case VK_RIGHT :
                    MovingRight = true; 
                    break;
                
                case VK_UP :
                    //StarLayer1->add_speed( Vector2D( 0, 0.1 ) ); 
                    MovingUp    = true;
                    break;

                case VK_DOWN :
                    //StarLayer1->add_speed( Vector2D( 0, -0.1 ) ); 
                    MovingDown  = true;
                    break;*/
            }
            break;
        
        case WM_KEYUP :
            break;

        case WM_TIMER :
            /*
            if( wparam == 888 )
            {
                DWORD   cur_time = GetTickCount();

                for( int i=0; i<N_BOOMS; i++ )
                {
                    Phase[i] += cur_time-Time;

                    if( Phase[i] > 5000 )
                    {
                        if( i==0 )
                        {
                             CurFPS = Frames / ( (float)Phase[i] / 1000.0 );
                             Frames = 0;
                             Log << CurFPS << "\n" << flush;
                        }
                        
                        Phase[i] -= 5000;
                        Boom[i]->explode();

                        Center.x = -60 + rand()%120;
                        Center.y = -60 + rand()%120;
                        
                    }
                }
                
                TotalTime  += cur_time-Time;
                Time        = cur_time;
                break; 
            }
            */
            break;
        /*
        default :
            return DefWindowProc( wnd, msg, wparam, lparam );
        */
    }

    return 0;
}



