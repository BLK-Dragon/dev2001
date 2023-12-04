#if !defined _SURFACE_HPP_
#define _SURFACE_HPP_
//==============================================================================
//
//  Surface decls   
//
//==============================================================================
//
//  externals: 

    #include <ddraw.h>
    
    #include "Core.hpp"
    #include "Rect.hpp"

namespace core
{
//------------------------------------------------------------------------------
//
//  publics:




class
Surface  
{
    friend                  class VideoCard;
  
  protected:

                            Surface();

  private:

                            Surface(const Surface & s);
    Surface&                operator=( const Surface& s );


  
  public:

    enum
    kind
    {
        VidMem,
        SysMem
    };


                            Surface( int width, int height, kind k=VidMem );
                            Surface( int width, int height, const UByte* mem, 
                                     kind k=VidMem
                                   );

    virtual                ~Surface();

    int                     width() const;
    long                    pitch() const;
    int                     height() const;
    int                     bpp() const;
    bool                    blit_done() const;

    void                    clear();
    UByte*                  memory();

    UDword                  red_mask() const;
    UDword                  green_mask() const;
    UDword                  blue_mask() const;

    
    void                    lock();
    void                    unlock();
    
    void                    blit_to( Surface& dst, const Vector2i& pos );
    void                    blit_to( Surface& dst, const Vector2i& dst_pos,
                                     const Rect2D& src_rect
                                   );
    void                    blit_to( Surface& dst, const Rect2D& dst_rect,
                                     const Rect2D& src_rect
                                   );
    
    void                    set_pixel( int x, int y, UDword color );
    void                    set_pixel( int x, int y, 
                                       UByte r, UByte g, UByte b 
                                     );
    void                    draw_text( int xpos, int ypos, 
                                       UDword color, int height,
                                       const Msg& msg, bool transparent = true
                                     );

    void                    set_src_colorkey( UByte r, UByte g, UByte b );
    UDword                  native_color( UByte r, UByte g, UByte b );
    UDword                  native_color( UDword rgb );

  private:

    DDSURFACEDESC           info;
    DDPIXELFORMAT           format;
    bool                    locked;
    DWORD                   transfer;
    DWORD                   blt_flags;

    static DDPIXELFORMAT    prim_surf_pixelformat;
    UDword                  red_offset;
    UDword                  green_offset;
    UDword                  blue_offset;
    UDword                  red_offset16;
    UDword                  green_offset16;
    UDword                  blue_offset16;
    
  
  protected:
    
    LPDIRECTDRAWSURFACE2    surf2;

    void                    update_info();
};

//------------------------------------------------------------------------------

inline void
Surface::clear()
{
    ::ZeroMemory( memory(), pitch() * height() );
    unlock(); 
}


//------------------------------------------------------------------------------

inline UDword 
Surface::red_mask() const
{
    return format.dwRBitMask;
}


//------------------------------------------------------------------------------

inline UDword 
Surface::green_mask() const
{
    return format.dwGBitMask;
}


//------------------------------------------------------------------------------

inline UDword 
Surface::blue_mask() const
{
    return format.dwBBitMask;
}


//------------------------------------------------------------------------------

inline UDword
Surface::native_color( UByte r, UByte g, UByte b )
{
    UDword  color = 0x00000000;
    
    if( this->bpp() == 32 )
    {
        if     ( red_mask() == 0x0000FF )   color |= ((UDword)r);
        else if( red_mask() == 0x00FF00 )   color |= ((UDword)r)<<8;
        else if( red_mask() == 0xFF0000 )   color |= ((UDword)r)<<16;
        
        if     ( green_mask() == 0x0000FF ) color |= ((UDword)g);
        else if( green_mask() == 0x00FF00 ) color |= ((UDword)g)<<8;
        else if( green_mask() == 0xFF0000 ) color |= ((UDword)g)<<16;
        
        if     ( blue_mask() == 0x0000FF )  color |= ((UDword)b);
        else if( blue_mask() == 0x00FF00 )  color |= ((UDword)b)<<8;
        else if( blue_mask() == 0xFF0000 )  color |= ((UDword)b)<<16;
    }
    else if( this->bpp() == 16 )
    {
        UDword  color16 = 0;
        UDword  c;
        UDword  mask;
        int     bit_offset;
/*
        c           = (UDword)r<<8;
        mask        = red_mask();
        bit_offset  = 0;

        while( !(mask&0x8000) )
        {
            mask <<= 1 ;
            ++bit_offset;
        }
        c       >>= bit_offset;
        color16 |= c & red_mask();
*/        
        color16 |= ( ((UDword)r<<8) >> red_offset ) & red_mask();
/*
        c           = (UDword)g<<8;
        mask        = green_mask();
        bit_offset  = 0;

        while( !(mask&0x8000) )
        {
            mask <<= 1 ;
            ++bit_offset;
        }
        c       >>= bit_offset;
        color16 |= c & green_mask();
*/
        color16 |= ( ((UDword)g<<8) >> green_offset ) & green_mask();
/*        
        c           = (UDword)b<<8;
        mask        = blue_mask();
        bit_offset  = 0;

        while( !(mask&0x8000) )
        {
            mask <<= 1 ;
            ++bit_offset;
        }
        c       >>= bit_offset;
        color16 |= c & blue_mask();

        color = color16;
*/    
        color16 |= ( ((UDword)b<<8) >> blue_offset ) & blue_mask();

        color = color16;
    }

    return color;
}


//------------------------------------------------------------------------------

inline bool     
Surface::blit_done() const
{
    HRESULT hr;
    
    switch( hr=surf2->GetFlipStatus( DDGFS_ISFLIPDONE ) )
    {
        case DD_OK :
            return true;
            break;

        case DDERR_WASSTILLDRAWING :
            return false; 
            break;
        
        default:                     
            throw DDrawError( hr __CT("Surface::blit_done") ); 
    }
    
    return false;
}


//==============================================================================




class
ScreenBuffer
  : public Surface 
{
  
  public:

    enum
    FlipMethod
    {
        no_vsync    = 1,
        vsync       = 2
    };
                            ScreenBuffer();

    void                    flip( FlipMethod method = vsync );

  
  private:

    void                    assign( LPDIRECTDRAWSURFACE2 s );
};


} // namespace core
#endif // _SURFACE_HPP_