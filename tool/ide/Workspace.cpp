//==============================================================================
//
//  Workspace realization   
//
//==============================================================================
//
//  externals: 

    #include <dom/DOMString.hpp>    
    
    #include "Workspace.hpp"


    using   std::string;
    using   std::vector;


//==============================================================================
//
//  publics:

Workspace::Workspace()
{
    XMLPlatformUtils::Initialize();
    
    parser = new DOMParser;
}


//------------------------------------------------------------------------------

Workspace::~Workspace()
{
    delete parser;
    XMLPlatformUtils::Terminate();
}


//------------------------------------------------------------------------------

void            
Workspace::load_from_XML( const std::string& name )
{
    parser->setValidationScheme( DOMParser::Val_Auto );
    parser->setDoNamespaces( false );
    parser->setCreateEntityReferenceNodes( false );
    parser->setToCreateXMLDeclTypeNode( true );
//    ErrorHandler *errReporter = new DOMTreeErrorReporter();
//    parser->setErrorHandler(errReporter);

    try
    {
        parser->parse( name.c_str() );
    }
    catch( const XMLException& e )
    {
        throw WorkspaceError( DOMString(e.getMessage()).transcode() );
    }
    catch( const DOM_DOMException& e )
    {
        throw WorkspaceError( DOMString(e.msg).transcode() );
    }
    catch( ... )
    {
        throw WorkspaceError( "An error occured during parsing\n " );
    }
}


//------------------------------------------------------------------------------

