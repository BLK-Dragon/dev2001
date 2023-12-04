#if !defined _JUMPER_HPP_
#define _JUMPER_HPP_
//==============================================================================
//
//
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
Jumper 
  : public SchemeElement
{
  public:    

                                Jumper();

    virtual float               output_intensity( int n=0 );
    
    void                        connect_output( Clip* clip );
    void                        connect_input( Clip* clip );    

  
  private:
    
};


//------------------------------------------------------------------------------

inline
Jumper::Jumper()
  : SchemeElement( 1, 1 )
{
}


//------------------------------------------------------------------------------

inline float               
Jumper::output_intensity( int n )
{
    return (SchemeElement::inputs())[0]->cur_intensity();
}


//------------------------------------------------------------------------------

inline void                        
Jumper::connect_output( Clip* clip )
{
    SchemeElement::connect_output( 0, clip );
}


//------------------------------------------------------------------------------

inline void                        
Jumper::connect_input( Clip* clip )
{
    SchemeElement::connect_input( 0, clip );
}

#endif // _JUMPER_HPP_
