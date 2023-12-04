
    #include "..\..\sys\Color32.hpp"
    #include "..\..\sys\FixedPoint.hpp"
    #include <stdio.h>
    #include <stdlib.h>

    using namespace core;

main()
{
    typedef Fixed<8,24>    fixed;

    fixed   f1  = 1.2f;
    //fixed   f2  = 5;

    //f1 /= 7;
    //f2 -= 1;

    printf( "f1/7 = %f (f1=%f)\n", float(f1/fixed(7.0f)), float(f1) );
    printf( "f1*7 = %f (f1=%f)\n", float(f1*fixed(7.0f)), float(f1) );
    printf( "f1+2.6 = %f (f1=%f)\n", float(f1+fixed(2.6f)), float(f1) );
    printf( "f1-2.6 = %f (f1=%f)\n", float(f1-fixed(2.6f)), float(f1) );
    //printf( "f2 = %i (%08X)\n", int(f2), int(f2) );

    
/*    
    vfx::Color32     c1( 0x22222222 );
//    vfx::Color32     c2( 0xAAAAAAAA );

    core::UDword    line1[4] = { 0x00000000, 0x11111111, 0x22222222, 0x33333333 };
//    core::UDword    line2[20];

    for( int i=0; i<4; i++ )
        vfx::Color32( line1+i ) += 0x11111111;
    
    
    for( i=0; i<4; i++ )
        printf( "line1[%i] == %08X\n", i, line1[i] );
*/
}