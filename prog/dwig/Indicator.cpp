//==============================================================================
//
//  Indicator realization
//
//==============================================================================
//
//  externals:

    #include "Indicator.hpp"
    #include "Clip.hpp"


//==============================================================================
//
//  publics:


Indicator::Indicator()
  : SchemeElement( 2, 1 )
{
}


//------------------------------------------------------------------------------

float               
Indicator::output_intensity( int n )
{
    return (SchemeElement::inputs())[0]->cur_intensity() +
           (SchemeElement::inputs())[1]->cur_intensity();
}

