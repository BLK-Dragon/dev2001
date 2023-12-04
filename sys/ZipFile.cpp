//==============================================================================
//
//  Zip file realization   
//
//==============================================================================
//
//  externals: 

    #include "ZipFile.hpp"

namespace core
{
//==============================================================================
//
//  publics:


ZipBinFile::ZipBinFile( const std::string& name )
  : BinFile( name )
{
}


//------------------------------------------------------------------------------

ZipBinFile::~ZipBinFile()
{
}


//------------------------------------------------------------------------------

void            
ZipBinFile::read_file( const std::string& name )
{
    if( file_image )
        delete[] file_image;

    BinFile::to_pos( 0 );
    while( !BinFile::eof() )
    {
        LocalFileHeader header;
        char            filename[256];
        
        header.signature                = BinFile::read_dword();
        header.version_to_extract       = BinFile::read_word();
        header.general_purpose_flags    = BinFile::read_word();
        header.compression_method       = BinFile::read_word();
        header.last_mod_time            = BinFile::read_word();
        header.last_mod_date            = BinFile::read_word();
        header.crc32                    = BinFile::read_dword();
        header.compressed_size          = BinFile::read_dword();
        header.uncompressed_size        = BinFile::read_dword();
        header.filename_length          = BinFile::read_word();
        header.extrafield_length        = BinFile::read_word();


        if( header.signature != 0x04034B50 ) 
        {    
            //throw FileError("corrupted ZIP archive");
            continue;
        }

        BinFile::read_bytes( header.filename_length, filename );
        filename[header.filename_length] = '\0';

        BinFile::skip( header.extrafield_length );

        if( filename != name )
        {
            BinFile::skip( header.compressed_size );
            if( header.general_purpose_flags & 0x8 )
                BinFile::skip( 3*sizeof(UDword) );
            continue;
        }

        if( header.compression_method != LocalFileHeader::stored )
            throw FileError("unsupported compression method");

        
        // read file to memory

        file_image = new UByte[header.compressed_size];
        if( !file_image )
            throw FileError("can't alloc mem for zipped file-image");

        BinFile::read_bytes( header.compressed_size, file_image );
        image_ptr   = file_image;
        image_size  = header.compressed_size;
        return;
    }

    throw FileError( Msg("\"%s\" not found",name.c_str()) );
}


//------------------------------------------------------------------------------

inline void 
ZipBinFile::read_string( int maxbytes, char* buf )
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


} // namspace core