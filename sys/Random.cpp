//==============================================================================
//
//  Random realization
//
//==============================================================================
//
//  externals: 

    #include <stdlib.h>
    
    #include "Random.hpp"


    core::UDword A = 0xDEADBEAF;
    core::UDword B = 0xABCDABCD;
    core::UDword C = 0xFFAAFFAA;

namespace vfx
{
//==============================================================================
//
//  publics:


//------------------------------------------------------------------------------

void
randomize( core::UDword a, core::UDword b, core::UDword c )
{
    A = a;
    B = b;
    C = c; 
}


//------------------------------------------------------------------------------

core::UDword
rnd( core::UDword max )
{   /*
    return rand()%max;
    
    if( max== 0 )
        return 0;
    
    A += B + C;

    A = ( A & 0x70000000 )  ? (A<<1)|1  : (A<<1)&0xFFFFFFFE;

    core::UDword tmp = A;

    A = B;
    B = C;
    C = tmp;

    return (max)  ? A%max  : A;
    */
    return rand()%max;
}


//------------------------------------------------------------------------------
/*
float
rnd( float max )
{
    return max * ( ((float)rnd(0xFFFFFFFF)) / ((float)0xFFFFFFFF) );
}
*/

}