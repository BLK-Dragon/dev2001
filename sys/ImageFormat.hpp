#if !defined _IMAGEFORMAT_HPP_
#define _IMAGEFORMAT_HPP_
//==============================================================================
//
//  Image formats   
//
//==============================================================================
//
//  externals: 

    #include "Core.hpp"
    using core::UByte;
    using core::UWord;
    using core::UDword;

//------------------------------------------------------------------------------
//
//  publics:




struct
TGAheader  
{
    UByte   id_length;
    UByte   colormap_type;
    
    enum
    {
            mapped      = 1,
            color       = 2,
            gray        = 3,
            mapped_rle  = 9,
            color_rle   = 10,
            gray_rle    = 11
                 
    };
    UByte   image_type;

    UWord   colormap_index;
    UWord   colormap_length;
    UByte   colormap_size;

    UWord   x_origin;
    UWord   y_origin;

    UWord   width;
    UWord   height;
    UByte   bpp;

    enum
    {
            right_to_left = 0x10,
            top_to_bottom = 0x20     
    };
    UByte   descriptor;
};


//------------------------------------------------------------------------------




struct
TGAfooter  
{
    UDword  extension_area_offset;
    UDword  developer_area_offset;

    UByte   signature[16];          // must be "TRUEVISION-XFILE"
    UByte   dot;                    // must be '.'
    UByte   null;                   // must be 0x00
};








#endif // _IMAGEFORMAT_HPP_