#if !defined _NEBULA_HPP_
#define _NEBULA_HPP_
//==============================================================================
//
//  Nebula rasterizer decl
//
//==============================================================================
//
//  externals: 

    #include <vector>
    
    #include "Rasterizer.hpp"
    //#include "GlareStar.hpp"
    #include "..\..\sys\PixelShader.hpp"
    #include "..\..\sys\Vector.hpp"
    #include "..\..\sys\ParamSet.hpp"


//==============================================================================
//
//  publics:




class
NebulaRasterizer
  : public Rasterizer  
{
  public:

                        NebulaRasterizer();
    virtual            ~NebulaRasterizer();

    virtual void        generate_scanline( core::UDword* scanline, int width );
    virtual void        advance_to_next_scanline();

    virtual void        activate( int x_pos, int n=0 );
    
    static void         register_paramset( core::ParamSetManager* manager );
  
  
  private:
    
    void                generate_polygon( std::vector<Vector2i>* vec );
    
    static void                     bind_members( core::ParamSetPool* pool );

    static std::string              paramset_name;
    static core::ParamSetManager*   paramset_manager;

    
    void            advance_l_index();
    void            advance_r_index();
    float           fade( float x );
    
    
    std::vector<Vector2i>       vertex;

    int                         x_pos;
    int                         l_cur_index;
    int                         r_cur_index;
    float                       l_dx;
    float                       r_dx;
    int                         l_sec_height;
    int                         r_sec_height;
    float                       l_x;
    float                       r_x;
    vfx::PerlinNoiseShaderSet*  shader0;
    vfx::PerlinNoiseShaderAdd*  shader1;
    vfx::PerlinNoiseShaderAdd*  shader2;
    int                         bound_width;
    int                         bound_height;
    int                         center_x;
    int                         center_y;
    int                         cur_y;
    int                         top_y;

    
    /*
    std::vector<GlareStarRasterizer>    glare;
    float                               glare_density;
    int                                 glare_max_r;
    core::UDword                        glare_color;
    */
};


//------------------------------------------------------------------------------

inline float
NebulaRasterizer::fade( float x )
{
    const float     b = 0.8;

    if( x < b )
    {
        return (x/b) * (x/b);
    }
    else
    {
        return 1.0;
    }
}

#endif // _NEBULA_HPP_