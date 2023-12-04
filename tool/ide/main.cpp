#include "Workspace.hpp"

std::vector<std::string>      Switch;
std::vector<std::string>      Parm;


//==============================================================================
//
//  Entry point here
//
//==============================================================================

int
main( int argn, char* argv[] )
{
    if( argn < 2 )
    {
        printf( "makefile generator tool v1.0\n"
                "(c) 2001 BLK Dragon\n\n"
                "usage:\n"
                "create_makefile <switch 1>, <switch 2>, ... <switch n> "
                    "[XML project filename]\n\n"
                "switches:\n"
                "-project=<project name>\n"
                "-config=<config name>\n"
                "-makefile=<makefile name>\n\n"
              );
        return 0;
    }
    
    
    // process command-line

    int             i = 0;
    char            buf;
    std::string     project;
    std::string     config;
    std::string     makefile;

    enum
    {
        switch_reading  = 13,
        parm_reading
    }
    state = switch_reading;

    while( ++i < argn )
    {
        switch( state )
        {
            case switch_reading :
                if     ( !strncmp( argv[i], "-project", 8 ) )
                {
                    //sscanf( argv[i], "-project=%s", &buf );
                    project = argv[i]+9;
                }
                else if( !strncmp( argv[i], "-config", 7 ) )
                {
                    //sscanf( argv[i], "-config=%s", &buf );
                    config = argv[i]+8;
                }
                else if( !strncmp( argv[i], "-makefile", 9 ) )
                {
                    //sscanf( argv[i], "-makefile=%s", &buf );
                    makefile = argv[i]+10;
                }
                else if( argv[i][0] == '-' )
                {
                    printf( "ignoring unknown switch \"%s\"\n", argv[i] );
                    state = parm_reading;
                }
                else
                {
                    state = parm_reading;
                    --i;
                    continue;
                }
                break;

            case parm_reading :
                Parm.push_back( argv[i] ); 
                break;
        }
    }
    
    
    Workspace   workspace;

    try
    {
        workspace.load_from_XML( Parm[0] ); 
        workspace.create_makefile( project, config, makefile );
    }
    catch( const WorkspaceError& e )
    {
        printf( "WorkSpace Error> %s\n", e.description().c_str() );
        return -13;
    }
    catch( ... )
    {
        printf( "Unknown Error, aborting\n" ) ;
        return -13;
    }

}