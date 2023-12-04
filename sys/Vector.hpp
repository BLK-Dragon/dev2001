#if !defined _VECTOR_HPP_
#define _VECTOR_HPP_
//==============================================================================
//
//  Vector   
//
//==============================================================================
//
//  externals: 

    
    #include <math.h>


//------------------------------------------------------------------------------
//
//  publics:




template < class Type, int size >  
class
Vector
{
  public:

                        Vector();
                        Vector( Type v0, Type v1 );
                        Vector( Type v0, Type v1, Type v2 );

    Type&               operator[] ( int i );
    const Type&         operator[] ( int i ) const;
    
    Vector&             operator+=( const Vector& v );
    Vector&             operator-=( const Vector& v );
    Vector&             operator*=( Type a );
    Type                length();
    
    friend Vector       operator*( const Vector& v, Type a );
    friend Vector       operator*( Type a, const Vector& v );


    friend Vector       operator+( const Vector& u, const Vector& v );
    friend Vector       operator-( const Vector& u, const Vector& v );
    friend Vector       operator-( const Vector& u );

    friend bool         operator==(  const Vector& u, const Vector& v );
    friend bool         operator!=(  const Vector& u, const Vector& v );

/*

    float               length() const;
    Vector2D&           normalize();
    Vector2D&           rotate( float angle );
*/

  private:

    Type                v[size];

};


typedef Vector<int,2>       Vector2i;
typedef Vector<long,2>      Vector2l;
typedef Vector<float,2>     Vector2f;
typedef Vector<float,3>     Vector3f;

typedef Vector<int,2>       Size2i;
typedef Vector<float,2>     Size2f;


//------------------------------------------------------------------------------

template < class Type, int size>
inline
Vector<Type,size>::Vector<Type,size>()
{
    for( int i=0; i<size; i++ )
        v[i] = 0;
}


//------------------------------------------------------------------------------

template < class Type, int size>
inline
Vector<Type,size>::Vector<Type,size>( Type v0, Type v1 )
{
    v[0] = v0;
    v[1] = v1;
    
    for( int i=2; i<size; i++ )
        v[i] = 0;
}


//------------------------------------------------------------------------------

template < class Type, int size>
inline
Vector<Type,size>::Vector<Type,size>( Type v0, Type v1, Type v2  )
{
    v[0] = v0;
    v[1] = v1
    v[2] = v1
    
    for( int i=3; i<size; i++ )
        v[i] = 0;
}


//------------------------------------------------------------------------------

template < class Type, int size>
inline Type& 
Vector<Type,size>::operator[]( int i )
{
    return this->v[i];
}


//------------------------------------------------------------------------------

template < class Type, int size>
inline const Type& 
Vector<Type,size>::operator[]( int i ) const
{
    return this->v[i];
}


//------------------------------------------------------------------------------

template < class Type, int size>
inline Vector<Type,size>& 
Vector<Type,size>::operator+=( const Vector<Type,size>& v )
{
    for( int i=0; i<size; i++ )
        this->v[i] += v.v[i];

    return *this;
}


//------------------------------------------------------------------------------

template < class Type, int size>
inline Vector<Type,size>& 
Vector<Type,size>::operator-=( const Vector<Type,size>& v )
{
    for( int i=0; i<size; i++ )
        this->v[i] -= v.v[i];

    return *this;
}


//------------------------------------------------------------------------------

template < class Type, int size >
inline Vector<Type,size>& 
Vector<Type,size>::operator*=( Type a )
{
    for( int i=0; i<size; i++ )
        this->v[i] *= a;

    return *this;
}


//------------------------------------------------------------------------------

template < class Type, int size >
inline Type
Vector<Type,size>::length()
{
    Type   l = 0;

    for( int i=0; i<size; i++ )
        l += v[i] * v[i];

    return sqrt( l );
}


//------------------------------------------------------------------------------

