#if !defined _BITOPS_HPP_
#define _BITOPS_HPP_
//==============================================================================
//
//  Some useful bit-ops for 32bit RGBA color   
//
//==============================================================================
//
//  externals:

    #include "..\..\sys\Type.hpp"


//==============================================================================
//
// publics:


inline void
scale_color( core::UDword* src_color, float s )
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
pixel_add( core::UDword* dst, core::UDword src )
{
    core::UByte*    src_color   = (core::UByte*)dst;
    core::UByte     color[4];

    __asm{
            push    EAX

            mov     EAX, src_color
            movd    MM0, dword ptr [EAX]
            paddusb MM0, qword ptr src
            movd    [EAX], MM0

            emms
            pop     EAX
         };
}


//------------------------------------------------------------------------------

inline void
pixel_alpha( core::UDword* dst, core::UDword src, int alpha )
{
    core::UByte*    src_color   = (core::UByte*)dst;
    core::UByte     color[4];

    __asm{
            push    EAX

            mov     EAX, src_color
            movd    MM0, dword ptr [EAX]
            paddusb MM0, qword ptr src
            movd    [EAX], MM0

            emms
            pop     EAX
         };
}


//------------------------------------------------------------------------------

inline core::UByte
intensity_of( core::UDword color )
{
    core::UByte*    c = (core::UByte*)&color;

    return core::UByte( ( (core::UDword)c[0] + c[1] + c[2] ) / 3 );
}


#endif // _BITOPS_HPP_