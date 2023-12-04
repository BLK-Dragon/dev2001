//==============================================================================
//
//  File realizations   
//
//==============================================================================
//
//  externals: 

    #include "File.hpp"


namespace core
{
//==============================================================================
//
//  publics:

BinFile::BinFile( const std::string& name )
{
    file = fopen( name.c_str(), "rb" );

    if( !file )
        throw FileError( core::Msg("can't open \"%s\"", name.c_str()) );
}


//------------------------------------------------------------------------------

BinFile::~BinFile()
{
    fclose( file );
}


//------------------------------------------------------------------------------

UByte 
BinFile::read_byte()
{
    UByte   b;

    fread( &b, sizeof(UByte), 1, file );
    if( ferror( file ) )
        throw FileError( core::Msg("error reading byte") );

    return b;
}


//------------------------------------------------------------------------------

UWord 
BinFile::read_word()
{
    UWord   w;

    fread( &w, sizeof(UWord), 1, file );
    if( ferror( file ) )
        throw FileError( core::Msg("error reading word") );

    return w;
}


//------------------------------------------------------------------------------

UDword 
BinFile::read_dword()
{
    UDword   d;

    fread( &d, sizeof(UDword), 1, file );
    if( ferror( file ) )
        throw FileError( core::Msg("error reading dword") );

    return d;
}


//------------------------------------------------------------------------------

void 
BinFile::read_bytes( int n, UByte* buf )
{
    fread( buf, sizeof(UByte), n, file );

    if( ferror( file ) )
        throw FileError( core::Msg("error reading bytes") );
}


//------------------------------------------------------------------------------

void 
BinFile::read_string( int maxbytes, char* buf )
{
    int     bytes_read  = 0;
    char    ch;
    char*   str         = buf;

    while( bytes_read < maxbytes-1 )
    {
        ch = (char)read_byte();
        ++bytes_read;

        if( ch == 0x0D  ||  ch == 0x0A )
        {
            *str++ = '\0';

            ch = (char)read_byte();
            if( ch != 0x0D  &&  ch != 0x0A )
                back(1);

            return;
        }
        else
        {
            *str++ = ch; 
        }
    }
    *( buf + maxbytes - 1 ) = '\0';
    
    if( ferror( file ) )
        throw FileError( core::Msg("error reading bytes") );
}


//------------------------------------------------------------------------------

void 
BinFile::skip( long n )
{
    if( fseek( file, n, SEEK_CUR ) )
        throw FileError( core::Msg("positionin' error") );
}


//------------------------------------------------------------------------------

void 
BinFile::back( long n )
{
    if( fseek( file, -n, SEEK_CUR ) )
        throw FileError( core::Msg("positionin' error") );
}


//------------------------------------------------------------------------------

bool
BinFile::eof()
{
    return feof( file );
}


//------------------------------------------------------------------------------

void    
BinFile::to_end()
{
    if( fseek( file, 0, SEEK_END ) )
        throw FileError( core::Msg("positionin' error") );
}


//------------------------------------------------------------------------------

void    
BinFile::to_begin()
{
    if( fseek( file, 0, SEEK_SET ) )
        throw FileError( core::Msg("positionin' error") );
}


//------------------------------------------------------------------------------

void    
BinFile::to_pos( long p )
{
    if( fseek( file, p, SEEK_SET ) )
        throw FileError( core::Msg("positionin' error") );
}



} // namespace core