#if !defined _RANDOM_HPP_
#define _RANDOM_HPP_
//==============================================================================
//
//  Random decls   
//
//==============================================================================
//
//  externals: 

    #include "Type.hpp"

namespace vfx
{
//==============================================================================
//
//  publics:

void
randomize( core::UDword a, core::UDword b, core::UDword c );

core::UDword
rnd( core::UDword max );
/*
float
rnd( float max );
*/
} // namespace vfx
#endif // _RANDOM_HPP_