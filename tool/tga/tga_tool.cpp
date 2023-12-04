    #include <fcntl.h>
    #include <io.h>
    #include <stdio.h>
    #include <string.h>
    #include <string>
    #include <vector>
    
    #include "..\..\sys\ImageFormat.hpp"


std::string                 Command;
std::vector<std::string>    Switch;
std::vector<std::string>    Parm;

//------------------------------------------------------------------------------

void
command_help()
{
    if( Parm.size() == 0 )
    {
        printf( "specify command you want to see help on\n" );
        return;
    } 

    if( Parm[0] == "ins" )
    {
        printf( "\n\nINSert command:\n"
                "---------------\n"
                "Ins command used to insert a file or string into development\n"
                "area of TGA file. Old contents of dev-area (if any) will be\n"
                "overwritten.\n\n"
                "Switches:\n"
                "  -s     second parm is a sequence of dwords that will be\n"
                "         placed into dev-area of TGA file.\n"
                "\n"
                "Parms:\n"
                "  parm1  name of file to process\n"
                "  parm2  name f bin-file to insert\n"
                "         string to place into dev-area (used with -s switch)\n"
                "\n"
              ); 
    }
    else if( Parm[0] == "pack" )
    {
        printf( "\n\nPACK command:\n"
                "-------------\n"
                "Pack command used to 'pack' a sequence of images into one\n"
                "TGA-file.\n"
                //" Quantity of images placed into dev-area as part of\n"
                //"text file; for example TGA file contains 24 frames of\n"
                //"animation, then dev-area looks like that:\n"
                //"...\n"
                //"[SPRITE]\n"
                //"frames = 24\n"
                //"...\n"
                "Packed files should have names 'file000', 'file001' and \n"
                "so on, e.g. 3 digits used for frame numeration, starting\n"
                "with zero.\n\n"
                "Switches:\n"
                "  -n<n>  number of files to process.\n"
                //"  -d     write number of images into TGA dev-area\n"
                "\n"
                "Parms:\n"
                "  parm1  name of input files ( without numbers and extension\n"
                "         e.g. instead 'file007.tga' should be just 'file' )\n"
                "\n"
              ); 
    }
}


//------------------------------------------------------------------------------

void
command_ins()
{
    FILE*       tga_file = fopen( Parm[0].c_str(), "rb+" ); 
    FILE*       bin_file;
    TGAfooter   footer;
    long        dev_offset;

    if( !tga_file )
    {
        printf( "can't open \"%s\"\n", Parm[0].c_str() );
        goto ins_exit;
    }
    
    if( Switch.size() == 0  &&  Parm.size() > 0 )
    {
        bin_file = fopen( Parm[1].c_str(), "rb" );  
        
        if( !bin_file )
        {
            printf( "can't open \"%s\"\n", Parm[1].c_str() );
            goto ins_exit;
        }
    }
    

    fseek( tga_file, -26, SEEK_END );
    dev_offset = ftell( tga_file );
    fread( &footer.extension_area_offset, sizeof(UDword), 1, tga_file );
    fread( &footer.developer_area_offset, sizeof(UDword), 1, tga_file );
    fread( &footer.signature, sizeof(UByte), 16, tga_file );
    fread( &footer.dot, sizeof(UByte), 1, tga_file );
    fread( &footer.null, sizeof(UByte), 1, tga_file );

    if( footer.developer_area_offset )
        dev_offset = footer.developer_area_offset;


    // truncate file

    int handle = _open( Parm[0].c_str(), _O_RDWR );

    if( handle == -1 )
    {
        printf( "can't open \"%s\"\n", Parm[0].c_str() );
        return;
    }

    _chsize( handle, dev_offset );
    _close( handle );


    // write dev area
    
    if( strncmp( (char*)footer.signature, "TRUEVISION-XFILE", 16 )  ||  
        footer.dot != '.'                                           ||
        footer.null != '\0' 
      )
    {
        printf( "\"%s\" is not a valid TGA file (or its format is obsolete)\n",
                Parm[0].c_str()
              );
        goto ins_exit; 
    }

    
    fseek( tga_file, dev_offset, SEEK_SET );
    
    if( Switch.size() > 0  &&  Switch[0] == "s" )
    {
        // extract dwords from Parm[1]

        const char*     p1;
        const char*     p2;

        p1 = Parm[1].c_str();
        p2 = p1;

        while( true )
        {
            while( *p2  &&  (*p2!=',') )
            {    
                if( !isxdigit(*p2)  &&  *p2!='x' )
                {
                    printf( "not a number in sequence (%c)\n", *p2 );
                    return; 
                }
                ++p2;
            }

            char    n[16];
            UDword  d;
            strncpy( n, p1, p2-p1 );
            n[ p2-p1 ] = '\0';
            sscanf( n, "%i", &d );

            printf( "%i\n", d );
            fwrite( &d, sizeof(UDword), 1, tga_file );

            if( *p2 == '\0' )
                break;

            p1 = ++p2;
        }
        
        //fwrite( Parm[1].c_str(), sizeof(UByte), strlen(Parm[1].c_str()), 
        //        tga_file 
        //      );
    }
    else
    {
        while( true )
        {
            UByte   tmp;
            
            fread( &tmp, sizeof(UByte), 1, bin_file );
            if( feof(bin_file) )
                break;
            fwrite( &tmp, sizeof(UByte), 1, tga_file );
        }
    }

    
    // write footer
    
    footer.developer_area_offset = dev_offset;
    fwrite( &footer.extension_area_offset, sizeof(UDword), 1, tga_file );
    fwrite( &footer.developer_area_offset, sizeof(UDword), 1, tga_file );
    fwrite( &footer.signature, sizeof(UByte), 16, tga_file );
    fwrite( &footer.dot, sizeof(UByte), 1, tga_file );
    fwrite( &footer.null, sizeof(UByte), 1, tga_file );
    
 ins_exit:   
    fcloseall();
}


