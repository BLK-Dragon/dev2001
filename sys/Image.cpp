//==============================================================================
//
//  Image loader realization
//
//==============================================================================
//
//  externals: 

    #include "Image.hpp"
    #include "ImageFormat.hpp"

    #include <iostream>
    #include <fstream>

namespace core
{
//------------------------------------------------------------------------------
//
//  publics:


Image::Image( BinFile* file )
  : raw(0),
    surface(0)
{   
    TGAheader   header;
    UByte*      colormap    = 0;

    header.id_length        = file->read_byte();
    header.colormap_type    = file->read_byte();
    header.image_type       = file->read_byte();
    header.colormap_index   = file->read_word();
    header.colormap_length  = file->read_word();
    header.colormap_size    = file->read_byte();
    header.x_origin         = file->read_word();
    header.y_origin         = file->read_word();
    header.width            = file->read_word();
    header.height           = file->read_word();
    header.bpp              = file->read_byte();
    header.descriptor       = file->read_byte();

    
    file->skip( header.id_length );

    if( (header.colormap_type&0x01)  &&  
        header.image_type==TGAheader::mapped
      )
    {
        // load palette

        if( header.colormap_size != 24 )
        {    
            throw ImageError( Msg( "unsupported colormap bpp (%i)",
                                   header.colormap_size
                                 ) 
                            );
        }

        colormap = new UByte[256*3];
        int i    = 0;

        while( i<256  &&  i<header.colormap_length )
        {
            colormap[i*3+0] = file->read_byte();
            colormap[i*3+1] = file->read_byte();
            colormap[i*3+2] = file->read_byte();
            ++i;
        }
    }



    if( header.bpp != 32  &&  header.bpp != 24  && header.bpp != 8 )
        throw ImageError( Msg( "unsupported bpp (%i)", header.bpp ) );

    UByte*      file_img    = 0;
    UByte*      img         = 0;

    if( header.bpp==8  &&  !(header.colormap_type&0x01) )
    {
        raw = new UByte[header.width*header.height];
        img = raw;
    }
    else
    {
        file_img = new UByte[header.width*header.height*4];
        img      = file_img;
    }

    if( !img )
        throw ImageError("can't alloc mem for image buffer");

    
    int     pixel_size      = (header.bpp==8  &&  !(header.colormap_type&0x01))
                              ? 1  
                              : 4;
    bool    right_to_left   = (header.descriptor & TGAheader::right_to_left);
    bool    bottom_to_top   = !(header.descriptor & TGAheader::top_to_bottom);
    int     scanline_len    = header.width * pixel_size;
    int     scanline_step   = (bottom_to_top)  ? -2*scanline_len  : 0;
    int     pixel_step      = (right_to_left)  ? -pixel_size  : pixel_size;

    scanline_step += (right_to_left)  
                     ? scanline_len - pixel_size
                     : 0;

    img += (bottom_to_top)  
           ? scanline_len * ( header.height - 1 )  
           : 0;

    img += (right_to_left)
           ? scanline_len - pixel_size
           : 0;

    
    for( int l=0; l<header.height; l++ )
    {
        if( header.bpp == 8 )
        {
            if( header.colormap_type&0x01 )
            {
                // paletted image 
                for( int c=0; c<header.width; c++ )
                {
                    int i  = file->read_byte();
                    
                    img[0] = colormap[ i*3 + 0 ];
                    img[1] = colormap[ i*3 + 1 ];
                    img[2] = colormap[ i*3 + 2 ];
                    img[3] = 0;

                    img += pixel_step;

                }
            }
            else // if( header.colormap_type&0x01 )
            {
                // grayscale image

                *img  = file->read_byte();
                img  += pixel_step;
            }
        }
        else // bpp =24 or =32
        {
            for( int c=0; c<header.width; c++ )
            {
                img[0] = file->read_byte();
                img[1] = file->read_byte();
                img[2] = file->read_byte();
                img[3] = ( header.bpp == 32 )  ? file->read_byte()  : 0;

                img += 4;//pixel_step;
            }
        }

        img += scanline_step;
    }
    
    if( !(header.bpp == 8  &&  !(header.colormap_type&0x01)) )
    {
        surface = new Surface( header.width, header.height, file_img,
                               Surface::SysMem
                             );
    }

    
    // read dev-area

    file->to_end();
    file->back( 26 );

    TGAfooter   footer;

    footer.extension_area_offset = file->read_dword();
    footer.developer_area_offset = file->read_dword();
    file->read_bytes( 16, (UByte*)&footer.signature );
    footer.dot  = file->read_byte();
    footer.null = file->read_byte();

    if( strncmp( (const char*)footer.signature, "TRUEVISION-XFILE", 16 )  ||
        footer.dot != '.'  ||  footer.null != 0
      )
    {
        throw ImageError("file is not a TrueVision Targa");
    }

    if( footer.developer_area_offset )
    {
        file->to_pos( footer.developer_area_offset );

        this->n_images      = file->read_dword();
        UDword  key_color   = file->read_dword();
        UByte*  c           = (UByte*)&key_color;

        if( surface )
            surface->set_src_colorkey( c[0], c[1], c[2] );
    }
    else
    {
        n_images = 1; 
    }
    

    if( file_img )  delete[] file_img;
    if( colormap )  delete[] colormap;
}


//------------------------------------------------------------------------------

Image::~Image()
{
    delete surface; 
}


} // namespace core