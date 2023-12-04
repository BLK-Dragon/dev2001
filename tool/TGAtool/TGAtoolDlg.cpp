// TGAtoolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TGAtool.h"
#include "TGAtoolDlg.h"

#include <fcntl.h>
#include <io.h>
#include "..\..\sys\Exception.hpp"
#include "..\..\sys\ImageFormat.hpp"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




/////////////////////////////////////////////////////////////////////////////
// CTGAtoolDlg dialog

CTGAtoolDlg::CTGAtoolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTGAtoolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTGAtoolDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTGAtoolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTGAtoolDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTGAtoolDlg, CDialog)
	//{{AFX_MSG_MAP(CTGAtoolDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_BN_CLICKED(IDC_PACK, OnPack)
	ON_BN_CLICKED(IDC_DEVAREA, OnDevarea)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTGAtoolDlg message handlers

BOOL CTGAtoolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


    file_list       = (CListCtrl*)GetDlgItem( IDC_FILELIST );
    log_window      = (CEdit*)GetDlgItem( IDC_LOG );
    out_name        = (CEdit*)GetDlgItem( IDC_OUTNAME );
    sequence_edit   = (CEdit*)GetDlgItem( IDC_DEVAREA_EDIT );
    checkbox        = (CButton*)GetDlgItem( IDC_WRITE_DEVAREA );

    out_name->SetWindowText( "output.tga" );
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTGAtoolDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CTGAtoolDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


//------------------------------------------------------------------------------

void 
CTGAtoolDlg::OnAdd() 
{
    OPENFILENAME    data; ::ZeroMemory( &data, sizeof(data) );
    char            names[4096] = "";
    char            title[4096] = "";

    data.lStructSize        = sizeof(OPENFILENAME);
    data.hwndOwner          = NULL;
    data.hInstance          = NULL;                 // ignored
    data.lpstrFilter        = "TGA-files\0*.tga\0\0";
    data.lpstrCustomFilter  = NULL;                 // unused
    data.nFilterIndex       = 1;

    data.nMaxCustFilter     = 0;                    // ignored
    data.nFilterIndex       = 0;                    // ignored
    data.lpstrFile          = names;
    data.nMaxFile           = 4096;//sizeof(names);
//    data.lpstrFileTitle     = title;
//    data.nMaxFileTitle      = sizeof(title);
    data.lpstrInitialDir    = NULL;
    data.lpstrTitle         = "Add .TGA files";
    data.Flags              = OFN_ALLOWMULTISELECT | OFN_EXPLORER;
    data.lpstrDefExt        = "tga";
    
    if( ::GetOpenFileName( &data ) )
    {
        strcpy( path, names );
        strcat( path, "\\" );
        
        char*   name = names + data.nFileOffset;

        while( true )
        {
            file_list->InsertItem( file_list->GetItemCount(), name );

            
            // get file name without digits

            char    pure_name[128];
            char*   end;

            strcpy( pure_name, name );
            end = strchr( pure_name, '.' );
            --end;
            while( isdigit(*end) )
                --end;
            *(end+1) = '\0';
            strcat( pure_name, ".tga" );

            char    title[256];

            sprintf( title, "TGAtool - %s", pure_name );
            ::SetWindowText( this->m_hWnd, title );
            ::SetWindowText( out_name->m_hWnd, pure_name );


            name += strlen(name) + 1;
            if( *name == '\0' )
                break;
        }
    }
	         ASSERT
}


//------------------------------------------------------------------------------

void 
CTGAtoolDlg::OnDel() 
{
    POSITION pos;
    
    while( file_list->GetSelectedCount() )
    {
        pos = file_list->GetFirstSelectedItemPosition();

        if( pos ) 
            file_list->DeleteItem( file_list->GetNextSelectedItem(pos) );
    }
}


//------------------------------------------------------------------------------

void
CTGAtoolDlg::log( const char* msg )
{
    CString text;

    log_window->GetWindowText( text );
    text += msg;
    log_window->SetWindowText( text );
    log_window->LineScroll( log_window->GetLineCount() );
}


//------------------------------------------------------------------------------

