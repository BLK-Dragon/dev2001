#if !defined _COLOR_HPP_
#define _COLOR_HPP_
//==============================================================================
//
//  Color issues   
//
//==============================================================================
//
//  externals: 

    #include "..\..\sys\Core.hpp"



class
Vec32
{
  public:

                    Vec32();
                    Vec32( core::UByte r, core::UByte g, core::UByte b,
                           core::UByte a=0
                         );
    explicit        Vec32( core::UDword color );

                    operator core::UDword();

    Vec32&          operator+= ( const Vec32& v );            
    Vec32&          operator-= ( const Vec32& v );
    Vec32&          operator*= ( float k );
    //Vec32&          operator/= ( core::UByte b );
    Vec32&          operator>>= ( int n );

    Vec32&          operator+ ( const Vec32& v );
    Vec32&          operator- ( const Vec32& v );
    //Vec32&          operator/ ( core::UByte b );
    Vec32&          operator>> ( int n );



  private:

    core::UByte     element[4];
};


//------------------------------------------------------------------------------

inline
Vec32::Vec32()
{
    *( (core::UDword*)element ) = 0x00000000;
}


//------------------------------------------------------------------------------

inline
Vec32::Vec32( core::UByte r, core::UByte g, core::UByte b, core::UByte a )
{
    element[0] = r;
    element[1] = g;
    element[2] = b;
    element[3] = a;
}


//------------------------------------------------------------------------------

inline
Vec32::Vec32( core::UDword color )
{
    *( (core::UDword*)element ) = color;
}


//------------------------------------------------------------------------------

inline
Vec32::operator core::UDword()
{
    return *( (core::UDword*)element );
}


//------------------------------------------------------------------------------

inline Vec32&
Vec32::operator+= ( const Vec32& v )
{
    for( int i=0; i<4; ++i )
    {
        int e = element[i] + v.element[i];
        
        element[i] = ( e < 0xFF)  ? e  : 0xFF;
    }     

    return *this;
}


//------------------------------------------------------------------------------

inline Vec32&       
Vec32::operator-= ( const Vec32& v )
{
    for( int i=0; i<4; ++i )
    {
        unsigned int e = element[i] - v.element[i];

        element[i] = ( e < 0xFF)  ? e  : 0x00;
    }
     
    return *this;
}


//------------------------------------------------------------------------------

inline Vec32&
Vec32::operator*= ( float k )
{
    for( int i=0; i<4; ++i )
    {
        int e = element[i] * k;

        element[i] = ( e < 0xFF)  ? e  : 0xFF;
    }
    
    return *this;
}



//------------------------------------------------------------------------------
/*
inline Vec32&
Vec32::operator/= ( core::UByte b )
{
    for( int i=0; i<4; ++i )
    {
        int e = element[i] / b;

        element[i] = ( e < 0xFF)  ? e  : 0xFF;
    }
    
    return *this;
}
*/

//------------------------------------------------------------------------------

inline Vec32&
Vec32::operator>>= ( int n )
{
    for( int i=0; i<4; ++i )
        element[i] >>= n;
    
    return *this;
}


//------------------------------------------------------------------------------

inline Vec32&
Vec32::operator+ ( const Vec32& v )
{
    *this += v;
    
    return *this;
}


//------------------------------------------------------------------------------

inline Vec32&
Vec32::operator- ( const Vec32& v )
{
    *this -= v;
    
    return *this;
}



//------------------------------------------------------------------------------
/*
inline Vec32&
Vec32::operator/ ( core::UByte b )
{
    *this /= b;

    return *this;
}
*/

//------------------------------------------------------------------------------

inline Vec32&
Vec32::operator>> ( int n )
{
    *this >>= n;
    
    return *this;
}


#endif // _COLOR_HPP_