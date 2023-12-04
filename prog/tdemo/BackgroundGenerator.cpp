//==============================================================================
//
//  Background generator realization   
//
//==============================================================================
//
//  externals: 

    #include <iostream>
    using std::flush;
    #include <math.h>
    
    #include "BackgroundGenerator.hpp"
    #include "GlareStar.hpp"
    #include "Nebula.hpp"
    //#include "Planet.hpp"
    #include "StarField.hpp"
    
    #include "..\..\sys\Random.hpp"
    #include "..\..\sys\BitOps.hpp"


//==============================================================================
//
//  publics:

//core::ParamSetPool* BackgroundGenerator::paramset_pool = 0;
core::ParamSetManager*   BackgroundGenerator::paramset_manager  = 0;
std::string              BackgroundGenerator::paramset_name     = "background";


//------------------------------------------------------------------------------

BackgroundGenerator::BackgroundGenerator( int w )
  : width(w),
    cur_y(0)
{
}


//------------------------------------------------------------------------------

BackgroundGenerator::~BackgroundGenerator()
{
    for( int i=0; i<rasterizer.size(); i++ )
        delete rasterizer[i];
}


//------------------------------------------------------------------------------

void
BackgroundGenerator::activate( int x_pos, int n )
{
    int i;

    rasterizer.clear();
    rasterizer.push_back( new StarFieldRasterizer );

    cur_paramset = paramset_manager->paramset( paramset_name, n );

   

    /*
    for( i=0; i<5; i++ )
    {    
        PlanetRasterizer*   p = new PlanetRasterizer;
        rasterizer.push_back( p );
        p->bind_to_parser( &parser );
    }
    */ 
    nebula_delay = cur_paramset["nebula_delay"];
    for( i=0; i<15; i++ )
        rasterizer.push_back( new NebulaRasterizer );
         
    glarestar_delay = cur_paramset["glarestar_delay"];
    for( i=0; i<25; i++ )
        rasterizer.push_back( new GlareStarRasterizer );
                                  
    //glarestar_ttw   = cur_paramset["glarestar_ttw"];
    //planet_ttw      = cur_paramset["planet_ttw"];
    nebula_ttw      = nebula_delay;
    glarestar_ttw   = glarestar_delay;

    rasterizer[0]->activate( 0, 2 );
}


//------------------------------------------------------------------------------

void 
BackgroundGenerator::generate_scanline( core::UDword* line, int width )
{   
    
    for( int i=0; i<rasterizer.size(); i++ )
    {
        if( rasterizer[i]->is_active() )
            rasterizer[i]->generate_scanline( line, this->width );
    }
    
    //core::UDword    b = 50 + vfx::rnd(200);
    //*(line+5) = (b<<16) | (b<<8) | b;
}


//------------------------------------------------------------------------------

void 
BackgroundGenerator::advance_to_next_scanline()
{
    for( int i=0; i<rasterizer.size(); i++ )
    {
        if( rasterizer[i]->is_active() )
            rasterizer[i]->advance_to_next_scanline();
    }
    /*
    if( --planet_ttw == 0 )
    {
        PlanetRasterizer*    p;
        for( int i=0; i<rasterizer.size(); i++ )
        {
            if( !( p = dynamic_cast<PlanetRasterizer*>(rasterizer[i]) ) )
                continue;

            if( p->is_active() )
                continue;

            p->activate( vfx::rnd( this->width ) );
            planet_ttw  = paramset_pool[0]["planet_ttw"];
            planet_ttw *= float( 1 + float(vfx::rnd(20))/100.0 );
            break;
        }
    }
    */
    if( --nebula_ttw == 0 )
    {
        NebulaRasterizer*    n;
        for( int i=0; i<rasterizer.size(); i++ )
        {
            if( !( n = dynamic_cast<NebulaRasterizer*>(rasterizer[i]) ) )
                continue;

            if( n->is_active() )
                continue;

            int v = paramset_manager->paramset_count( "nebula" );
            n->activate( vfx::rnd( this->width ), vfx::rnd(v) );
            nebula_ttw  = nebula_delay;
            nebula_ttw *= float( 1 + float(vfx::rnd(20))/100.0 );
            break;
        }
    }   
    
    if( --glarestar_ttw == 0 )
    {
        GlareStarRasterizer*    gs;
        for( int i=0; i<rasterizer.size(); i++ )
        {
            if( !( gs = dynamic_cast<GlareStarRasterizer*>(rasterizer[i]) ) )
                continue;

            if( gs->is_active() )
                continue;

            int v = paramset_manager->paramset_count( "glarestar" );
            gs->activate( vfx::rnd( this->width ), vfx::rnd(v) );
            glarestar_ttw   = glarestar_delay;
            glarestar_ttw  *= float( 1 + float(vfx::rnd(20))/100.0 );
            break;
        }
    } 
    
    
    //std::clog << "cur_y = " << cur_y << "\n" << std::flush;
    
    /*
    std::vector<Rasterizer*>::iterator   p;
    
    for( p=active_rasterizer.begin(); p<active_rasterizer.end(); p++ )
    {    
        (*p)->advance_to_next_scanline();
    }
    
    starfield->advance_to_next_scanline();
    
    ++cur_y;
    vfx::randomize( GetTickCount(), 
                    GetTickCount(), 
                    GetTickCount() 
                  );
    */
}


