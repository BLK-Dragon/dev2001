
#include "..\..\sys\Core.hpp"
#include "Color.hpp"

void
main()
{
    Vec32   c1( 0x11223344 );// ( 0x11, 0x22, 0x33, 0x44 );
    Vec32   c2( 0x112233FF );// ( 0x11, 0x22, 0x33, 0x44 );
    
    
    printf( "%08X\n", (core::UDword)c1 );
    printf( "%08X\n", (core::UDword)c2 );

    Vec32   c3 = c1 >> 1; // - Vec32( 0x000000FF );//c1 + c2;
                      
    printf( "%08X\n", (core::UDword)c3 );

}