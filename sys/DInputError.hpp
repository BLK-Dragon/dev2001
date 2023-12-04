#if !defined _DINPUTERROR_HPP_
#define _DINPUTERROR_HPP_
//==============================================================================
//
//  DDraw errors   
//
//==============================================================================
//
//  externals: 

    #undef DIRECTINPUT_VERSION
    #define DIRECTINPUT_VERSION 0x0300
    #include <dinput.h>
    
    #include "Exception.hpp"
    #include "Win32error.hpp"

namespace core
{
//------------------------------------------------------------------------------
//
//  publics:




class
DInputError
  : public Error  
{
    
  public:
    
                                DInputError( HRESULT code, 
                                             const std::string& where =
                                                   std::string("")
                                           );

    virtual const std::string&  description() const;
    virtual const std::string&  comment() const;

};


//------------------------------------------------------------------------------

inline
DInputError::DInputError( HRESULT code, const std::string& where )
  : Error( Msg( "%s (when %s)", win32_error(code).c_str(), where.c_str() ) )
{   
    comment_text = std::string( "DInput error" );
}


//------------------------------------------------------------------------------

inline const std::string& 
DInputError::description() const
{
    return text;
}


//------------------------------------------------------------------------------

inline const std::string& 
DInputError::comment() const
{
    return comment_text;
}



//==============================================================================

inline void
try_dinput( HRESULT result, const std::string& where = std::string("") )
{
    if( result != DI_OK  &&  result != S_FALSE )
        throw DInputError( result, where );
}


} // namespace core
#endif // _DINPUTERROR_HPP_