void            
Workspace::create_makefile( const std::string& prj_name, 
                            const std::string& cfg_name,
                            const std::string& mk_name
                          )
{
    int i;
    
    
    // get workspace

    DOM_Document doc                = parser->getDocument();
   
    DOM_NodeList    workspace_list  = doc.getElementsByTagName( "workspace" );
    
    if( workspace_list.getLength() == 0 )
        throw WorkspaceError( "no workspaces" );

    if( workspace_list.item( 0 ).getNodeType() != DOM_Node::ELEMENT_NODE )
        throw WorkspaceError( "can't get workspace" );
    
    DOM_Element     workspace       = (DOM_Element&)(workspace_list.item( 0 ));

    
    // find project

    DOM_NodeList    project_list    = workspace.getElementsByTagName( "project" );
    DOM_Element     project;
    bool            project_found   = false;

    if( project_list.getLength() == 0 )
        throw WorkspaceError( "no projects" );

    for( i=0; i<project_list.getLength(); i++ )
    {
        if( project_list.item( 0 ).getNodeType() != DOM_Node::ELEMENT_NODE )
            continue;
        
        project = (DOM_Element&)project_list.item( i );

        if( !strcmp( project.getAttribute( "name" ).transcode(), prj_name.c_str() ) )
        {
            project_found = true;
            break; 
        }
    }

    if( !project_found )
    {
        throw WorkspaceError( core::Msg( "project \"%s\" not found",
                                         prj_name.c_str()
                                       ) 
                            ); 
    }


    // find config

    DOM_NodeList    config_list     = project.getElementsByTagName( "config" );
    DOM_Element     config;
    bool            config_found    = false;

    if( config_list.getLength() == 0 )
    {
        throw WorkspaceError( core::Msg("project \"%s\" doesn't contain any configs",
                                        prj_name.c_str()
                                       ) 
                            );
    }

    for( i=0; i<config_list.getLength(); i++ )
    {
        if( config_list.item( 0 ).getNodeType() != DOM_Node::ELEMENT_NODE )
            continue;
        
        config = (DOM_Element&)config_list.item( i );

        if( !strcmp( config.getAttribute( "name" ).transcode(), cfg_name.c_str() ) )
        {
            config_found = true;
            break; 
        }
    }

    if( !config_found )
    {
        throw WorkspaceError( core::Msg( "config \"%s\" not found "
                                         "in project \"%s\"",
                                         cfg_name.c_str(), prj_name.c_str()
                                       ) 
                            ); 
    }

    
    // find link specification

    DOM_NodeList    link_list = config.getElementsByTagName( "link" );

    if( link_list.getLength() == 0 )
    {    
        throw WorkspaceError( core::Msg( "link specification not found in" 
                                         "project '%s'",
                                         prj_name.c_str()
                                       ) 
                            );
    }
    if( link_list.item( 0 ).getNodeType() != DOM_Node::ELEMENT_NODE )
    {
        throw WorkspaceError( core::Msg( "eror getting link specification " 
                                         "for project '%s'",
                                         prj_name.c_str()
                                       ) 
                            );
    }
 
    DOM_Element     link = (DOM_Element&)link_list.item( 0 );


    // find extention associations
    
    DOM_NodeList    association_list = config.getElementsByTagName( "file_association" );
    if( association_list.getLength() == 0 )
    {    
        throw WorkspaceError( core::Msg( "file associations not found in" 
                                         "project '%s'",
                                         prj_name.c_str()
                                       ) 
                            );
    }


    // get file list

    DOM_NodeList    file_list = project.getElementsByTagName( "file" );
    if( file_list.getLength() == 0 )
    {    
        throw WorkspaceError( core::Msg( "project '%s' doesn't contain files",
                                         prj_name.c_str()
                                       ) 
                            );
    }


    // create makefile
    //
    //

    FILE*   mk_file = fopen( mk_name.c_str(), "w+" );


    // title

    fprintf( mk_file,
            "#===============================================================================\n"
            "#\n"
            "# '%s' project MAKEFILE\n"
            "# generated by gen_makefile 1.1 (c) Ghost Dragon 2001\n"
            "#\n"
            "#===============================================================================\n\n",
            prj_name.c_str()
           );

    // macros

    DOM_NodeList     macro_list = workspace.getElementsByTagName( "macro" );

    for( i=0; i<macro_list.getLength(); i++ )
    {
        if( macro_list.item( i ).getNodeType() != DOM_Node::ELEMENT_NODE )
            continue;

        DOM_Element macro   = (DOM_Element&)macro_list.item( i );

        fprintf( mk_file, "%-24s= %s\n", 
                 macro.getAttribute("name").transcode(),
                 macro.getAttribute("value").transcode()
               );
    } 
    fprintf( mk_file, "\n\n" );
    
    
    // intermediate file list

    std::vector<std::string>    intermediate;


    for( i=0; i<association_list.getLength(); i++ )
    {
        if( association_list.item( i ).getNodeType() != DOM_Node::ELEMENT_NODE )
            continue;

        DOM_Element assoc   = (DOM_Element&)association_list.item( i );
        std::string in_ext  = assoc.getAttribute( "in_ext" ).transcode();
        std::string out_ext = assoc.getAttribute( "out_ext" ).transcode();

        std::string i_macro = prj_name.c_str() + std::string("_") + 
                              in_ext + out_ext + std::string("s");
        intermediate.push_back( i_macro );
        
        fprintf( mk_file, "%-79s\\\n", (i_macro + std::string(" =")).c_str() );
        for( int j=0; j<file_list.getLength(); j++ )
        {
            if( file_list.item( j ).getNodeType() != DOM_Node::ELEMENT_NODE )
                continue;

            DOM_Element file        = (DOM_Element&)file_list.item( j );
            std::string full_name   = file.getAttribute( "name" ).transcode();
            std::string name        = strip_ext( full_name );
            std::string ext         = ext_of( full_name );

            if( ext != in_ext )
                continue;

            name += ".";
            name += out_ext;
            fprintf( mk_file, "%-79s\\\n", 
                     (std::string("    _") + 
                      std::string(config.getAttribute("name").transcode()) + 
                      std::string("\\") + name).c_str() 
                   );

            //file.setAttribute( "out_name", name.c_str() );
        }
    }

    
    // link section

    std::string     target_name = std::string("_") +
                                  std::string(config.getAttribute( "name" ).transcode()) +
                                  std::string("\\") +
                                  std::string(project.getAttribute( "target" ).transcode());

    fprintf( mk_file, "\n\n%-24s: ", target_name.c_str() );
    fprintf( mk_file, "__before_all__ " );
    for( i=0; i<intermediate.size(); i++ )
        fprintf( mk_file, "$(%s) ", intermediate[i].c_str() );
    fprintf( mk_file, "\n" );
    
    fprintf( mk_file, "%-24s  %s\n", 
             "", link.getAttribute("before").transcode()
           );
    
    fprintf( mk_file, "%-24s  %-53s\\\n", "",  
             link.getAttribute("tool").transcode()
           );
    
    
    char    buf[1024];
    char*   begin       = buf;
    char*   end;
    bool    last_step   = false;
    
    strcpy( buf, link.getAttribute( "opt" ).transcode() );
    while( !last_step )
    {
        while( *begin == ' ' )
            ++begin;

        if( *begin == '\0' )
            break;

        end = begin;

        while( *end != ' '  &&  *end != '\0' )
            ++end;

        if( *end == '\0' )
            last_step = true;

        char    opt[64];

        strncpy( opt, begin, end-begin );
        opt[end-begin] = '\0';

        fprintf( mk_file, "%-24s", "" );
        fprintf( mk_file, "  " );
        fprintf( mk_file, "%-53s\\\n", opt );

        begin = end;
    }
    
    for( i=0; i<intermediate.size(); i++ )
    {
        fprintf( mk_file, "%-24s", "" );     
        fprintf( mk_file, "  " );
        fprintf( mk_file, "$(%s) ", intermediate[i].c_str() );
    }
    
    fprintf( mk_file, "\n%-24s  %s", 
             "", link.getAttribute("after").transcode()
           );
    fprintf( mk_file, "\n\n" );

    
    // file sections
    
    fprintf( mk_file, "%-24s: \n%-24s  %s\n\n", 
             "__before_all__", "",
             link.getAttribute("before_all").transcode()
           );

    
    std::vector<std::string>    depency;

    for( i=0; i<file_list.getLength(); i++ )
    {
        int j;

        if( file_list.item( i ).getNodeType() != DOM_Node::ELEMENT_NODE )
            continue;

        DOM_Element file        = (DOM_Element&)file_list.item( i );
        std::string full_name   = file.getAttribute( "name" ).transcode();
        std::string name        = strip_ext( full_name );
        std::string ext         = ext_of( full_name );

        depency.clear();
        create_depencies_of_file( full_name, &depency );

        fprintf( mk_file, "\n#" );
        for( j=0; j<79; j++ )
            fprintf( mk_file, "-" );
        fprintf( mk_file, "\n#\n#   '%s'\n", full_name.c_str() );

        std::string list_macro  = name + std::string("_")  + ext + 
                                  std::string("__files");

        fprintf( mk_file, "\n%-79s\\\n", 
                 (list_macro + std::string(" =")).c_str() 
               );

        for( j=0; j<depency.size(); j++ )
        {
            fprintf( mk_file, "    %-75s\\\n", depency[j].c_str() );
        }
        fprintf( mk_file, "\n" );

        DOM_Element     assoc;
        bool            assoc_found = false;

        for( j=0; j<association_list.getLength(); j++ )
        {
            if( association_list.item( j ).getNodeType() != DOM_Node::ELEMENT_NODE )
                continue;
            
            assoc = (DOM_Element&)association_list.item( j );

            std::string in_ext = assoc.getAttribute("in_ext").transcode();

            if( in_ext != ext )
            {
                assoc_found = true;
                break; 
            }
        }
        
        name += ".";
        name += assoc.getAttribute("out_ext").transcode();
        
        fprintf( mk_file, "%-24s: ", 
                 ( std::string("_") +
                   config.getAttribute("name").transcode() + std::string("\\") +
                   name 
                 ).c_str() 
               );
        fprintf( mk_file, "%s $(%s)\n", full_name.c_str(), list_macro.c_str() );
        
        fprintf( mk_file, "%-24s  %s %s %s\n\n", 
                 "",
                 assoc.getAttribute("tool").transcode(),
                 assoc.getAttribute("opt").transcode(),
                 full_name.c_str()
               );
    }


    
    fclose( mk_file );


/*    
    for( int p=0; p<project_info.size(); p++ )
    {
        if( project_info[p].name != prj_name ) continue;

        FILE*   mk_file = fopen( mk_name.c_str(), "w" );
        int     i;
        
        
        // title
        //
        //

        fprintf( mk_file,
                "#===============================================================================\n"
                "#\n"
                "# MAKEFILE for '%s'\n"
                "# generated by gen_makefile 1.0 (c) BLK Dragon 2001\n"
                "#\n"
                "#===============================================================================\n\n",
                project_info[p].name.c_str()
               );
        
        
        // create link section
        //
        //

        // intermediate files list
        
        fprintf( mk_file, "%s_%ss = \\\n",  
                 project_info[p].name.c_str(),
                 project_info[p].link_info.ext.c_str()
               ); 

        for( i=0; i<project_info[p].filename.size(); i++ )
        {
            fprintf( mk_file, "%s.%s \\\n",
                     ( strip_ext(project_info[p].filename[i]) ).c_str(),
                     project_info[p].link_info.ext.c_str()
                   ); 
        }
        fprintf( mk_file, "\n\n" );

        
        // link comand
        
        fprintf( mk_file, "%s : ", project_info[p].out_name.c_str() );
        fprintf( mk_file, "$(%s_%ss) ", 
                 project_info[p].name.c_str(),
                 project_info[p].link_info.ext.c_str()
               );
        fprintf( mk_file, "\n    %s \\\n", project_info[p].link_info.tool.c_str() );

        
        // options list

        process_long_string( mk_file, project_info[p].link_info.opt ); 
        
        fprintf( mk_file, "    $(%s_%ss)",  
                 project_info[p].name.c_str(),
                 project_info[p].link_info.ext.c_str()
               );

        fprintf( mk_file, "\n\n" );


        // files rules

        for( i=0; i<project_info[p].filename.size(); i++ )
        {
            vector<string>  depency;

            create_depencies_of_file( project_info[p].filename[i], &depency );
            fprintf( mk_file, "\n%s_files = \\\n",
                     ( strip_ext(project_info[p].filename[i]) ).c_str()
                   );
            for( int j=0; j<depency.size(); j++ )
                fprintf( mk_file, "%s \\\n", depency[j].c_str() );

            fprintf( mk_file, "\n%s.%s : %s $(%s_files)\n", 
                     (strip_ext(project_info[p].filename[i]) ).c_str(),
                     project_info[p].link_info.ext.c_str(),
                     project_info[p].filename[i].c_str(), 
                     ( strip_ext(project_info[p].filename[i]) ).c_str()
                   );
            
            int xt = -1;

            for( int x=0; x<project_info[p].association.size(); x++ )
            {
                if( project_info[p].association[x].ext == 
                    ext_of( project_info[p].filename[i] )
                  )
                {
                    xt = x;
                    break;
                }
            }

            if( xt == -1 )
                continue;
            
            fprintf( mk_file, "    %s ",
                     project_info[p].association[xt].tool.c_str()
                   );  
            
            process_long_string( mk_file, project_info[p].association[xt].opt );                     

            fprintf( mk_file, " %s\n\n", 
                     project_info[p].filename[i].c_str()
                   );
        }


        fclose( mk_file );
    }     
*/
}


