#include <vector>
using std::vector;
#include <string>
using std::string;
#include <stdio.h>
#include <string.h>
#include <direct.h>



#include "..\..\sys\Exception.hpp"
using core::Error;
using core::Msg;


vector<string>      Switch;
vector<string>      Parm;

vector<string>      Depency;
string              Compiler;
string              Options;

vector<string>      SourceFile;

bool                CompileCPP  = false;

string              Compiler_CmdLine;


//------------------------------------------------------------------------------

inline void
error_msg( const Msg& msg )
{
    printf( "%s\n", msg.text().c_str() );
} 


//------------------------------------------------------------------------------

inline void
info_msg( const Msg& msg )
{
    //printf( "%s\n", msg.text().c_str() );
} 


//------------------------------------------------------------------------------

void
check_depencies_of_file( const string& filename, vector<string>* depency )
{
    // adds depencies of file into vector depency

    
    FILE*   src_file = fopen( filename.c_str(), "rt" );
    char    line[120];
    char    inc_filename[256];
    char    filename_path[256];
    const char*   src;
    char*   dst;

    if( !src_file )
    {
        error_msg( Msg( "can't open \"%s\", its depencies not processed",
                        filename.c_str()
                      ) 
                 ); 
        return;
    }
    info_msg( Msg("checking \"%s\"\n", filename.c_str()) );

    src = filename.c_str() + strlen(filename.c_str());
    while( *src != '.'  &&  src > filename.c_str() ) --src;
    if( src == filename.c_str() )
        error_msg( Msg("bad name (%s)",filename.c_str()) );
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
    info_msg( Msg("path=\"%s\"\n", filename_path) );

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

        for( int i=0; i< Depency.size(); i++ )
        {
            if( Depency[i] == inc_filename )
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
    info_msg( Msg( "\"%s\" checked\n", filename.c_str()) );
}


//------------------------------------------------------------------------------

void
create_depencies_of_file( const string& filename, vector<string>* depency )
{
    depency->clear();
    check_depencies_of_file( filename, depency );

    /*
    char*   dot_pos = strchr( filename.c_str(), '.' );
    char    name[80];

    if( !dot_pos )
        throw Error( Msg("bad file name: \"%s\"", filename.c_str() ) );

    strncpy( name, filename.c_str(), dot_pos-filename.c_str() );
    name[ dot_pos-filename.c_str() ] = '\0';

    FILE*   mk_file = fopen( (string(name) + ".mk").c_str(), "w+" );

    fprintf( mk_file, "%s_files = \\\n", name );
    for( int i=0; i<Depency.size(); i++ )
        fprintf( mk_file, "%s \\\n", Depency[i].c_str() );
    
    fprintf( mk_file, 
             "\n\n%s.obj : $(%s_files)\n"
             "    %s %s %s\n",
             name, name, Compiler.c_str(), Options.c_str(), filename.c_str()
           );

    fclose( mk_file );
    */
}


//------------------------------------------------------------------------------
/*
void
build_file( const string& filename )
{
    Depency.clear();
    check_depencies_of_file( filename );
    
    HANDLE      file;
    HANDLE      dep;
    FILETIME    file_time;
    FILETIME    dep_time;

    file = CreateFile( filename.c_str(),
                       GENERIC_READ,
                       0,
                       NULL,
                       OPEN_EXISTING,
                       FILE_ATTRIBUTE_NORMAL,
                       NULL
                     );
    
    if( file == INVALID_HANDLE_VALUE )
    {
        error_msg( Msg("can't open \"%s\", it's not processed\n",
                       filename.c_str()
                      ) 
                 );
        return;
    }
    
    if( !GetFileTime( file, NULL, NULL, &file_time ) )
    {
        error_msg( Msg("can't get timestamp of \"%s\", it's not processed\n",
                       filename.c_str()
                      ) 
                 );
        return;         
    }  
    
    for( int i=0; i<Depency.size(); i++ )
    {
        dep  = CreateFile( Depency[i].c_str(),
                           GENERIC_READ,
                           0,
                           NULL,
                           OPEN_EXISTING,
                           FILE_ATTRIBUTE_NORMAL,
                           NULL
                         );
    
        if( dep == INVALID_HANDLE_VALUE )
        {
            error_msg( Msg("can't open \"%s\", it's not processed\n") );
            return;
        }
        
        if( !GetFileTime( dep, NULL, NULL, &dep_time ) )
        {
            error_msg( Msg("can't get timestamp of \"%s\", "
                           "it's not processed\n"
                          ) 
                     );
            return;         
        }
        
        
        // compare times

        __int64 file_timestamp = (((__int64)file_time.dwHighDateTime )<<32) |
                                 file_time.dwLowDateTime;
        __int64 dep_timestamp  = (((__int64)dep_time.dwHighDateTime )<<32) |
                                 dep_time.dwLowDateTime;
        
        if( file_timestamp < dep_timestamp )
        {
            char   cmd_line[256];

            if( !Compiler_CmdLine.size() )
            {
                error_msg( "no compiler specified\n" );
                CloseHandle( dep );
                break;
            }

            sprintf( cmd_line, "%s %s",
                     Compiler_CmdLine.c_str(), filename.c_str()
                    );

            printf( "%s\n", cmd_line );
            WinExec( cmd_line, SW_SHOWDEFAULT );
            printf( "compiled\n" );
             
            CloseHandle( dep );
            break;
        }


        CloseHandle( dep );
    }
    
    CloseHandle( file );
}
*/

//==============================================================================
//
//  Entry point here
//
//==============================================================================

void
main( int argn, char* argv[] )
{
    if( argn < 2 )
    {
        printf( "build tool v0.1\n"
                "(c) 2001 BLK Dragon\n\n"
                "usage:\n"
                "build <switch 1>, <switch 2>, ... <switch n> [filename]\n\n"
                "switches:\n"
                "-c                 check depencies and compile\n"
                "-cc=\"cmd-line\"     command line for compiler\n"
              );
        return;
    }
    
    
    // process command-line

    int     i = 0;

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
                if( argv[i][0] == '-' )
                {
                    Switch.push_back( argv[i]+1 ); 
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
    
    
    for( i=0; i<Switch.size(); i++ )
        printf( "switch #%i = '%s'\n", i, Switch[i].c_str() );
    for( i=0; i<Parm.size(); i++ )
        printf( "parm #%i = '%s'\n", i, Parm[i].c_str() );
    


    // lookup switches
    
    for( i=0; i<Switch.size(); i++ )
    {   
        if( Switch[i] == "c" )
        {
            Depency.clear();
            create_depencies_of_file( Parm[0], &Depency ); 
            
            for( int i=0; i<Depency.size(); i++ )
                printf( "'%s'\n", Depency[i].c_str() );
        }
        
        /*
        if( Switch[i] == "c" )
            CompileCPP = true;

        if( !strncmp( Switch[i].c_str(), "cc=", 3 ) )
        {
            const char* cc = Switch[i].c_str() + 3;

            Compiler_CmdLine = cc;
        }
        */
    }

/*
    // create source-file list

    for( i=0; i<Parm.size(); i++ )
        SourceFile.push_back( Parm[i] );
    if( Parm.empty() )
    {
        error_msg( "no files to process\n" ); 
        return;
    }

    printf( "compile as '%s'\n", Compiler_CmdLine.c_str() );


    // do actual work

    if( CompileCPP )
    {
        for( i=0; i<SourceFile.size(); i++ )
            build_file( SourceFile[i] );
    }
    
    //create_depencies_of_file( Parm[0] );
*/    
}