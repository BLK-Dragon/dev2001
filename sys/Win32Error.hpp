#if !defined _WIN32ERROR_HPP_
#define _WIN32ERROR_HPP_
//==============================================================================
//
//  Win32 errors   
//
//==============================================================================
//
//  externals: 

    #include <windows.h>
    #include <string.h>

    #include "Exception.hpp"

namespace core
{
//==============================================================================
//
//  publics:

inline std::string
win32_error( long code )
{
    LPVOID  buf;
    char*   str;
    
    FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER |
                   FORMAT_MESSAGE_FROM_SYSTEM |
                   FORMAT_MESSAGE_IGNORE_INSERTS,
                   NULL,
                   code,
                   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                   (LPTSTR)&buf,
                   0,
                   NULL
                 );
    str         = (char*)buf;

    if( str )
    {
        while(true)
        {
            int last    = strlen(str) - 1;
            if( str[last] == '\r'  ||  str[last] == '\n' )
                str[last] = '\0';
            else
                break;
        }
         
        std::string s(str);
        LocalFree( buf );

        return s;
    }
    else
    {
        return std::string(""); 
    }

}




//==============================================================================




class
Win32Error
  : public Error  
{
    
  public:
    
                                Win32Error( const std::string& cmt =
                                                  std::string("win32 error")  
                                          );

    virtual const std::string&  description() const;
    virtual const std::string&  comment() const;
};



//------------------------------------------------------------------------------

inline
Win32Error::Win32Error( const std::string& cmt )
  : Error( core::Msg( win32_error(GetLastError()).c_str() ) )
{
    comment_text    = cmt;
}


//------------------------------------------------------------------------------

inline const std::string&  
Win32Error::description() const
{
    return text;
}


//------------------------------------------------------------------------------

inline const std::string& 
Win32Error::comment() const
{
    return comment_text;
}


//------------------------------------------------------------------------------

inline void
try_win32( DWORD result )
{
    if( result > 0 )
        return;
    
    DWORD   code = GetLastError();
    LPVOID  buf;
    char*   str;

    if( code == ERROR_SUCCESS )
        return;

    FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER |
                   FORMAT_MESSAGE_FROM_SYSTEM |
                   FORMAT_MESSAGE_IGNORE_INSERTS,
                   NULL,
                   code,
                   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                   (LPTSTR)&buf,
                   0,
                   NULL
                 );
    str         = (char*)buf;
    
    while(true)
    {
        int last    = strlen(str) - 1;
        if( str[last] == '\r'  ||  str[last] == '\n' )
            str[last] = '\0';
        else
            break;
    }
    throw Win32Error( std::string(str) );    
    
    //LocalFree( buf );
}

} // namespace core
#endif // _WIN32ERROR_HPP_