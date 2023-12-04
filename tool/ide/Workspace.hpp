#if !defined _PROJECT_HPP_
#define _PROJECT_HPP_
//==============================================================================
//
//  Workspace decl   
//
//==============================================================================
//
//  externals: 

    #include <vector>
    #include <string>
    #include <stdio.h>
    #include <string.h>
    #include <direct.h>

    #include <util/PlatformUtils.hpp>
    #include <util/XMLString.hpp>
    #include <util/XMLUniDefs.hpp>
    #include <framework/XMLFormatter.hpp>
    #include <util/TranscodingException.hpp>
    #include <dom/DOM_DOMException.hpp>

    #include <parsers/DOMParser.hpp>
    #include <dom/DOM.hpp>

    #include "..\..\sys\Exception.hpp"


//==============================================================================
//
//  publics:




struct
FileAssociation  
{
    FileAssociation(){};
    
    std::string     ext;
    std::string     tool;
    std::string     opt;
};



struct
LinkInfo  
{
    LinkInfo(){};
    
    std::string     ext;
    std::string     tool;
    std::string     opt;

    std::string     before;
    std::string     after;
};



struct
ProjectInfo  
{
    ProjectInfo(){};
    
    std::string                     name;
    std::string                     out_name;
    LinkInfo                        link_info;
    std::vector<FileAssociation>    association;
    std::vector<std::string>        filename;
};

//==============================================================================




class
WorkspaceError
  : public core::Error  
{
  
  public:

                                WorkspaceError( const core::Msg& msg)
                                  : core::Error( msg ) { comment_text = "WorkSpace error"; };
                                
};



//==============================================================================




class
Workspace
//  : public DefaultHandler  
{
  
  public:

                    Workspace();
                   ~Workspace();
    
    
    void            load_from_XML( const std::string& name );
    void            create_makefile( const std::string& prj_name, 
                                     const std::string& cfg_name,
                                     const std::string& mk_name
                                   );

    std::string     strip_ext( const std::string& name );
    std::string     ext_of( const std::string& name );

    static void     create_depencies_of_file( const std::string& filename, 
                                              std::vector<std::string>* depency
                                            );
    static void     check_depencies_of_file( const std::string& filename, 
                                             std::vector<std::string>* depency 
                                           );


    // XML related
/*    
    void            startElement( const XMLCh* const uri, 
                                  const XMLCh* const localname, 
                                  const XMLCh* const qname, 
                                  const Attributes& attrs
                                );
    void            endElement( const XMLCh* const uri, 
                                const XMLCh* const localname, 
                                const XMLCh* const qname
                              );
    void            characters( const XMLCh * const chars,  
                                unsigned int length 
                              );
    void            ignorableWhitespace( const XMLCh * const chars,  
                                         unsigned int length 
                                       );

    void            warning( const SAXParseException& exception ){};
    void            error( const SAXParseException& exception ){};
    void            fatalError( const SAXParseException& exception ){};
*/
  
  private:
    
    DOMParser*                  parser;
    
//    void            process_long_string( FILE* file, const std::string& str );

    std::vector<ProjectInfo>    project_info;
    ProjectInfo                 cur_project;

};


//------------------------------------------------------------------------------

inline std::string     
Workspace::strip_ext( const std::string& name )
{
    std::string     s = name;

    s.erase( s.end()-4, s.end() );
    return s;
}


//------------------------------------------------------------------------------

inline std::string     
Workspace::ext_of( const std::string& name )
{
    std::string     s = name;
    
    s.erase( s.begin(), s.end()-3 );
    return s;
}


#endif // _WORKSPACE_HPP_