//------------------------------------------------------------------------------

void
Workspace::create_depencies_of_file( const string& filename, 
                                     vector<string>* depency 
                                   )
{
    depency->clear();
    check_depencies_of_file( filename, depency );
}


//------------------------------------------------------------------------------

void
Workspace::check_depencies_of_file( const std::string& filename, 
                                    std::vector<std::string>* depency 
                                  )
{
    // adds depencies of file into vector depency

    
    FILE*   src_file = fopen( filename.c_str(), "rt" );
    char    line[120];
    char    inc_filename[256];
    char    filename_path[256];
    const char*   src;
    char*   dst;

    if( !src_file )
    {   /*
        error_msg( Msg( "can't open \"%s\", its depencies not processed",
                        filename.c_str()
                      ) 
                 ); */
        return;
    }
    //info_msg( Msg("checking \"%s\"\n", filename.c_str()) );

    src = filename.c_str() + strlen(filename.c_str());
    while( *src != '.'  &&  src > filename.c_str() ) --src;
    //if( src == filename.c_str() )
    //    error_msg( Msg("bad name (%s)",filename.c_str()) );
    while( *src != '\\'  &&  src > filename.c_str() ) --src;
    if( src > filename.c_str() )
    {
        strncpy( filename_path, filename.c_str(), src-filename.c_str()+1 );
        filename_path[ src - filename.c_str()+1 ] = '\0';
    }
    else
    {
        filename_path[0] = '\0';
    }
    //info_msg( Msg("path=\"%s\"\n", filename_path) );

    while( !feof(src_file) )
    {
        fgets( line, 120, src_file );
        src = line;

        
        // check if it's #include

        while( *src == ' '  ||  *src == '\t' )  src++;
        if( strncmp( src, "#include", 8 ) )
            continue;

        src = strchr( line, '"' );
        if( !src ) continue;             
        
        strcpy( inc_filename, filename_path );
        //strcat( inc_filename, "\\" );
        
        ++src;
        dst = inc_filename + strlen(inc_filename);
        while( *src != '"'  &&  *src  != '\0' )
            *dst++ = *src++;
        if( *src == '\0' ) continue;
        *dst = '\0';

        
        // insert new depency if it isn't dupe

        bool    dupe = false;

        for( int i=0; i< depency->size(); i++ )
        {
            if( (*depency)[i] == inc_filename )
            {
                dupe = true;
                break;
            } 
        }
        
        if( !dupe )
            depency->push_back( inc_filename );
        
        check_depencies_of_file( inc_filename, depency );
    }

    fclose( src_file );
    //info_msg( Msg( "\"%s\" checked\n", filename.c_str()) );
}


