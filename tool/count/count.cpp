#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>


//------------------------------------------------------------------------------

int
line_count( char* filename )
{
    FILE*   file = fopen( filename, "rt" );     
    int     n    = 0;

    if( !file )
        return 0;
    
    while( !feof(file) )
    {
        char    line[120];

        fgets( line, 120, file ); 
        //if( feof(file) )
        //    break;

        ++n;
    }
    
    fclose( file );
    return n;
}



//------------------------------------------------------------------------------
//
//  Entry point here
//
//------------------------------------------------------------------------------

main( int argn, char* argv[] )
{
    if( argn < 2 )
    {
        printf( "usage:\ncount <mask>\n" );
        return;
    }
    
    const char*         mask        = argv[1];
    HANDLE              handle;
    WIN32_FIND_DATA     find_data;
    int                 count       = 0;

    handle = ::FindFirstFile( mask, &find_data );

    if( handle == INVALID_HANDLE_VALUE )
    {
        printf( "no files found\n" ); 
        return -13;
    }

    while( true )
    {
        int n = line_count( find_data.cFileName );
        
        count += n;
        printf( "%s  %i lines\n", find_data.cFileName, n );
        
        if( !::FindNextFile( handle, &find_data ) )
            break;
    }

    printf( "total line count: %i\n", count );
}

