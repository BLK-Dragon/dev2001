#if !defined _FILE_HPP_
#define _FILE_HPP_
//==============================================================================
//
//  decls of classes for file ops   
//
//==============================================================================
//
//  externals:

    #include "Core.hpp"

    #include <stdio.h>


namespace core
{
//==============================================================================
//
//  publics:




class
BinFile  
{
  public:

                    BinFile( const std::string& name );
                   ~BinFile();

    virtual UByte   read_byte();
    virtual UWord   read_word();
    virtual UDword  read_dword();
    virtual void    read_bytes( int n, UByte* buf );
    virtual void    read_string( int maxbytes, char* buf );

    virtual void    skip( long n );
    virtual void    back( long n );

    virtual bool    eof();
    virtual void    to_end();
    virtual void    to_begin();
    virtual void    to_pos( long p );


  protected:

    FILE*           file;
};


//==============================================================================




class
FileError
  : public core::Error  
{

  public:

                        FileError( const core::Msg& msg );
};


//------------------------------------------------------------------------------

inline
FileError::FileError( const core::Msg& msg )
  : Error( msg )
{
    comment_text = std::string( "file error" );
}

} // namespace sys
#endif // _FILE_HPP_
