#if !defined _MSG_HPP_
#define _MSG_HPP_
//==============================================================================
//
//  Msg class   
//
//==============================================================================
//
//  externals: 

    #include <string>

namespace core
{
//==================================================================
//            
//  publics:




class
Msg  
{
  public:

                                Msg( const char* format, ... );

    const std::string           text() const;


  private:

    char                        txt[512];
};


//------------------------------------------------------------------------------

inline
Msg::Msg( const char* format, ... )
{
    va_list     arglist;

    va_start( arglist, format );
    vsprintf( txt, format, arglist );
    va_end( arglist );
}


//------------------------------------------------------------------------------

inline const std::string
Msg::text() const
{
    return std::string( txt ); 
}

} // core
#endif // _MSG_HPP_

