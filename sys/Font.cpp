//==============================================================================
//
//  Raster font realization   
//
//==============================================================================
//
//  externals: 

    #include "Font.hpp"

namespace core
{
//==============================================================================
//
//  publics:


//==============================================================================
//
//  RasterFont

RasterFont::RasterFont( const RasterFontInfo& font_info, ParamSetPool& pool )
  : fnt_name(font_info.name),
    fnt_max_height(font_info.max_height),
    fnt_def_char_spacing(font_info.def_char_spacing),
    fnt_max_width(font_info.max_width),
    fnt_baseline(font_info.baseline)
{
    fnt_char_data = new UByte[128*(fnt_max_height+1)];
    memset( (void*)fnt_char_data, 0, 128*(fnt_max_height+1) );

    for( unsigned i=0; i<pool.size(); i++ )
    {
        UByte   code        = pool[i]["char"];
        UByte   width       = pool[i]["width"];
        UByte*  chardata    = fnt_char_data + int(code) * ( fnt_max_height + 1 );

        *chardata++ = width;

        for( int l=0; l<fnt_max_height; l++ )
        {
            char    linename[7];
            sprintf( linename, "line%i", l );

            UByte   line = (UByte)pool[i][linename];
            
            *chardata++ = line;
        }
    }
}


//------------------------------------------------------------------------------

RasterFont::~RasterFont()
{
    delete fnt_char_data;
}


//------------------------------------------------------------------------------

const UByte* 
RasterFont::char_data( char ascii_code ) const
{   /*
    UByte*  data = fnt_char_data + int(ascii_code) * ( fnt_max_height + 1 );
    UByte   w    = *(this->fnt_char_data+ int(ascii_code) * ( fnt_max_height + 1 ) );
    UByte*  d = data+0x31*( fnt_max_height + 1 );
    */
    return fnt_char_data + int(ascii_code) * ( fnt_max_height + 1 );
}


//------------------------------------------------------------------------------

void     
RasterFont::register_paramset( ParamSetManager* manager )
{
    manager->register_paramset( "fontinfo", bind_fontinfo_members );
    manager->register_paramset( "charinfo", bind_charinfo_members );
}


//------------------------------------------------------------------------------

void     
RasterFont::bind_fontinfo_members( ParamSetPool* pool )
{
    pool->start_member_binding();
        
        pool->bind_string_member("font_name");
        pool->bind_int_member("max_width");
        pool->bind_int_member("def_char_spacing");
        pool->bind_int_member("max_height");
        pool->bind_int_member("baseline");

    pool->finish_member_binding(); 
}


//------------------------------------------------------------------------------

void     
RasterFont::bind_charinfo_members( ParamSetPool* pool )
{
    pool->start_member_binding();

        pool->bind_ubyte_member("char");
        pool->bind_ubyte_member("width");

        pool->bind_ubyte_member("line0");
        pool->bind_ubyte_member("line1");
        pool->bind_ubyte_member("line2");
        pool->bind_ubyte_member("line3");
        pool->bind_ubyte_member("line4");
        pool->bind_ubyte_member("line5");
        pool->bind_ubyte_member("line6");
        pool->bind_ubyte_member("line7");
        pool->bind_ubyte_member("line8");
        pool->bind_ubyte_member("line9");
        pool->bind_ubyte_member("line10");
        pool->bind_ubyte_member("line11");

    pool->finish_member_binding();
}



//==============================================================================
//
//  RasterFontManager

RasterFontManager::RasterFontManager()
{
    RasterFont::register_paramset( &paramset_manager );
}


//------------------------------------------------------------------------------

RasterFontManager::~RasterFontManager()
{
    for( unsigned i=0; i<raster_font.size(); i++ )
        delete raster_font[i];
}


//------------------------------------------------------------------------------

RasterFont& 
RasterFontManager::font( const std::string& name )
{
    for( unsigned i=0; i<raster_font.size(); i++ )
    {
        if( raster_font[i]->name() == name )
            return *raster_font[i];
    }

    throw Error(Msg("font manger hasn't font named '%s'",name.c_str()));
}


//------------------------------------------------------------------------------

void 
RasterFontManager::load_font( BinFile* file )
{
    paramset_manager.clear_paramset("fontinfo");
    paramset_manager.clear_paramset("charinfo");
    paramset_manager.parse_ini_file( file );

    ParamSet        fnt_info    = paramset_manager.paramset( "fontinfo", 0 );
    ParamSet        charinfo    = paramset_manager.paramset( "charinfo", 0 );
    RasterFontInfo  fontinfo;

    fontinfo.name               = std::string( fnt_info["font_name"] );
    fontinfo.max_width          = fnt_info["max_width"];
    fontinfo.def_char_spacing   = fnt_info["def_char_spacing"];
    fontinfo.max_height         = fnt_info["max_height"];
    fontinfo.baseline           = fnt_info["baseline"];

    raster_font.push_back( new RasterFont( fontinfo, 
                                           paramset_manager.paramset_pool("charinfo") 
                                         )
                         );
}



} // namespace core











