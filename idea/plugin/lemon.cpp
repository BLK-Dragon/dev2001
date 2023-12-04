
    #include <windows.h>
    #include <stdio.h>

    #include "Fruit.hpp"


//==============================================================================
//
//  Entry point here
//
//==============================================================================


BOOL WINAPI 
DllMain( HINSTANCE instance, DWORD reason, LPVOID lpReserved )
{
    switch( reason ) 
    { 
        case DLL_PROCESS_ATTACH:
            // Initialize once for each new process.
            // Return FALSE to fail DLL load.
            break;

        case DLL_THREAD_ATTACH:
            // Do thread-specific initialization.
            break;

        case DLL_THREAD_DETACH:
            // Do thread-specific cleanup.
            break;

        case DLL_PROCESS_DETACH:
            // Perform any necessary cleanup.
            break;
    }
    
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}



//------------------------------------------------------------------------------




class
Lemon
  : public Fruit
{
  
  public:

    virtual void        show() { printf("Lemon\n"); };

};




//------------------------------------------------------------------------------

__declspec(dllexport) Fruit* 
CreateFruit()
{
    return new Lemon; 
}


//------------------------------------------------------------------------------

__declspec(dllexport) void 
DestroyFruit( Fruit* fruit )
{
    delete fruit; 
}


