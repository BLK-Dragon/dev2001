#if !defined _STAR_HPP_
#define _STAR_HPP_
//==============================================================================
//
//  Star rasterizer decl   
//
//==============================================================================
//
//  externals:

    #include "Rasterizer.hpp"
    #include "..\..\sys\BitOps.hpp"


//==============================================================================
//
//  publics:




class
StarRasterizer
  : public Rasterizer
{
  public:
                    StarRasterizer();
                   ~StarRasterizer();


    virtual void    activate( int x_pos, int n=0 );
    virtual void    generate_scanline( core::UDword* scanline, int width );
    virtual void    advance_to_next_scanline();

    void            scale_intensity( float s );
    
    static void     register_paramset( core::ParamSetManager* manager );

  
  private:
    
    static void                     bind_members( core::ParamSetPool* pool );
    
    static std::string              paramset_name;
    static core::ParamSetManager*   paramset_manager;
    core::ParamSet                  cur_paramset;

    
    int                         x_center;
    int                         dy;
    int                         star_radius;
    core::UDword                color;
};


//------------------------------------------------------------------------------

inline void            
StarRasterizer::scale_intensity( float s )
{
    vfx::scale_color( &color, s );
}

#endif // _STAR_HPP_