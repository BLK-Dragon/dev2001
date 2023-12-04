//==============================================================================
//
//  Resource pack realization   
//
//==============================================================================
//
//  externals: 

    #include <algorithm>
    
    #include "ResourcePack.hpp"

namespace core
{
//==============================================================================
//
//  publics:



//------------------------------------------------------------------------------

ResourcePack::ResourcePack( FileSystem* fs )
  : file_system(fs)
{
}


//------------------------------------------------------------------------------

ResourcePack::~ResourcePack()
{
    for( int i=0; i<rc_image.size(); ++i )
        delete rc_image[i];
}

//------------------------------------------------------------------------------

void
ResourcePack::precache_image( const std::string& name )
{
    for( int i=0; i<rc_image_name.size(); i++ )
    {
        if( rc_image_name[i] == name )
            return;
    }
    
    BinFile*    file = file_system->create_file( name );

    rc_image.push_back( new Image(file) );
    rc_image_name.push_back( name );

    delete file;
    return;
}


//------------------------------------------------------------------------------

Image& 
ResourcePack::image( const std::string& name )
{      
    for( int i=0; i<rc_image_name.size(); i++ )
    {
        if( rc_image_name[i] == name )
            return *rc_image[i]; 
    }

    BinFile*    file = file_system->create_file( name );

    rc_image.push_back( new Image(file) );
    rc_image_name.push_back( name );

    delete file;
    return *rc_image[ rc_image.size() - 1 ];
}



} // namespace core
