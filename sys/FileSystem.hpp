#if !defined _FILESYSTEM_HPP_
#define _FILESYSTEM_HPP_
//==============================================================================
//
//  FileSystem decl
//
//==============================================================================
//
//  externals: 

    #include "File.hpp"
    #include "ZipFile.hpp"

namespace core
{
//==============================================================================
//
//  publics:




class
FileSystem  
{
  public:

                FileSystem();
               ~FileSystem();

    BinFile*    create_file( const std::string& name );

};


} // namespace core
#endif // _FILESYSTEM_HPP_