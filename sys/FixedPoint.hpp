#if !defined _FIXEDPOINT_HPP_
#define _FIXEDPOINT_HPP_
//==============================================================================
//
//  Fixed Point operations   
//
//==============================================================================
//
//  externals: 

    #include "Type.hpp"

namespace core
{
//==============================================================================
//
//  publics:




template< int intgr, int frac >  
class
Fixed  
{
  public:
                    
                    Fixed();
                    Fixed( const Fixed& f );
                    Fixed( int i );
                    Fixed( float f );

    Fixed&          operator+=( const Fixed& f );
    Fixed&          operator-=( const Fixed& f );
    Fixed&          operator*=( const Fixed& f );
    Fixed&          operator/=( const Fixed& f );

    friend Fixed    operator+( const Fixed& f1,  const Fixed& f2 );
    friend Fixed    operator-( const Fixed& f1,  const Fixed& f2 );
    friend Fixed    operator*( const Fixed& f1,  const Fixed& f2 );
    friend Fixed    operator/( const Fixed& f1,  const Fixed& f2 );

                    operator int();
                    operator float();

  private:

  Dword         val;
};


typedef Fixed<16,16> Fixed_16_16;


//------------------------------------------------------------------------------

template< int intgr, int frac >
inline
Fixed<intgr,frac>::Fixed()
  : val(0)
{
}


//------------------------------------------------------------------------------

template< int intgr, int frac >
inline
Fixed<intgr,frac>::Fixed( const Fixed<intgr,frac>& f )
  : val(f.val)
{
}


//------------------------------------------------------------------------------

template< int intgr, int frac >
inline
Fixed<intgr,frac>::Fixed( int i )
  : val( i<<frac )
{
}


//------------------------------------------------------------------------------

template< int intgr, int frac >
inline
Fixed<intgr,frac>::Fixed( float f )
  : val( Dword(f*(long(0x1)<<(frac+0))) )
{
}


//------------------------------------------------------------------------------

template< int intgr, int frac >
inline Fixed<intgr,frac>&
Fixed<intgr,frac>::operator+=( const Fixed<intgr,frac>& f )
{
    this->val += f.val;

    return *this; 
}


//------------------------------------------------------------------------------

template< int intgr, int frac >
inline Fixed<intgr,frac>&
Fixed<intgr,frac>::operator-=( const Fixed<intgr,frac>& f )
{
    this->val -= f.val;

    return *this;
}


//------------------------------------------------------------------------------

template< int intgr, int frac >
inline Fixed<intgr,frac>&
Fixed<intgr,frac>::operator*=( const Fixed<intgr,frac>& f )
{
    Dword   op1 = this->val;
    Dword   op2 = f.val;
    
    __asm{
            mov     EAX, op1
            imul    op2
            shrd    EAX,EDX,frac

            mov     op1, EAX
         };
    
//    this->val = Dword( ( long(this->val) * long(f.val) )/* >> frac*/ );
    this->val = op1;

    return *this;
}


//------------------------------------------------------------------------------

template< int intgr, int frac >
inline Fixed<intgr,frac>&
Fixed<intgr,frac>::operator/=( const Fixed<intgr,frac>& f )
{
    Dword   op1 = this->val;
    Dword   op2 = f.val;
    
    __asm{
            mov     EAX, op1
            mov     EBX, op2
            xor     EDX, EDX

            shld    EDX, EAX, frac

            idiv    EBX
            mov     op1, EAX
         };
    
    this->val = op1;

    return *this;
}



//------------------------------------------------------------------------------

template< int intgr, int frac >
inline Fixed<intgr,frac>   
operator+( const Fixed<intgr,frac>& f1,  const Fixed<intgr,frac>& f2 )
{
    Fixed<intgr,frac>   result(f1);

    result += f2;
    return result;
}



//------------------------------------------------------------------------------

template< int intgr, int frac >
inline Fixed<intgr,frac>
operator-( const Fixed<intgr,frac>& f1,  const Fixed<intgr,frac>& f2 )
{
    Fixed<intgr,frac>   result(f1);

    result -= f2;
    return result;
}



//------------------------------------------------------------------------------

template< int intgr, int frac >
inline Fixed<intgr,frac>   
operator*( const Fixed<intgr,frac>& f1,  const Fixed<intgr,frac>& f2 )
{
    Fixed<intgr,frac>   result(f1);

    result *= f2;
    return result;
}



//------------------------------------------------------------------------------

template< int intgr, int frac >
inline Fixed<intgr,frac>   
operator/( const Fixed<intgr,frac>& f1,  const Fixed<intgr,frac>& f2 )
{
    Fixed<intgr,frac>   result(f1);

    result /= f2;
    return result;
}



//------------------------------------------------------------------------------

template< int intgr, int frac >
inline
Fixed<intgr,frac>::operator int()
{
    return (val>>frac);     
}


//------------------------------------------------------------------------------

template< int intgr, int frac >
inline
Fixed<intgr,frac>::operator float()
{
    return ( float(val) / float(long(0x1)<<(frac+0)) );
}


} // namespace core
#endif // _FIXEDPOINT_HPP_