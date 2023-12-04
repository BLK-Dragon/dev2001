#if !defined _DDRAWERROR_HPP_
#define _DDRAWERROR_HPP_
//==============================================================================
//
//  DDraw errors   
//
//==============================================================================
//
//  externals: 

    #include <ddraw.h>
    
    #include "Exception.hpp"
    #include "Win32error.hpp"

namespace core
{
//------------------------------------------------------------------------------
//
//  publics:

std::string
ddraw_error( HRESULT code );


//==============================================================================




class
DDrawError
  : public Error  
{
    
  public:
    
                                DDrawError( HRESULT code, 
                                            const std::string& where =
                                                  std::string("")
                                          );

    virtual const std::string&  description() const;
    virtual const std::string&  comment() const;

};


//------------------------------------------------------------------------------

inline
DDrawError::DDrawError( HRESULT code, const std::string& where )
  : Error( Msg( "%s (when %s)", ddraw_error(code).c_str(), where.c_str() ) )
{   
    comment_text = std::string( "DX error" );
}


//------------------------------------------------------------------------------

inline const std::string& 
DDrawError::description() const
{
    return text;
}


//------------------------------------------------------------------------------

inline const std::string& 
DDrawError::comment() const
{
    return comment_text;
}



//==============================================================================

inline void
try_ddraw( HRESULT result, const std::string& where = std::string("") )
{
    if( result != DD_OK )
        throw DDrawError( result, where );
}

} // namespace core
#endif // _DDRAWERROR_HPP_