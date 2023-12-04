#if !defined _ZIPFILE_HPP_
#define _ZIPFILE_HPP_
//==============================================================================
//
//  ZIP file decls   
//
//==============================================================================
//
//  externals: 

    #include "File.hpp"

namespace core
{
//==============================================================================
//
//  publics




struct
CentralDirFooter
{
    UDword  signature;
    UWord   disk_number;
    UWord   start_disk_number;
    UWord   num_entries_in_this_disk;
    UWord   num_entries;
    UDword  dir_size;
    UDword  offset_to_start;
    UWord   comment_len;

};


//------------------------------------------------------------------------------




struct
LocalFileHeader  
{
    UDword  signature;
    UWord   version_to_extract;
    UWord   general_purpose_flags;
    UWord   compression_method;
    UWord   last_mod_time;
    UWord   last_mod_date;
    UDword  crc32;
    UDword  compressed_size;
    UDword  uncompressed_size;
    UWord   filename_length;
    UWord   extrafield_length;

    enum
    {
            stored      = 0,
            shrunk      = 1,
            reduced1    = 2,
            reduced2    = 3,
            reduced3    = 4,
            reduced4    = 5,
            imploded    = 6,
            tokenized   = 7,
            deflated    = 8,
            deflated64  = 9,
            pkwareDCL   = 10
    };
};


//==============================================================================




class
ZipBinFile
  : public BinFile  
{
  public:

                    ZipBinFile( const std::string& name );
                   ~ZipBinFile();

    void            read_file( const std::string& name );

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

  
  private:

    UByte*          file_image;
    UByte*          image_ptr;
    long            image_size;
};


//------------------------------------------------------------------------------

inline UByte 
ZipBinFile::read_byte()
{
    return *image_ptr++;
}


//------------------------------------------------------------------------------

inline UWord 
ZipBinFile::read_word()
{
    UDword  w = *( (UWord*)image_ptr );

    image_ptr += sizeof(UWord);
    return w;
}


//------------------------------------------------------------------------------

inline UDword 
ZipBinFile::read_dword()
{
    UDword  dw = *( (UDword*)image_ptr );

    image_ptr += sizeof(UDword);
    return dw;
}


//------------------------------------------------------------------------------

inline void 
ZipBinFile::read_bytes( int n, UByte* buf )
{
    for( int i=0; i<n; i++ )
        *buf++ = *image_ptr++;
}


//------------------------------------------------------------------------------

inline void 
ZipBinFile::skip( long n )
{
    image_ptr += n;
}


//------------------------------------------------------------------------------

inline void 
ZipBinFile::back( long n )
{
    image_ptr -= n;
}


//------------------------------------------------------------------------------

inline bool 
ZipBinFile::eof()
{
    return image_ptr >= file_image + image_size;
}


//------------------------------------------------------------------------------

inline void 
ZipBinFile::to_end()
{
    image_ptr = file_image + image_size-1;
}


//------------------------------------------------------------------------------

inline void 
ZipBinFile::to_begin()
{
    image_ptr = file_image;
}


//------------------------------------------------------------------------------

inline void 
ZipBinFile::to_pos( long p )
{
    image_ptr = file_image + p;
}



} // namespace core

#endif // _ZIPFILE_HPP_