//------------------------------------------------------------------------------

void
command_pack()
{
    int     n_images        = 1;
    int     n_digits        = 1;
    char    name[64];
    bool    write_devarea   = true;//false;

    if( Parm.size() == 0 )
    {
        printf( "no output file name specified\n" ); 
        return;
    }
    sprintf( name, "%s.tga", Parm[0].c_str() );
    
    for( int i=0; i<Switch.size(); i++ )
    {
        switch( Switch[i][0] )
        {
            case 'n' :
                sscanf( Switch[i].c_str(), "n%i", &n_images );
                break;

            case 'd' :
                write_devarea = true; 
                break;
 
            default:
                printf( "ignoring unknown switch \"%s\"", Switch[i].c_str() );
        }
    }


    TGAheader   out_header;
    FILE*       out         = fopen( name, "wb+" );

    if( !out )
    {
        printf( "can't open \"%s\" for writing\n", name ); 
        return;
    }

    
    fseek( out, sizeof(TGAheader), SEEK_SET );

    for( i=0; i<n_images; i++ )
    {
        sprintf( name, "%s%03i.tga", Parm[0].c_str(), i );
        
        FILE*       img = fopen( name, "rb" ); 
        TGAheader   header;

        if( !img )
        {
            printf( "can't open \"%s\" for reading", name ); 
            fcloseall();
            return;
        }

        fread( &header, sizeof(TGAheader), 1, img );
        if( i == 0 )
        {
            out_header = header;
        }

        if( header.width != out_header.width           ||
            header.height != out_header.height         ||
            header.bpp != out_header.bpp               ||
            header.descriptor != out_header.descriptor 
          )
        {
            printf( "all images should have the same width,heigth,bpp "
                    "& descriptor\n" 
                  ); 
            fcloseall();
            return;
        }

        for( long p=0; p<header.width*header.height*header.bpp/8; p++ )
        {
            char    ch;

            fread( &ch, sizeof(char), 1, img );
            fwrite( &ch, sizeof(char), 1, out );
        }

        fclose( img );
        //printf( "image %i: %i bytes written\n", i, 
        //        header.width*header.height*header.bpp/8 );
    }

    
    // write footer
    
    long    dev_area_offset = 0;//sizeof(TGAheader) + n_images * 
                              //out_header.width*out_header.height
                              //*out_header.bpp/8;
    long    xt_area_offset  = 0x00000000;
    
    if( write_devarea )
    {
        fwrite( &xt_area_offset, sizeof(UDword), 1, out );
        fwrite( &dev_area_offset, sizeof(UDword), 1, out );
        fwrite( "TRUEVISION-XFILE.\0", 18, 1, out );
    }

    
    // write header
    
    out_header.height  *= n_images;

    fseek( out, 0, SEEK_SET );
    fwrite( &out_header, sizeof(TGAheader), 1, out );

    

    fcloseall(); 
}



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
        printf( "tga_tool [command] <switches> <parms>\n"
                "       help    - help on commands\n"
                "       ins     - inserts bin-file into TGA's developer area\n"
                "       pack    - pack a sequence number of images into one\n"
              );
        return;
    }
    
    
    // process command-line

    int     i = 0;

    enum
    {
        command_reading = 13,
        switch_reading,
        parm_reading
    }
    state = command_reading;

    while( ++i < argn )
    {
        switch( state )
        {
            case command_reading :
                Command = argv[i]; 
                state   = switch_reading;
                break;

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

    
    // process commands

    if( Command == "help" )
        command_help();
    else if( Command == "ins" )
        command_ins();
    else if( Command == "pack" )
        command_pack();

}