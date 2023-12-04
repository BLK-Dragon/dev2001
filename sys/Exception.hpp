#if !defined _EXCEPTION_HPP_
#define _EXCEPTION_HPP_
//==============================================================================
//
//  Exceptions   
//
//==============================================================================
//
//  externals:

    #include <stdlib.h>
    #include <stdarg.h>
    #include <string>
    #include <iostream>
    #include <iomanip>

    #include <ddraw.h>
    #include <dinput.h>

    #include "Msg.hpp"


namespace core
{
//==============================================================================
//
//  publics:




class
Error  
{
  public:

                                Error( const Msg&, 
                                       const std::string& desc="generic error" 
                                     );

    virtual const std::string&  description() const;
    virtual const std::string&  comment() const;


  protected:

    std::string                 text;
    std::string                 comment_text;
};


//------------------------------------------------------------------------------

inline
Error::Error( const Msg& msg, const std::string& desc )
{
    text          = msg.text();
    comment_text  = desc;
}


//------------------------------------------------------------------------------

inline const std::string&
Error::description() const
{
    return text; 
}


//------------------------------------------------------------------------------

inline const std::string&
Error::comment() const
{
    return comment_text; 
}

} // namespace core
#endif // _EXCEPTION_HPP_                                                        