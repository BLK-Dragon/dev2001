#if !defined _POINT2D_HPP_
#define _POINT2D_HPP_
//==============================================================================
//
//  2D-Point
//
//==============================================================================
//
//  externals: 


//------------------------------------------------------------------------------
//
//  publics:




class
Point2D
{
    float               x_pos,y_pos;

  public:

                        Point2D();
                        Point2D( float _x, float _y );

    float               x() const;
    float               y() const;
    
    
    Point2D&            operator+=( const Point2D& p );
    
    friend Point2D      operator+( const Point2D& p1, const Point2D& p2 );
    friend Point2D      operator-( const Point2D& p1, const Point2D& p2 );
};


//------------------------------------------------------------------------------

inline
Point2D::Point2D()
  : x_pos(0.0), 
    y_pos(0.0)
{
}

//------------------------------------------------------------------------------

inline
Point2D::Point2D( float _x, float _y )
  : x_pos(_x), y_pos(_y)
{
}


//------------------------------------------------------------------------------

inline float
Point2D::x() const
{
    return x_pos; 
}


//------------------------------------------------------------------------------

inline float
Point2D::y() const
{
    return y_pos; 
}


//------------------------------------------------------------------------------

inline Point2D&
Point2D::operator+=( const Point2D& p )
{
    x_pos += p.x_pos;
    y_pos += p.y_pos;

   return *this; 
}


//------------------------------------------------------------------------------

inline Point2D
operator+( const Point2D& p1, const Point2D& p2 )
{
    return Point2D( p1.x_pos+p2.x_pos, p1.y_pos+p2.y_pos ); 
}


//------------------------------------------------------------------------------

inline Point2D
operator-( const Point2D& p1, const Point2D& p2 )
{
    return Point2D( p1.x_pos-p2.x_pos, p1.y_pos-p2.y_pos ); 
}


#endif // _POINT2D_HPP_