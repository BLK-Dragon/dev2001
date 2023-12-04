#if !defined _FILL_HPP_
#define _FILL_HPP_
//==============================================================================
//
//  Filling related decls   
//
//==============================================================================
//
//  externals: 

    #include "BitOps.hpp"

namespace vfx
{
//==============================================================================
//
//  publics:




class
Fill1D  
{
  public:

    virtual core::UDword    value_at( float t ) const =0;
//    virtual core::UDword    value_at( int t ) const;
};


//==============================================================================
//
//  solid fill




class
SolidFill1D
  : public Fill1D  
{
    
  public:

                            SolidFill1D( core::UDword _color );
    
    virtual core::UDword    value_at( float t ) const;


  private:

    core::UDword            color;
};


//------------------------------------------------------------------------------

inline
SolidFill1D::SolidFill1D( core::UDword _color )
  : color(_color)
{
}


//------------------------------------------------------------------------------
    
inline core::UDword    
SolidFill1D::value_at( float t ) const
{
    return color; 
}





} // namespace vfx
#endif // _FILL_HPP_