template < class Type, int size>
inline Vector<Type,size>
operator*( const Vector<Type,size>& v, Type a )
{
    Vector<Type,size> r = v;
    
    for( int i=0; i<size; i++ )
        r.v[i] *= a;

    return r;
}


//------------------------------------------------------------------------------

template < class Type, int size>
inline Vector<Type,size>
operator*( Type a, const Vector<Type,size>& v )
{
    Vector<Type,size> r = v;
    
    for( int i=0; i<size; i++ )
        r.v[i] *= a;

    return r;
}


//------------------------------------------------------------------------------

template < class Type, int size>
inline Vector<Type,size>
operator+( const Vector<Type,size>& u, const Vector<Type,size>& v )
{
    Vector<Type,size> r = u;
    
    r += v;

    return r;
}


//------------------------------------------------------------------------------

template < class Type, int size>
inline Vector<Type,size>
operator-( const Vector<Type,size>& u, const Vector<Type,size>& v )
{
    Vector<Type,size> r = u;
    
    r -= v;

    return r;
}


//------------------------------------------------------------------------------

template < class Type, int size>
inline Vector<Type,size>
operator-( const Vector<Type,size>& u )
{
    Vector<Type,size> r = u;
    
    for( int i=0; i<size; i++ )
        r.v[i] = -u.v[i];

    return r;
}


/*
//------------------------------------------------------------------------------

inline Vector2D& 
Vector2D::operator-=( const Vector2D& v )
{
    x -= v.x;
    y -= v.y;

    return *this;
}


//------------------------------------------------------------------------------

inline Vector2D& 
Vector2D::operator*=( float a )
{
    x *= a;
    y *= a;

    return *this;
}


//------------------------------------------------------------------------------

inline Vector2D 
operator*( const Vector2D& v, float a )
{
    return Vector2D( v.x*a, v.y*a );
}


//------------------------------------------------------------------------------

inline Vector2D
operator*( float a, const Vector2D& v )
{
    return Vector2D( v.x*a, v.y*a );
}


//------------------------------------------------------------------------------

inline Vector2D 
operator+( const Vector2D& u, const Vector2D& v )
{
    return Vector2D( u.x+v.x, u.y+v.y);
}


//------------------------------------------------------------------------------

inline Vector2D 
operator-( const Vector2D& u, const Vector2D& v )
{
    return Vector2D( u.x-v.x, u.y-v.y);
}


//------------------------------------------------------------------------------

inline Vector2D 
operator-( const Vector2D& u )
{
    return Vector2D( -u.x, -u.y );
}


//------------------------------------------------------------------------------

inline
Vector2D::operator Point2D() const
{
    return Point2D( x, y ); 
}


//------------------------------------------------------------------------------

inline float&
Vector2D::operator[] ( int i )
{
    return (i==0)  ? x  : y; 
}


//------------------------------------------------------------------------------

inline float
Vector2D::length() const
{
    return sqrt( x*x + y*y ); 
}


//------------------------------------------------------------------------------

inline Vector2D&
Vector2D::normalize()
{
    float len = sqrt( x*x + y*y ); 

    x /= len;
    y /= len;
    
    return *this;
}


//------------------------------------------------------------------------------

inline Vector2D&
Vector2D::rotate( float angle )
{
    float   tmp = x;
    float   a   = angle*3.1415926535897931/180.0;

    x = tmp * cos(a) + y * sin(a);
    y = tmp * sin(a) - y * cos(a);

    return *this;
}
*/

//------------------------------------------------------------------------------

template < class Type, int size>
inline bool         
operator==( const Vector<Type,size>& u, const Vector<Type,size>& v )
{
    for( unsigned i=0; i<size; i++ )
        if( u.v[i] != v.v[i]) return false;

    return true; 
}


//------------------------------------------------------------------------------

template < class Type, int size>
inline bool         
operator!=( const Vector<Type,size>& u, const Vector<Type,size>& v )
{
    return  (u == v)  ? false  : true;
}

#endif // _VECTOR_HPP_