//==============================================================================
//
//  FileSystem realization   
//
//==============================================================================
//
//  externals: 

    #include "FileSystem.hpp"

namespace core
{
//==============================================================================
//
//  publics:


//------------------------------------------------------------------------------

FileSystem::FileSystem()
{
}


//------------------------------------------------------------------------------

FileSystem::~FileSystem()
{
}


//------------------------------------------------------------------------------

BinFile* 
FileSystem::create_file( const std::string& name )
{
    return new BinFile( name );
}



} // namespace core