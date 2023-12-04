#if !defined _FONT_HPP_
#define _FONT_HPP_
//==============================================================================
//
//  Font operations decls   
//
//==============================================================================
//
//  externals: 

    #include "ParamSet.hpp"

namespace core
{
//==============================================================================
//
//  publics:




struct
RasterFontInfo  
{
    std::string     name;
    int             max_width;
    int             def_char_spacing;
    int             max_height;
    int             baseline;
};


//==============================================================================




class
RasterFont  
{
  public:
                        RasterFont( const RasterFontInfo& font_info, 
                                    ParamSetPool& pool
                                  );
                       ~RasterFont();

    const std::string&  name() const;
    int                 max_width() const;
    int                 def_char_spacing() const;
    int                 max_height() const;
    int                 baseline() const;

    const UByte*        char_data( char ascii_code ) const;

    static void         register_paramset( ParamSetManager* manager );
    static void         bind_fontinfo_members( ParamSetPool* pool );
    static void         bind_charinfo_members( ParamSetPool* pool );

  
  private:

    std::string     fnt_name;
    int             fnt_max_width;
    int             fnt_def_char_spacing;
    int             fnt_max_height;
    int             fnt_baseline;
    UByte*          fnt_char_data;
};


//==============================================================================




class
RasterFontManager  
{
  public:
                    RasterFontManager();
                   ~RasterFontManager();

    RasterFont&     font( const std::string& name );
    
    void            load_font( BinFile* file );


  private:
    ParamSetManager             paramset_manager;
    std::vector<RasterFont*>    raster_font;

};



//==============================================================================
//
//  RasterFont:

inline const std::string&
RasterFont::name() const
{
    return fnt_name;
}


//------------------------------------------------------------------------------

inline int 
RasterFont::max_width() const
{
    return fnt_max_width;
}


//------------------------------------------------------------------------------

inline int 
RasterFont::def_char_spacing() const
{
    return fnt_def_char_spacing;
}


//------------------------------------------------------------------------------

inline int 
RasterFont::max_height() const
{
    return fnt_max_height;
}


//------------------------------------------------------------------------------

inline int 
RasterFont::baseline() const
{
    return fnt_baseline;
}


} // namespace core
#endif // _FONT_HPP_