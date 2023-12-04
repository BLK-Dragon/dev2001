#if !defined _COLORINTERPOLATOR_HPP_
#define _COLORINTERPOLATOR_HPP_
//==============================================================================
//
//  Class for color interpolation   
//
//==============================================================================
//
//  externals:

    #include "..\..\sys\Type.hpp"


//==============================================================================
//
//  publics:




class
ColorInterpolator  
{
  public:

                    ColorInterpolator( core::UDword color1,
                                       core::UDword color2,
                                       int n
                                     );

    core::UDword    cur_value() const;
    void            calc_next_value();

  
  private:

    float           r, g, b;
    float           dr, dg, db;
    core::UDword    color;
};



//------------------------------------------------------------------------------

inline
ColorInterpolator::ColorInterpolator( core::UDword color1,
                                      core::UDword color2,
                                      int n
                                    )
{
    r   = ( (color1>>0) & 0xFF );
    g   = ( (color1>>8) & 0xFF );
    b   = ( (color1>>16) & 0xFF );

    dr  = ( float( (color2>>0) & 0xFF ) - r ) / n;
    dg  = ( float( (color2>>8) & 0xFF ) - g ) / n;
    db  = ( float( (color2>>16) & 0xFF ) - b ) / n;
}


//------------------------------------------------------------------------------

inline core::UDword    
ColorInterpolator::cur_value() const
{
    return color;
}


//------------------------------------------------------------------------------

inline void 
ColorInterpolator::calc_next_value()
{
    r += dr;
    g += dg;
    b += db;

    color = core::UDword(r) | (core::UDword(g)<<8) | (core::UDword(b)<<16);
}


#endif // _COLORINTERPOLATOR_HPP_