//------------------------------------------------------------------------------

void         
BackgroundGenerator::register_paramset( core::ParamSetManager* manager )
{
    paramset_manager = manager;
    manager->register_paramset( paramset_name, bind_members );

    StarFieldRasterizer::register_paramset( manager );
    NebulaRasterizer::register_paramset( manager );
    GlareStarRasterizer::register_paramset( manager );
}


//------------------------------------------------------------------------------

void                 
BackgroundGenerator::bind_members( core::ParamSetPool* pool )
{
    pool->start_member_binding();
        pool->bind_int_member("glarestar_delay");
        pool->bind_int_member("nebula_delay");
//        pool->bind_int_member("planet_ttw");
    pool->finish_member_binding();
}


//------------------------------------------------------------------------------

void                        
BackgroundGenerator::generate_polygon( std::vector<Vector2i>* vertex, Vector2i c )
{
    vertex->clear();

    int t = 1+vfx::rnd(5);
    //int t = 5;

    if( t==1 )
    { 
        vertex->push_back( Vector2i( 103 -147, -(36 -174) ) );
        vertex->push_back( Vector2i( 109 -147, -(69 -174) ) );
        vertex->push_back( Vector2i( 200 -147, -(141 -174) ) );
        vertex->push_back( Vector2i( 229 -147, -(190 -174) ) );
        vertex->push_back( Vector2i( 224 -147, -(258 -174) ) );
        vertex->push_back( Vector2i( 123 -147, -(309 -174) ) );
        vertex->push_back( Vector2i( 37 -147,  -(282 -174) ) );
        vertex->push_back( Vector2i( 62 -147,  -(203 -174) ) );
        vertex->push_back( Vector2i( 98 -147,  -(162 -174) ) );
        vertex->push_back( Vector2i( 87 -147,  -(112 -174) ) );
        vertex->push_back( Vector2i( 49 -147,  -(66 -174) ) );
    }
    else if( t==2 )
    {
        vertex->push_back( Vector2i( 252 -190, -(42 -32) ) );
        vertex->push_back( Vector2i( 287 -190, -(125 -32) ) );
        vertex->push_back( Vector2i( 243 -190, -(209 -32) ) );
        vertex->push_back( Vector2i( 109 -190, -(196 -32) ) );
        vertex->push_back( Vector2i( 83 -190,  -(139 -32) ) );
        vertex->push_back( Vector2i( 121 -190, -(121 -32) ) );
        vertex->push_back( Vector2i( 160 -190, -(62 -32) ) );
    }
    else if( t==3 )
    {
        vertex->push_back( Vector2i( 122 -115, -(31 -151) ) );
        vertex->push_back( Vector2i( 178 -115, -(41 -151) ) );
        vertex->push_back( Vector2i( 211 -115, -(128 -151) ) );
        vertex->push_back( Vector2i( 202 -115, -(306 -151) ) );
        vertex->push_back( Vector2i( 109 -115, -(250 -151) ) );
        vertex->push_back( Vector2i( 83 -115,  -(209 -151) ) );
        vertex->push_back( Vector2i( 25 -115,  -(194 -151) ) );
        vertex->push_back( Vector2i( 5 -115,   -(143 -151) ) );
        vertex->push_back( Vector2i( 94 -115,  -(102 -151) ) );
    }
    else if( t==4 )
    {
        vertex->push_back( Vector2i( 92 -211,  -(31 -113) ) );
        vertex->push_back( Vector2i( 188 -211, -(48 -113) ) );
        vertex->push_back( Vector2i( 371 -211, -(131 -113) ) );
        vertex->push_back( Vector2i( 386 -211, -(168 -113) ) );
        vertex->push_back( Vector2i( 254 -211, -(198 -113) ) );
        vertex->push_back( Vector2i( 80 -211,  -(135 -113) ) );
        vertex->push_back( Vector2i( 31 -211,  -(78 -113) ) );
    }
    else if( t==5 )
    {
        vertex->push_back( Vector2i( 250 -207,  -(5 -173) ) );
        vertex->push_back( Vector2i( 321 -207,  -(28 -173) ) );
        vertex->push_back( Vector2i( 241 -207,  -(108 -173) ) );
        vertex->push_back( Vector2i( 328 -207,  -(196 -173) ) );
        vertex->push_back( Vector2i( 252 -207,  -(338 -173) ) );
        vertex->push_back( Vector2i( 166 -207,  -(312 -173) ) );
        vertex->push_back( Vector2i( 186 -207,  -(252 -173) ) );
        vertex->push_back( Vector2i( 106 -207,  -(185 -173) ) );
        vertex->push_back( Vector2i( 106 -207,  -(88 -173) ) );
        vertex->push_back( Vector2i( 138 -207,  -(37 -173) ) );
    }
    
    
}