//------------------------------------------------------------------------------
/*
void            
Workspace::startElement( const XMLCh* const uri, 
                       const XMLCh* const localname, 
                       const XMLCh* const qname,
                       const Attributes& attrs
                     )
{
     std::string    elem_name( DOMString(localname).transcode() );
     
    if     ( elem_name == "project" )
    {
        cur_project = ProjectInfo();
        //.association.clear();
        //cur_project.filename.clear();
        for( int i=0; i<attrs.getLength(); i++ )
        {
            std::string attr_name = DOMString(attrs.getLocalName(i)).transcode();
            std::string attr_value= DOMString(attrs.getValue(i)).transcode();

            if     ( attr_name == "name" )
            {
                cur_project.name = attr_value; 
            }
            else if( attr_name == "out_name" )
            {
                cur_project.out_name = attr_value; 
            }
        }

    }
    else if( elem_name == "link" )
    {
        for( int i=0; i<attrs.getLength(); i++ )
        {
            std::string attr_name = DOMString(attrs.getLocalName(i)).transcode();
            std::string attr_value= DOMString(attrs.getValue(i)).transcode();

            if     ( attr_name == "ext" )
            {
                cur_project.link_info.ext = attr_value; 
            }
            else if( attr_name == "tool" )
            {
                cur_project.link_info.tool = attr_value; 
            }
            else if( attr_name == "opt" )
            {
                cur_project.link_info.opt += attr_value; 
            }
        }
    }
    else if( elem_name == "file_association" )
    {
        FileAssociation ass;
        
        for( int i=0; i<attrs.getLength(); i++ )
        {
            std::string attr_name = DOMString(attrs.getLocalName(i)).transcode();
            std::string attr_value= DOMString(attrs.getValue(i)).transcode();

            if     ( attr_name == "ext" )
            {
                ass.ext = attr_value; 
            }
            else if( attr_name == "tool" )
            {
                ass.tool = attr_value; 
            }
            else if( attr_name == "opt" )
            {
                ass.opt += attr_value; 
            }
        }

        cur_project.association.push_back( ass );
    }
    else if( elem_name == "file" )
    {
        for( int i=0; i<attrs.getLength(); i++ )
        {
            std::string attr_name = DOMString(attrs.getLocalName(i)).transcode();
            std::string attr_value= DOMString(attrs.getValue(i)).transcode();
            
            if( attr_name == "name" )
                cur_project.filename.push_back( attr_value );
        }
    }
}


//------------------------------------------------------------------------------

void            
Workspace::endElement( const XMLCh* const uri, 
                       const XMLCh* const localname, 
                       const XMLCh* const qname
                     )
{
    std::string    elem_name( DOMString(localname).transcode() );
     
    if( elem_name == "project" )
    {
        project_info.push_back( cur_project ); 
    }
}


//------------------------------------------------------------------------------

void            
Workspace::characters( const XMLCh * const chars,  
                       unsigned int length 
                     )  
{
//    std::string    white( DOMString(chars).transcode() );
 
//    printf( "chars '%s'\n", white );
}


//------------------------------------------------------------------------------

void            
Workspace::ignorableWhitespace( const XMLCh * const chars,  
                                unsigned int length 
                              )
{
    std::string    white( DOMString(chars).transcode() );

    printf( "white '%s'\n", white );     
}


//------------------------------------------------------------------------------

void            
Workspace::process_long_string( FILE* file, const std::string& str )
{
    // file is expected to be valid open file
    
    fprintf( file, " %s \\\n", str.c_str() );
    
    
    
    const char* begin;
    const char* end         = str.c_str();
    bool        first_time  = true;

    while( true )
    {
        char    buf[4096];
        begin = end;
        
        if( *begin == '\0' )
            break;

        end = begin;
        if( !first_time )
            fprintf( file, " \\\n" ); 
        else
            first_time = false;

        while( *end != '\n'  &&  *end != '\r'  &&  *end != '\0' )
            ++end;

        strncpy( buf, begin, end-begin );
        buf[end-begin] = '\0';
        ++end;

        fprintf( file, "    %s", buf );
        begin = end;
    }
    
}
*/



