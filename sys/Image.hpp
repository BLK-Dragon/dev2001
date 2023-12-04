#if !defined _IMAGE_HPP_
#define _IMAGE_HPP_
//==============================================================================
//
//  Image decl   
//
//==============================================================================
//
//  externals:

    #include "File.hpp"
    #include "ImageFormat.hpp" 
    #include "Surface.hpp"

namespace core
{
//==============================================================================
//
//  publics:




class
Image  
{
  public:

                    Image( BinFile* file );
                   ~Image();

    Surface&        image();
//    const UByte*    alpha_channel() const;
    const UByte*    raw_data() const;
    int             num_images() const;

    int             width() const;
    int             height() const;
    int             bpp() const;


  private:
    Surface*        surface;
    int             n_images;
    
    int             raw_bpp;
    UByte*          raw;
};


//==============================================================================




class
ImageError
  : public Error  
{
    
  public:

                        ImageError( const Msg& msg );
};


//------------------------------------------------------------------------------

inline Surface&        
Image::image()
{
    if( !surface )
        throw ImageError(Msg("null surface"));
    
    return *surface; 
}


//------------------------------------------------------------------------------

inline const UByte*
Image::raw_data() const
{
    return raw;
}


//------------------------------------------------------------------------------

inline int             
Image::num_images() const
{
    return n_images;
}


//------------------------------------------------------------------------------

inline int             
Image::width() const
{
    return surface->width();
}


//------------------------------------------------------------------------------

inline int             
Image::height() const
{
    return surface->height() / n_images;
}


//------------------------------------------------------------------------------

inline int
Image::bpp() const
{
    return (surface) ? surface->bpp()  : raw_bpp;
}


//==============================================================================


inline
ImageError::ImageError( const Msg& msg )
  : Error( msg )
{
    comment_text = "image error";     
}

} // namespace core
#endif // _IMAGE_HPP_