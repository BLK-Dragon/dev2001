#if !defined _STARFIELD_HPP_
#define _STARFIELD_HPP_
//==============================================================================
//
//  StarField rasterizer decl   
//
//==============================================================================
//
//  externals: 

    //#include "..\..\sys\Vector.hpp"
    #include "..\..\sys\NoiseLayer.hpp"
    
    //#include "GlareStar.hpp"
    #include "Star.hpp"


//==============================================================================
//
//  pblics:




class
StarFieldRasterizer  
  : public Rasterizer
{
  public:
                        StarFieldRasterizer();
                       ~StarFieldRasterizer();

    virtual void        generate_scanline( core::UDword* scanline, int width );
    virtual void        advance_to_next_scanline();
    
    virtual void        activate( int x_pos, int n=0 );

    static void         register_paramset( core::ParamSetManager* manager );
    
  
  private:
                                               
    static void                     bind_members( core::ParamSetPool* pool );
    
    static std::string              paramset_name;
    static core::ParamSetManager*   paramset_manager;
    core::ParamSet              cur_paramset;
    std::vector<StarRasterizer> star;
    std::vector<int>            star_variant;


    enum
    {
        max_noise_layers        = 8,
        num_xwarp_points        = 4
    };
    vfx::GradNoiseLayer*        noise_layer[max_noise_layers];
    std::vector<core::UDword>   star_color;
    
    double                      star_density;
    double                      star_base_intensity;
    double                      star_max_intensity;
    
    double                      bigstar_threshold;
    double                      bigstar_density;
    int                         bigstar_line;
    int                         bigstar_lines_to_skip;
    double                      bigstar_base_intensity;
    double                      bigstar_max_intensity;

    int                         num_layers;
    
    // chaos related

    Vector2f                    layer0_u, layer0_v;
    Vector2f                    layer0_du, layer0_dv;
    int                         layer0_change_ttw;
    float                       layer0_su, layer0_sv;
    float                       layer0_dsu, layer0_dsv;
    int                         layer0_scale_ttw;



    struct
    {
        float   x_pos;
        float   dx;
        float   scale;          // Ux scale coefficient
        int     change_ttw;
    }                           xwarp_info[num_xwarp_points];
    bool                        first_warp;
};



#endif // _STARFIELD_HPP_