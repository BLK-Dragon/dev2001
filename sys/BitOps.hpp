#if !defined _BITOPS_HPP_
#define _BITOPS_HPP_
//==============================================================================
//
//  Bit operations   
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




class
ColorInterpolation  
{
  public:

                    ColorInterpolation( core::UDword c1, core::UDword c2, int n );
                   ~ColorInterpolation();

    core::UDword    operator[]( int i );

  private:

    core::UDword*   color;

};


//------------------------------------------------------------------------------

inline
ColorInterpolation::ColorInterpolation( core::UDword c1, core::UDword c2, int n )
{
    color = new core::UDword[n];

    core::UByte*    _c1 = (core::UByte*)&c1;
    core::UByte*    _c2 = (core::UByte*)&c2;

    double  r   = double(_c1[0]);
    double  g   = double(_c1[1]);
    double  b   = double(_c1[2]);
    double  dr  = ( double(_c2[0]) - double(_c1[0]) ) / n;
    double  dg  = ( double(_c2[1]) - double(_c1[1]) ) / n;
    double  db  = ( double(_c2[2]) - double(_c1[2]) ) / n;

    for( int i=0; i<n; i++ )
    {
        core::UByte*    c = (core::UByte*)(color+i);

        c[0] = (core::UByte)r;
        c[1] = (core::UByte)g;
        c[2] = (core::UByte)b;

        r += dr;
        g += dg;
        b += db;
    }
}


//------------------------------------------------------------------------------

inline
ColorInterpolation::~ColorInterpolation()
{
    delete[] color;
}


//------------------------------------------------------------------------------

inline core::UDword 
ColorInterpolation::operator[]( int i )
{
    return color[i];
}


//==============================================================================

inline core::UDword
rgb( core::UByte r, core::UByte g, core::UByte b )
{
    return core::UDword(r)                  | 
           ((core::UDword(g)<<8)&0xFF00)    | 
           ((core::UDword(b)<<16)&0xFF0000);
}


//------------------------------------------------------------------------------

inline core::UDword
bgr( core::UByte b, core::UByte g, core::UByte r )
{
    return core::UDword(r)                  | 
           ((core::UDword(g)<<8)&0xFF00)    | 
           ((core::UDword(b)<<16)&0xFF0000);
}



//==============================================================================

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


//------------------------------------------------------------------------------

inline void
block_fill( core::UDword* dst, int len, core::UDword color )
{
     for( int i=0; i<len; i++ )
        *dst++ = color;
}


//------------------------------------------------------------------------------

inline void
block_copy( core::UDword* dst, core::UDword* src, int len )
{
     for( int i=0; i<len; i++ )
        *dst++ = *src++;
}


//------------------------------------------------------------------------------

inline void
block_add( core::UDword* dst, core::UDword* src, int len )
{                
    __asm{
                push    ECX
                push    EDI
                push    ESI

                mov     ESI,src
                mov     EDI,dst
                mov     ECX,len

        loop_add:
                
                movd    MM0,[EDI]
                paddusb MM0,[ESI]
                movd    [EDI],MM0

                add     ESI,4
                add     EDI,4
                
                dec     ECX
                jnz     loop_add 

                pop     ESI
                pop     EDI
                pop     ECX
                emms
         }     
}


//------------------------------------------------------------------------------

inline void
block_blend( core::UDword* dst, core::UDword* src, int len, float alpha )
{                
    for( int i=0; i<len; i++ )
    {
        core::UByte*    src_color   = (core::UByte*)&src[i];
        core::UByte*    dst_color   = (core::UByte*)&dst[i];
        //core::UByte     result[4]   = {0,0,0,0};

        for( int k=0; k<3; k++ )
        {
            dst_color[k] = alpha * float(src_color[k]) + 
                           (1.0-alpha) * float(dst_color[k]);
        }

        //*dst_color = *( (core::UDword*) result );
    }
    
    
    /*
    core::UWord     k   = core::UWord( alpha * 32768 );
    core::UDword    k2  = (k<<16) | k;
    core::UWord     r   = core::UWord( (1.0-alpha) * 32768 );
    core::UDword    r2  = (r<<16) | r;
    core::UByte     mask[8] = {0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00};
    
    __asm{
                push    ECX
                push    EDI
                push    ESI

                mov     ESI,src
                mov     EDI,dst
                mov     ECX,len

                ; put alpha into MM5
                movd    MM0,k2
                movd    MM1,k2
                psllq   MM0,16
                por     MM0,MM1
                movq    MM5,MM0
                
                ; put 1-alpha into MM6
                movd    MM0,k2
                movd    MM1,k2
                psllq   MM0,16
                por     MM0,MM1
                movq    MM6,MM0

                pxor    MM7,MM7
                movq    MM3,mask


        loop_blend:
                
                pxor        MM0,MM0
                punpcklbw   MM0,[ESI]
                pxor        MM1,MM1
                punpcklbw   MM1,[EDI]

                pmulhw      MM0,MM6
                pmulhw      MM1,MM5

                psrlw       MM0,7
                psrlw       MM1,7
                packuswb    MM0,MM7
                packuswb    MM1,MM7

                paddusb     MM0,MM1
                movd        [EDI],MM0

                add     ESI,4
                add     EDI,4
                
                dec     ECX
                jnz     loop_blend 

                pop     ESI
                pop     EDI
                pop     ECX
                emms
         }     
    */
}


} // namespace vfx

#endif // _BITOPS_HPP_