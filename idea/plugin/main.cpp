
    #include <windows.h>
    #include <vector>

    #include "Fruit.hpp"



//==============================================================================
//
//  Entry point here
//
//==============================================================================


void
main()
{
    std::vector<Fruit*>     fruit;
    std::vector<HMODULE>    plugin;
    std::vector<Creator>    creator;
    std::vector<Killer>     killer;

    
    plugin.push_back( ::LoadLibrary("orange.dll") );
    plugin.push_back( ::LoadLibrary("lemon.dll") );

    creator.push_back( (Creator)::GetProcAddress(plugin[0],"CreateFruit") );
    creator.push_back( (Creator)::GetProcAddress(plugin[1],"CreateFruit") );
    killer.push_back( (Killer)::GetProcAddress(plugin[0],"DestroyFruit") );
    killer.push_back( (Killer)::GetProcAddress(plugin[1],"DestroyFruit") );

    
    fruit.push_back( (creator[0])() );
    fruit.push_back( (creator[1])() );

    fruit[0]->show();
    fruit[1]->show();

    //delete fruit[0];
    //delete fruit[1];
    (killer[0])(fruit[0]);
    (killer[1])(fruit[1]);

    ::FreeLibrary( plugin[0] );
    ::FreeLibrary( plugin[1] );
}