void CTGAtoolDlg::OnPack() 
{
    int     n_images        = file_list->GetItemCount();
    int     n_digits        = 1;
    char    name[64];
//    bool    write_devarea   = false;


    CString nm;
    out_name->GetWindowText( nm );
    strcpy( name, nm );

    TGAheader   out_header;
    FILE*       out         = fopen( name, "wb+" );

    if( !out )
    {
        log( core::Msg("can't open \"%s\" for writing\r\n", name).text().c_str() ); 
        return;
    }

    
    fseek( out, sizeof(TGAheader), SEEK_SET );

    log(core::Msg("\r\nPacking images:\r\n").text().c_str());
    for( int i=0; i<n_images; i++ )
    {
        char    name[128];
        char    fullname[512];

        strcpy( fullname, path );
        file_list->GetItemText( i, 0, name, sizeof(name) );
        strcat( fullname, name );
        
        FILE*       img = fopen( fullname, "rb" ); 
        TGAheader   header;

        if( !img )
        {
            log( core::Msg("can't open \"%s\" for reading\r\n", name).text().c_str() ); 
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
            log( "all images should have the same width,heigth,bpp "
                 "& descriptor\r\n" 
               ); 
            fcloseall();
            return;
        }

        log( core::Msg("adding \"%s\"...",name).text().c_str() );
        for( long p=0; p<header.width*header.height*header.bpp/8; p++ )
        {
            char    ch;

            fread( &ch, sizeof(char), 1, img );
            fwrite( &ch, sizeof(char), 1, out );
        }

        fclose( img );
        log( "done\r\n");
        //printf( "image %i: %i bytes written\n", i, 
        //        header.width*header.height*header.bpp/8 );
    }

    
    // write footer
    
    long    dev_area_offset = 0;//sizeof(TGAheader) + n_images * 
                              //out_header.width*out_header.height
                              //*out_header.bpp/8;
    long    xt_area_offset  = 0x00000000;
    
    fwrite( &xt_area_offset, sizeof(UDword), 1, out );
    fwrite( &dev_area_offset, sizeof(UDword), 1, out );
    fwrite( "TRUEVISION-XFILE.\0", 18, 1, out );

    
    // write header
    
    out_header.height  *= n_images;

    fseek( out, 0, SEEK_SET );
    fwrite( &out_header, sizeof(TGAheader), 1, out );

    fcloseall(); 
    log(core::Msg("%i images packed\r\n",n_images).text().c_str());

    if( checkbox->GetCheck() )
        OnDevarea();
}


//------------------------------------------------------------------------------

void 
CTGAtoolDlg::OnDevarea() 
{
    CString     name;
    out_name->GetWindowText( name );
    
    FILE*       tga_file = fopen( name, "rb+" ); 
    TGAfooter   footer;
    long        dev_offset;

    if( !tga_file )
    {
        log( core::Msg("can't open \"%s\"\n", name ).text().c_str() );
        fcloseall();
        return;
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

    int handle = _open( name, _O_RDWR );

    if( handle == -1 )
    {
        log( core::Msg("can't open \"%s\"\n", name).text().c_str() );
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
        log( core::Msg( "\"%s\" is not a valid TGA file "
                        "(or its format is obsolete)\r\n",
                        name
                      ).text().c_str()
              );
        fcloseall();
        return; 
    }

    
    fseek( tga_file, dev_offset, SEEK_SET );
    
    char            sequence[128];
    CString         sq;
    
    sequence_edit->GetWindowText( sq );
    strcpy( sequence, sq );
   
    const char*     p1          = sequence;
    const char*     p2          = p1;
    int             n_bytes     = 0;

    log( "\r\nWriting dev-area:\r\n" );
    while( true )
    {
        while( *p2  &&  (*p2!=',') )
        {    
            if( !isxdigit(*p2)  &&  *p2!='x' )
            {
                log( core::Msg( "not a number in sequence (%c)\n", 
                                 *p2
                              ).text().c_str() 
                   );
                return; 
            }
            ++p2;
        }

        char    n[16];
        UDword  d;
        strncpy( n, p1, p2-p1 );
        n[ p2-p1 ] = '\0';
        sscanf( n, "%i", &d );

        log( core::Msg("%i\r\n", d).text().c_str() );
        fwrite( &d, sizeof(UDword), 1, tga_file );
        n_bytes += 4;

        if( *p2 == '\0' )
            break;

        p1 = ++p2;
    }
    log( core::Msg( "%i bytes written\r\n",n_bytes ).text().c_str() );

    // write footer
    
    footer.developer_area_offset = dev_offset;
    fwrite( &footer.extension_area_offset, sizeof(UDword), 1, tga_file );
    fwrite( &footer.developer_area_offset, sizeof(UDword), 1, tga_file );
    fwrite( &footer.signature, sizeof(UByte), 16, tga_file );
    fwrite( &footer.dot, sizeof(UByte), 1, tga_file );
    fwrite( &footer.null, sizeof(UByte), 1, tga_file );

    fclose( tga_file );
}
