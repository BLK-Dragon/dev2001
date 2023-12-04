#if !defined _PLANET_HPP_
#define _PLANET_HPP_
//==============================================================================
//
//  Planet rasterizer decl   
//
//==============================================================================
//
//  externals: 

    #include "..\..\sys\Vector.hpp"
    #include "..\..\sys\ParamSet.hpp"
    #include "..\..\sys\PixelShader.hpp"
    
    #include "Rasterizer.hpp"



//==============================================================================
//
//  publics:




class
PlanetRasterizer
  : public Rasterizer  
{
  public:

                        PlanetRasterizer( RasterizerPool* rp = 0 );
    virtual            ~PlanetRasterizer();

    virtual void        generate_scanline( core::UDword* scanline, int width );
    virtual void        advance_to_next_scanline();
    
    virtual void        activate( int x_pos, int n=0 );

  
  private:
    static core::ParamSetPool   paramset_pool;
    
    
    int                         x_pos;
    int                         y_pos;
    int                         radius;
    PerlinNoiseShader*          shader0;
    PerlinNoiseShader*          shader1;
    float                       lx,ly,lz;
};





#endif // _PLANET_HPP_