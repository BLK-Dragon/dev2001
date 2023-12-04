#if !defined _RESOURCEPACK_HPP_
#define _RESOURCEPACK_HPP_
//==============================================================================
//
//  Resource pack decls
//
//==============================================================================
//
//  externals: 

    #include <string>
    #include <vector>
    
    #include "FileSystem.hpp"
    #include "Image.hpp"

namespace core
{
//==============================================================================
//
//  publics:




class
ResourcePack  
{
  public:

                                ResourcePack( FileSystem* fs );
                               ~ResourcePack();

    void                        precache_image( const std::string& name );
    Image&                      image( const std::string& name );


  private:
    FileSystem*                 file_system;
    std::vector<Image*>         rc_image;
    std::vector<std::string>    rc_image_name;

};






} // namespace core
#endif //  _RESOURCEPACK_HPP_ 
