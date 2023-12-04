#if !defined _RASTERIZER_HPP_
#define _RASTERIZER_HPP_
//==============================================================================
//
//  Rasterizer interface decl
//
//==============================================================================
//
//  externals:

    #include "..\..\sys\ParamSet.hpp"


//==============================================================================
//
//  publics:




class
Rasterizer  
{
  public:

                        Rasterizer();
    virtual            ~Rasterizer();

    virtual void        generate_scanline( core::UDword* scanline, int width ) =0;
    virtual void        advance_to_next_scanline() =0;
    
    virtual void        activate( int x_pos, int n=0 );
    void                deactivate();
    bool                is_active() const;
  
  
  private:

    bool                active;
};

/*
//==============================================================================

template < const std::string name >
class RasterizerImpl
{
  protected:
    
    static void                     bind_members( core::ParamSetPool* pool );
    
    static std::string              paramset_name;
    static core::ParamSetManager*   paramset_manager;
    
    
    core::ParamSet                  cur_paramset;
};
*/




//------------------------------------------------------------------------------

inline
Rasterizer::Rasterizer()
  : active(false)
{
}


//------------------------------------------------------------------------------

inline void        
Rasterizer::activate( int x_pos, int n )
{
    active = true; 
}


//------------------------------------------------------------------------------

inline void
Rasterizer::deactivate()
{
    active = false;
}


//------------------------------------------------------------------------------

inline bool    
Rasterizer::is_active() const
{
    return active; 
}

/*
//------------------------------------------------------------------------------

inline void                
Rasterizer::bind_to_parser( core::IniFileParser* parser )
{
    if( !paramset )
        core::Error("invalid Rasterizer::pool");
    
    parser->bind( paramset ); 
}
*/


#endif // _RASTERIZER_HPP_