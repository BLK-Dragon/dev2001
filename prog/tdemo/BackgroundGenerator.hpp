#if !defined _BACKGROUNDGENERATOR_HPP_
#define _BACKGROUNDGENERATOR_HPP_
//==============================================================================
//
//  Background generator class decl   
//
//==============================================================================
//
//  externals:

    #include <vector>
    #include "..\..\sys\Type.hpp"

    #include "StarField.hpp"
    


//==============================================================================
//
//  publics:




class
BackgroundGenerator
  : public Rasterizer
{
  public:

                    BackgroundGenerator( int w );
                   ~BackgroundGenerator();

    void            activate( int x_pos, int n=0 );
    void            generate_scanline( core::UDword* scanline, int width );
    void            advance_to_next_scanline();
    
    static void     register_paramset( core::ParamSetManager* manager );

  
  private:

    static void                     bind_members( core::ParamSetPool* pool );

    static std::string              paramset_name;
    static core::ParamSetManager*   paramset_manager;
    core::ParamSet                  cur_paramset;
    
    int             width;
    int             height;
    int             cur_y;

    void            generate_polygon( std::vector<Vector2i>* vertex, Vector2i c );
    void            fractalize_polygon( std::vector<Vector2i>* vertex );
    void            scale_polygon( std::vector<Vector2i>* vertex, float s );
    std::vector<Rasterizer*>    rasterizer;
    //std::vector<Rasterizer*>    active_rasterizer;
    //StarFieldRasterizer*        starfield;

    int             glarestar_delay;
    int             glarestar_ttw;
    //int             planet_ttw;
    int             nebula_delay;
    int             nebula_ttw;
};


#endif // _BACKGROUNDGENERATOR_HPP_