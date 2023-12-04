#if !defined _RECT_HPP_
#define _RECT_HPP_
//==============================================================================
//
//  Rectangle template
//
//==============================================================================
//
//  externals: 

    #include "Vector.hpp"


//==============================================================================
//
//  publics:




template < class Type >  
class
Rect  
{
  
  public:

                    Rect();
                    Rect( const Vector<Type,2>& orig, Type w, Type h );
                    Rect( Type l, Type t, Type r, Type b );

    Vector<Type,2>  origin() const;
    Type            left() const;
    Type            right() const;
    Type            top() const;
    Type            bottom() const;
    Type            width() const;
    Type            height() const;

    bool            is_null() const;
    
    bool            inside_of( const Rect<Type>& rect ) const;
    bool            outside_of( const Rect<Type>& rect ) const;

    friend Rect<Type> overlap( const Rect<Type>& r1, const Rect<Type>& r2 ); 

    bool            includes_point( const Vector<Type,2>& p );

    Rect<Type>      operator +=( const Vector<Type,2>& p );

    void            resize( Type dl, Type dt, Type dr, Type db );
    void            move_to( const Vector<Type,2>& pos );

  
  private:

    Type            _top;
    Type            _left;
    Type            _right;
    Type            _bottom;
};

typedef     Rect<int>   Rect2D;


//------------------------------------------------------------------------------

template < class Type >
inline 
Rect<Type>::Rect<Type>()
  : _left(0),
    _right(0),
    _top(0),
    _bottom(0)
{
}


//------------------------------------------------------------------------------

template < class Type >
inline
Rect<Type>::Rect<Type>( const Vector<Type,2>& orig, Type w, Type h )
{
    _left   = orig[0];
    _top    = orig[1];
    _right  = _left + w;
    _bottom = _top + h;
}


//------------------------------------------------------------------------------

template < class Type >
inline
Rect<Type>::Rect<Type>( Type l, Type t, Type r, Type b )
  : _left(l),
    _top(t),
    _right(r),
    _bottom(b)
{
}


//------------------------------------------------------------------------------

template < class Type >
inline Type             
Rect<Type>::left() const
{
    return _left; 
}


//------------------------------------------------------------------------------

template < class Type >
inline Type             
Rect<Type>::right() const
{
    return _right;
}


//------------------------------------------------------------------------------

template < class Type >
inline Type             
Rect<Type>::top() const
{
    return _top; 
}


//------------------------------------------------------------------------------

template < class Type >
inline Type             
Rect<Type>::bottom() const
{
    return _bottom; 
}


//------------------------------------------------------------------------------

template < class Type >
inline Vector<Type,2> 
Rect<Type>::origin() const
{
    return Vector<Type,2>( _left, _top );
}


//------------------------------------------------------------------------------

template < class Type >
inline Type
Rect<Type>::width() const
{
    return _right - _left;
}


//------------------------------------------------------------------------------

template < class Type >
inline Type 
Rect<Type>::height() const
{
    return _bottom - _top;
}


//------------------------------------------------------------------------------

template < class Type >
inline bool
Rect<Type>::is_null() const
{
    if( _right==0 && _left== 0 && _bottom==0 && _top==0 )
        return true;
    else
        return false;
}


//------------------------------------------------------------------------------

template < class Type >
inline bool 
Rect<Type>::inside_of( const Rect<Type>& rect ) const
{
    return false;
}


//------------------------------------------------------------------------------

template < class Type >
inline bool 
Rect<Type>::outside_of( const Rect<Type>& rect ) const
{
    return false;
}


//------------------------------------------------------------------------------

template < class Type >
inline Rect<Type>   
overlap( const Rect<Type>& r1, const Rect<Type>& r2 )
{
    Rect<Type>  ovl;

    ovl._left   = r1._left <= r2._left  ? r2._left  : r1._left;
    ovl._right  = r1._right >= r2._right  ? r2._right  : r1._right;
    ovl._top    = r1._top <= r2._top  ?  r2._top  : r1._top;
    ovl._bottom = r1._bottom >= r2._bottom  ? r2._bottom  : r1._bottom;

    if( (ovl._right-ovl._left) <= 0  ||  (ovl._bottom-ovl._top) <= 0 )
    {
        ovl._left   = 0;
        ovl._right  = 0;
        ovl._top    = 0;
        ovl._bottom = 0;
    } 

    return ovl;
}


//------------------------------------------------------------------------------

template < class Type >
inline bool            
Rect<Type>::includes_point( const Vector<Type,2>& p )
{
    if( p[0] >= _left  &&  p[0] <= _right  &&
        p[1] >= _top  &&  p[1] <= _bottom
      )
    {
        return true; 
    }
    else
        return false;
}


//------------------------------------------------------------------------------

template < class Type >
inline Rect<Type>          
Rect<Type>::operator +=( const Vector<Type,2>& p )
{
    _left   += p[0];
    _right  += p[0];
    _top    += p[1];
    _bottom += p[1];

    return *this;
}


//------------------------------------------------------------------------------

template < class Type >
inline void            
Rect<Type>::resize( Type dl, Type dt, Type dr, Type db )
{
    _left   += dl;
    _right  += dr;
    _top    += dt;
    _bottom += db;
}


//------------------------------------------------------------------------------

template < class Type >
inline void
Rect<Type>::move_to( const Vector<Type,2>& pos )
{
    Type    width   = _right - _left;
    Type    height  = _bottom - _top;

   _left    = pos[0];
   _top     = pos[1];
   _right   = _left + width;
   _bottom  = _top + height;
}




#endif // _RECT_HPP_