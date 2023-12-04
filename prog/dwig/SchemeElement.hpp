#if !defined _SCHEMEELEMENT_HPP_
#define _SCHEMEELEMENT_HPP_
//==============================================================================
//
//  Scheme Element base
//
//==============================================================================
//
//  externals:

    #include <vector>
    

//==============================================================================
//
//  publics:

class Clip;



class
SchemeElement
{
  public:
                                SchemeElement( int n_ins, int n_outs );
    virtual                    ~SchemeElement() {};
    
    virtual float               output_intensity( int n=0 ) =0;
    
    void                        connect_output( int n, Clip* clip );
    void                        connect_input( int n, Clip* clip );    
    
    const std::vector<Clip*>&   inputs();
    const std::vector<Clip*>&   outputs();

  
  private :
    std::vector<Clip*> input;
    std::vector<Clip*> output;
};


//------------------------------------------------------------------------------

inline
SchemeElement::SchemeElement( int n_ins, int n_outs )
{
    for( unsigned i=0; i<n_ins; i++ )
        input.push_back( 0 );     
    for( i=0; i<n_outs; i++ )
        output.push_back( 0 );     
}


//------------------------------------------------------------------------------

inline void 
SchemeElement::connect_output( int n, Clip* clip )
{
    output[n] = clip;
}


//------------------------------------------------------------------------------

inline void 
SchemeElement::connect_input( int n, Clip* clip )
{
    input[n] = clip;
}


//------------------------------------------------------------------------------

inline const std::vector<Clip*>& 
SchemeElement::inputs()
{
    return input;
}


//------------------------------------------------------------------------------

inline const std::vector<Clip*>& 
SchemeElement::outputs()
{
    return output;
}


#endif // _SCHEMEELEMENT_HPP_
