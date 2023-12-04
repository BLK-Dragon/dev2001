#if !defined _RENDER2D_HPP_
#define _RENDER2D_HPP_
//==============================================================================
//
//  2Drender stuff decls   
//
//==============================================================================
//
//  externals: 

    #include "Surface.hpp"
    #include "Fill.hpp"

namespace vfx
{
//==============================================================================
//
//  publics:




class
Render2D  
{
  public:
                Render2D( core::Surface& surf );

    void        bind_surface(  core::Surface& surf  );

    
    void        h_line( int x, int y, int len, const Fill1D& fill );
    void        h_line_nc( int x, int y, int len, const Fill1D& fill );
    void        h_line_c( int x, int y, int len, const Fill1D& fill );
    
    void        v_line( int x, int y, int hgt, const Fill1D& fill );
    void        v_line_nc( int x, int y, int hgt, const Fill1D& fill );
    void        v_line_c( int x, int y, int hgt, const Fill1D& fill );

    void        enable_clipping();
    void        disable_clipping();

  
  private:
    core::Surface*      surface;
    
    void        (Render2D::*h_line_ptr)(int,int,int,const Fill1D&);
    void        (Render2D::*v_line_ptr)(int,int,int,const Fill1D&);

};


//------------------------------------------------------------------------------

inline
Render2D::Render2D( core::Surface& surf )
  : surface( &surf )
{
    disable_clipping();
}


//------------------------------------------------------------------------------

inline void 
Render2D::bind_surface( core::Surface& surf  )
{
    surface = &surf;
}


//------------------------------------------------------------------------------

inline void        
Render2D::enable_clipping()
{
    h_line_ptr = h_line_c; 
    v_line_ptr = v_line_c; 
}


//------------------------------------------------------------------------------

inline void        
Render2D::disable_clipping()
{
    h_line_ptr = h_line_nc;
    v_line_ptr = v_line_nc;
}


//------------------------------------------------------------------------------
//
// H-lines

inline void        
Render2D::h_line( int x, int y, int len, const Fill1D& fill )
{
    (this->*h_line_ptr)( x, y, len, fill );
}


//------------------------------------------------------------------------------

inline void        
Render2D::h_line_nc( int x, int y, int len, const Fill1D& fill )
{
    core::UByte*    mem     = surface->memory();
    core::UDword    color   = surface->native_color( fill.value_at(0) );
    int             bpp     = surface->bpp() / 8;

    mem += y * surface->pitch() + x * bpp;
    
    switch( bpp )
    {
        case 2 :
        {     
            core::UWord*    ptr = (core::UWord*)mem;
            
            for( int i=0; i<len; i++ )
                *ptr++ = (core::UWord)color;
        }   break;
        
        case 4 :
        {
            core::UDword*   ptr = (core::UDword*)mem;

            for( int i=0; i<len; i++ )
                *ptr++ = color;
        }   break;
    }

}


//------------------------------------------------------------------------------

inline void        
Render2D::h_line_c( int x, int y, int len, const Fill1D& fill )
{
    if( y < 0  ||  y > surface->height()-1 )
        return;
    
    if( x < 0 )
    {
        len += x;
        x    = 0; 
    }

    int dx = surface->width()-1 - (x+len);

    if( dx < 0 )
        len += dx;

    h_line_nc( x, y, len, fill );
}



//------------------------------------------------------------------------------
//
// V-lines

inline void        
Render2D::v_line( int x, int y, int hgt, const Fill1D& fill )
{
    (this->*v_line_ptr)( x, y, hgt, fill );
}


//------------------------------------------------------------------------------

inline void        
Render2D::v_line_nc( int x, int y, int hgt, const Fill1D& fill )
{
    core::UByte*    mem     = surface->memory();
    core::UDword    color   = surface->native_color( fill.value_at(0) );
    int             bpp     = surface->bpp() / 8;
    int             dm      = surface->pitch();

    mem += y * surface->pitch() + x * bpp;
    
    switch( bpp )
    {
        case 2 :
        {     
            for( int i=0; i<hgt; i++ )
            {
                *((core::UWord*)mem) = (core::UWord)color;
                mem += dm;
            }
        }   break;
        
        case 4 :
        {
            for( int i=0; i<hgt; i++ )
            {
                *((core::UDword*)mem) = color;
                mem += dm;
            }
        }   break;
    }

}


//------------------------------------------------------------------------------

inline void        
Render2D::v_line_c( int x, int y, int hgt, const Fill1D& fill )
{
    if( x < 0  ||  x > surface->width()-1 )
        return;
    
    if( y < 0 )
    {
        hgt += y;
        y    = 0; 
    }

    int dy = surface->height()-1 - (y+hgt);

    if( dy < 0 )
        hgt += dy;

    v_line_nc( x, y, hgt, fill );
}


} // namespace vfx
#endif // _RENDER2D_HPP_