//------------------------------------------------------------------------------

void                        
BackgroundGenerator::fractalize_polygon( std::vector<Vector2i>* vertex )
{
    std::vector<Vector2i> src_vertex = *vertex;

    vertex->clear();
    for( int i=0; i<src_vertex.size(); i++ )
    {
        int nxt_i = (i==src_vertex.size()-1)  ? 0  : i+1;

        float       dx      = src_vertex[nxt_i][0] - src_vertex[i][0];
        float       dy      = src_vertex[nxt_i][1] - src_vertex[i][1];

        Vector2i    mid     = Vector2i( -dy/2, dx/2 );
        float       mid_x   = src_vertex[i][0] + dx/2;
        float       mid_y   = src_vertex[i][1] + dy/2;
        float       k       = 0.01 + 50.0 / vfx::rnd(100);

        //vertex->push_back( src_vertex[i] );
        vertex->push_back( Vector2i( mid_x + k*mid[0], mid_y + k*mid[1] ) );
        //vertex->push_back( Vector2i( mid_x, mid_y ) );
    }
}


//------------------------------------------------------------------------------

void                        
BackgroundGenerator::scale_polygon( std::vector<Vector2i>* vertex, float s )
{
    float   l_bound = (*vertex)[0][0];
    float   r_bound = (*vertex)[0][0];
    float   t_bound = (*vertex)[0][1];
    float   b_bound = (*vertex)[0][1];

    for( int i=0; i<vertex->size(); i++ )
    {
        if( (*vertex)[i][0] < l_bound )
            l_bound = (*vertex)[i][0];
        
        if( (*vertex)[i][0] > r_bound )
            r_bound = (*vertex)[i][0];
         
        if( (*vertex)[i][1] > t_bound )
            t_bound = (*vertex)[i][1];
        
        if( (*vertex)[i][1] < b_bound )
            b_bound = (*vertex)[i][1];
    }

    float   x_center = l_bound + ( r_bound - l_bound ) / 2;
    float   y_center = b_bound + ( t_bound - b_bound ) / 2;

    for( i=0; i<vertex->size(); i++ )
    {
        float   x = (*vertex)[i][0] - x_center;
        float   y = (*vertex)[i][1] - y_center;

        x *= s;
        y *= s;

        (*vertex)[i] = Vector2i( x_center + x, y_center + y );
        //(*vertex)[i] = Vector2i( x, y );
    }
}


