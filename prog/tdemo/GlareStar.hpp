#if !defined _GLARESTAR_HPP_
#define _GLARESTAR_HPP_
//==============================================================================
//
//  GlareStar rasterizer decl   
//
//==============================================================================
//
//  externals:

    #include "Rasterizer.hpp"
    //#include "..\..\sys\Vector2i.hpp"


//==============================================================================
//
//  publics:




class
GlareStarRasterizer
  : public Rasterizer
{
  public:
                    GlareStarRasterizer();
                   ~GlareStarRasterizer();


    virtual void    activate( int x_pos, int n=0 );
    virtual void    generate_scanline( core::UDword* scanline, int width );
    virtual void    advance_to_next_scanline();
    static void     register_paramset( core::ParamSetManager* manager );

  
  private:

    static void                     bind_members( core::ParamSetPool* pool );
    static std::string              paramset_name;
    static core::ParamSetManager*   paramset_manager;
    core::ParamSet                  cur_paramset;


    int                             x_center;
    int                             dy;
    int                             star_radius;
    int                             ray_start;
    core::UDword                    color;
    
    static bool                     first_creation;
    static core::UByte              ray[512];
//    static float                    sqrt_value[50000];
};


#endif // _GLARESTAR_HPP_