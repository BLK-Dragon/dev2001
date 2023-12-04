#if !defined _INDICATOR_HPP_
#define _INDICATOR_HPP_
//==============================================================================
//
//  Indicator decl
//
//==============================================================================
//
//  externals:

    #include "SchemeElement.hpp"
    #include "Clip.hpp"


//==============================================================================
//
//  publics:




class
Indicator 
  : public SchemeElement
{
  public:    
    
                                Indicator();
    virtual float               output_intensity( int n=0 );
  
  
  private:
};

#endif // _INDICATOR_HPP_
