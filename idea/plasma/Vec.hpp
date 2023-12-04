#if !defined _VEC_HPP_
#define _VEC_HPP_
//==============================================================================
//
//  Vec   
//
//==============================================================================
//
//  externals: 

    #include "..\..\sys\Core.hpp"
    using core::UByte;
    using core::UWord;
    using core::UDword;


    //typedef UByte   Elem;


    

template< class Elem, int n >  
class
Vec  
{
  public:  
    
    Elem                element[n];

  public:


    
    class
    Assigner
    {
        Elem&           elem;

      public:

                        Assigner( Elem& e);
        void            operator=( const Elem& e );
                        operator Elem();
    };


                        Vec();
                        Vec( UDword d );
                        Vec( UByte v0, UByte v2, UByte v3, UByte v4=0 );

    Assigner            operator[] ( int i );
    void                operator+=( const Vec& v );
    void                operator-=( const Vec& v );
    void                operator/=( Elem e );
    void                operator/=( int e );

    Vec<Elem,n>&        operator+( const Vec<Elem,n>& v );
    Vec<Elem,n>&        operator-( const Vec<Elem,n>& v );
    
    template<class Elem, int n> 
    friend Vec<Elem,n>  operator/( const Vec<Elem,n>& v, Elem e );
    
    template<class Elem, int n> 
    friend Vec<Elem,n>  operator/( const Vec<Elem,n>& v, int e );

                        operator UDword();
};


//------------------------------------------------------------------------------

template< class Elem, int n >  
inline 
Vec<Elem,n>::Assigner::Assigner( Elem& e )
  : elem( e )
{
}


//------------------------------------------------------------------------------

template< class Elem, int n >  
inline void
Vec<Elem,n>::Assigner::operator= ( const Elem& e )
{
    elem = e; 
}


//------------------------------------------------------------------------------

template< class Elem, int n >  
inline
Vec<Elem,n>::Assigner::operator Elem()
{
    return elem; 
}


//==============================================================================

template< class Elem, int n >  
inline
Vec<Elem,n>::Vec<Elem,n>()
{
    for( int i=0; i<3; ++i )
        element[i] = 0;
}


//------------------------------------------------------------------------------

template< class Elem, int n >  
inline Vec<Elem,n>::Assigner
Vec<Elem,n>::operator[] ( int i )
{
    return Assigner( element[i] ); 
}


//------------------------------------------------------------------------------

template< class Elem, int n >  
inline void
Vec<Elem,n>::operator+= ( const Vec& v )
{
    for( int i=0; i<n; i++ ) 
        this->element[i] += v.element[i];
}


//------------------------------------------------------------------------------

template< class Elem, int n >  
inline void
Vec<Elem,n>::operator-= ( const Vec& v )
{
    for( int i=0; i<n; i++ ) 
        this->element[i] -= v.element[i];
}


//------------------------------------------------------------------------------

template< class Elem, int n >  
inline void
Vec<Elem,n>::operator/= ( Elem e )
{
    for( int i=0; i<n; i++ ) 
        this->element[i] /= e;
}


//------------------------------------------------------------------------------

template< class Elem, int n >  
inline void
Vec<Elem,n>::operator/= ( int e )
{
    for( int i=0; i<n; i++ ) 
        element[i] /= (Elem)e;
}


//------------------------------------------------------------------------------


template<class Elem, int n> 
inline Vec<Elem,n>&        
Vec<Elem,n>::operator+( const Vec<Elem,n>& v )
{
    for( int i=0; i<n; i++ ) 
        element[i] += v.element[i];

    return *this;
}


//------------------------------------------------------------------------------

template< class Elem, int n >  
inline Vec<Elem,n>&
Vec<Elem,n>::operator-( const Vec<Elem,n>& v )
{
    for( int i=0; i<n; i++ ) 
        element[i] -= v.element[i];

    return *this;
}


//------------------------------------------------------------------------------

template< class Elem, int n >  
inline Vec<Elem,n>
operator/ ( const Vec<Elem,n>& v, Elem e )
{
    Vec<Elem,n> r(v);

    for( int i=0; i<n; i++ ) 
        r.element[i] /= e;

    return r;
}


//------------------------------------------------------------------------------

template< class Elem, int n >  
inline Vec<Elem,n>
operator/ ( const Vec<Elem,n>& v, int e )
{
    Vec<Elem,n> r(v);

    for( int i=0; i<n; i++ ) 
        r.element[i] /= (Elem)e;

    return r;
}


//==============================================================================
//
//  special cases
//
//==============================================================================

inline
Vec<UByte,4>::Vec<UByte,4>( UDword d )
{
    *( (UDword*)element ) = d;
}


//------------------------------------------------------------------------------

inline
Vec<UByte,4>::Vec<UByte,4>( UByte v0, UByte v1, UByte v2, UByte v3 )
{
    element[0] = v0;
    element[1] = v1;
    element[2] = v2;
    element[3] = v3;
}


//------------------------------------------------------------------------------
 
inline
Vec<UByte,4>::operator UDword()
{
    return *( (UDword*)element );
}



#endif // _VEC_HPP_