#include <windows.h>
#include <windowsx.h>
#include <stdio.h>

#include "..\..\sys\Win32Error.hpp"
#include "resource.h"

const int   NumVars          = 15;
const int   VarNameMaxLen    = 20;


enum
VarType
{
    as_int      = 16,
    as_float
};

HMODULE Toster;
HWND    TosterWin;

int     Var_NameID[NumVars];
char    Var_Name[NumVars][VarNameMaxLen];
int     Var_ValueID[NumVars];
int     IntVar_Value[NumVars];
float   FloatVar_Value[NumVars];
VarType Var_Type[NumVars];
int     IntVar_MinValue[NumVars];
int     IntVar_MaxValue[NumVars];
float   FloatVar_MinValue[NumVars];
float   FloatVar_Delta[NumVars];
int*    IntVar_Ptr[NumVars];
float*  FloatVar_Ptr[NumVars];
int     Var_ScrollbarID[NumVars];

int     NumUsedVars             = 0;

void (*apply_callback)()        = 0;

bool    InstantChange = false;


const int   NumColorEdits       = 8;
int         NumUsedColorEdits   = 0;

int             Color_NameID[NumColorEdits];
char            Color_Name[NumColorEdits][VarNameMaxLen];
unsigned int*   Color_ValuePtr[NumColorEdits];
unsigned int    Color_Value[NumColorEdits];


const int   NumListboxes        = 3;
int         NumUsedListboxes    = 0;

int         List_NameID[NumListboxes];
char        List_Name[NumListboxes][VarNameMaxLen];
char        List_Item[NumListboxes][255];
int         List_ListboxID[NumListboxes];
int*        List_ValuePtr[NumListboxes];
int         List_Value[NumListboxes];


//------------------------------------------------------------------------------

BOOL WINAPI
DllMain( HINSTANCE hinstDLL,  // handle to the DLL module
         DWORD reason,     // reason for calling function
         LPVOID lpvReserved   // reserved
       )
{
    switch( reason )
    {
        case DLL_PROCESS_ATTACH :
        case DLL_THREAD_ATTACH :
        case DLL_THREAD_DETACH :
        case DLL_PROCESS_DETACH :
            break;
    }
    return TRUE;
}


//------------------------------------------------------------------------------

void
update_controls()
{
    int i;

    for( i=0; i<NumUsedVars; i++ )
    {
        HWND    scrollbar   = GetDlgItem( TosterWin, Var_ScrollbarID[i] );
        HWND    value       = GetDlgItem( TosterWin, Var_ValueID[i] );
        char    str[128];

        SetScrollPos( scrollbar, SB_CTL, IntVar_Value[i], FALSE );
        UpdateWindow( scrollbar );

        if( Var_Type[i] == as_int )
        {
            sprintf( str, "%i", IntVar_Value[i] );
        }
        else
        {
            FloatVar_Value[i] = FloatVar_MinValue[i] +
                                IntVar_Value[i] * FloatVar_Delta[i];
            sprintf( str, "%4.4f", FloatVar_Value[i] );
        }

        SetWindowText( value, str );
        UpdateWindow( value );
    }

    for( i=0; i<NumUsedColorEdits; i++ )
    {
        HWND    name    = GetDlgItem( TosterWin, Color_NameID[i] );
        char    str[20];

        SetWindowText( name, Color_Name[i] );
        ShowWindow( name, SW_SHOW );
    }

    for( i=0; i<NumUsedListboxes; i++ )
    {
        HWND    name = GetDlgItem( TosterWin, List_NameID[i] );
        HWND    list = GetDlgItem( TosterWin, List_ListboxID[i] );

        //SendMessage( list, LB_SETCURSEL, List_Value[i], 0 );
        ShowWindow( list, SW_SHOW );

        SetWindowText( name, List_Name[i] );
        ShowWindow( name, SW_SHOW );
    }

    InvalidateRect( TosterWin, NULL, FALSE );
    UpdateWindow( TosterWin );
}


//------------------------------------------------------------------------------

__declspec( dllexport ) void
PrintConsole( char* str )
{
    HWND    console = GetDlgItem( TosterWin, IDC_CONSOLE );

    SetWindowText( console, str );
    UpdateWindow( console );
}


//------------------------------------------------------------------------------

void
Apply()
{
    for( int i=0; i<NumUsedVars; i++ )
    {
        if( Var_Type[i] == as_int )
            *IntVar_Ptr[i] = IntVar_Value[i];
        else
            *FloatVar_Ptr[i] = FloatVar_Value[i];
    }

    for( i=0; i<NumUsedColorEdits; i++ )
    {
        *(Color_ValuePtr[i]) = Color_Value[i];
    }

    for( i=0; i<NumUsedListboxes; i++ )
    {
        *(List_ValuePtr[i]) = List_Value[i];
    }

    //HWND    state = GetDlgItem( TosterWin, IDC_STATE );

    //SetWindowText( state, "applying..." );
    //UpdateWindow( state );

    apply_callback();

    //SetWindowText( state, "READY" );
    //UpdateWindow( state );
}


//------------------------------------------------------------------------------

void
save_sav( const char* filename = "default.sav" )
{
    FILE*   file = fopen( filename, "w+" );

    if( !file )
    {
        MessageBox( NULL, "can't open \"toster.sav\" for writing", "Error",
                    MB_OK|MB_ICONEXCLAMATION );
        return;
    }

    fprintf( file, "Toster save-file\n\n" );

    for( int i=0; i<NumUsedVars; i++ )
    {
        if( Var_Type[i] == as_int )
        {
            fprintf( file, "%-32s = %i\n", Var_Name[i], IntVar_Value[i] );
        }
        else
        {
            fprintf( file, "%-32s = %f\n", Var_Name[i], FloatVar_Value[i] );
        }
    }

    for( i=0; i<NumUsedColorEdits; i++ )
    {
        fprintf( file, "%-32s = %08X\n", Color_Name[i], Color_Value[i] );
    }
    
    for( i=0; i<NumUsedListboxes; i++ )
    {
        fprintf( file, "%-32s = %i\n", List_Name[i], List_Value[i] );
    }

    fclose( file );
}


//------------------------------------------------------------------------------

void
load_sav( const char* filename = "default.sav" )
{
    FILE*   sav         = fopen( filename, "r" );
    char    line[80];

    if( !sav )
        return;

    fgets( line, 80, sav );
    if( strncmp( line, "Toster save-file", strlen("Toster save-file") ) )
    {
        fclose( sav );
        return;
    }

    while( !feof(sav) )
    {
        if( !fgets( line, 80, sav ) )
            break;

        char*   eq  = strchr( line, '=' );
        char*   v   = eq;

        if( !eq ) continue;

        --eq;
        while( *eq == ' ' ) --eq;

        char    name[80];

        strncpy( name, line, eq-line+1 );
        name[eq-line+1] = '\0';
        //char s[80];
        //sprintf( s, "'%s'", name );
        //MessageBox( NULL, s, "ddd", MB_OK );

        for( int i=0; i<NumUsedVars; i++ )
        {

            if( strcmp( Var_Name[i], name ) )
                continue;

            if( Var_Type[i] == as_int )
            {
                sscanf( v, "= %i", &IntVar_Value[i] );
            }
            else if( Var_Type[i] == as_float )
            {
                sscanf( v, "= %f", &FloatVar_Value[i] );
                IntVar_Value[i] = (FloatVar_Value[i]-FloatVar_MinValue[i]) /
                                   FloatVar_Delta[i];
            }
        }

        for( i=0; i<NumUsedColorEdits; i++ )
        {
            if( strcmp( Color_Name[i], name ) )
                continue;

            sscanf( v, "= %08X", &Color_Value[i] );
        }

        for( i=0; i<NumUsedListboxes; i++ )
        {
            if( strcmp( List_Name[i], name ) )
                continue;

            sscanf( v, "= %08X", &List_Value[i] );
        }
    }
    fclose( sav );
}


//------------------------------------------------------------------------------

void
Load()
{
    OPENFILENAME    info;
    char            name[255]       = "default.sav";
    char            full_path[255]  = "default.sav";
    char            cur_dir[255];

    GetCurrentDirectory( 255, cur_dir );


    info.lStructSize        = sizeof( OPENFILENAME );
    info.hwndOwner          = NULL;
    info.hInstance          = NULL;
    info.lpstrFilter        = "Toster sav-files\0*.sav\0\0";
    info.lpstrCustomFilter  = NULL;
    info.nMaxCustFilter     = 0;
    info.nFilterIndex       = 0;
    info.lpstrFile          = full_path;
    info.nMaxFile           = 255;
    info.lpstrFileTitle     = name;
    info.nMaxFileTitle      = strlen(name);
    info.lpstrInitialDir    = cur_dir;
    info.lpstrTitle         = "Load Toster .sav";
    info.Flags              = OFN_HIDEREADONLY;
    //info.nFileOffset        = ;
    //info.nFileExtension;
    info.lpstrDefExt        = "sav";
    //info.lCustData;
    //info.lpfnHook;
    //info.lpTemplateName;

    if( GetOpenFileName( &info ) )
    {
        load_sav( full_path );
        update_controls();
        Apply();
    }
}


//------------------------------------------------------------------------------



//------------------------------------------------------------------------------

void
Save()
{
    OPENFILENAME    info;
    char            name[255]       = "default.sav";
    char            full_path[255]  = "default.sav";
    char            cur_dir[255];

    GetCurrentDirectory( 255, cur_dir );


    info.lStructSize        = sizeof( OPENFILENAME );
    info.hwndOwner          = NULL;
    info.hInstance          = NULL;
    info.lpstrFilter        = "Toster sav-files\0*.sav\0\0";
    info.lpstrCustomFilter  = NULL;
    info.nMaxCustFilter     = 0;
    info.nFilterIndex       = 0;
    info.lpstrFile          = full_path;
    info.nMaxFile           = 255;
    info.lpstrFileTitle     = name;
    info.nMaxFileTitle      = strlen(name);
    info.lpstrInitialDir    = cur_dir;
    info.lpstrTitle         = "Save to Toster .sav";
    info.Flags              = OFN_HIDEREADONLY;
    //info.nFileOffset        = ;
    //info.nFileExtension;
    info.lpstrDefExt        = "sav";
    //info.lCustData;
    //info.lpfnHook;
    //info.lpTemplateName;

    if( GetSaveFileName( &info ) )
    {
        save_sav( full_path );
    }
}


//------------------------------------------------------------------------------

__declspec( dllexport ) BOOL CALLBACK
TosterProc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
    BOOL            success;
    char            str[20];
    int             i;
    HWND            scrollbar;
    HWND            value;
    LRESULT         result;
    HDC             dc;
    RECT            client_rect;
    static COLORREF custom_colors[16];


    switch( msg )
    {
        case WM_INITDIALOG :
            update_controls();
            return TRUE;
            break;

        case WM_HSCROLL :
            scrollbar   = (HWND)lparam;
            for( i=0; i<NumUsedVars; i++ )
            {
                if( GetDlgItem(TosterWin,Var_ScrollbarID[i]) == scrollbar )
                {
                    //MessageBox( NULL, "ww", "ww", MB_OK );
                    value = GetDlgItem(TosterWin,Var_ValueID[i]);
                    break;
                }
            }
            switch( LOWORD(wparam) )
            {
                case SB_LINELEFT :
                    --IntVar_Value[i];
                    break;

                case SB_LINERIGHT :
                    //MessageBox( NULL, "ww", "ww", MB_OK );
                    ++IntVar_Value[i];
                    break;

                case SB_THUMBPOSITION :
                case SB_THUMBTRACK :
                    IntVar_Value[i] = HIWORD(wparam);
                    break;

            }

            SetScrollPos( scrollbar, SB_CTL, IntVar_Value[i], FALSE );
            if( Var_Type[i] == as_int )
            {
                sprintf( str, "%i", IntVar_Value[i] );
            }
            else
            {
                FloatVar_Value[i] = FloatVar_MinValue[i] +
                                    IntVar_Value[i] * FloatVar_Delta[i];
                sprintf( str, "%4.4f", FloatVar_Value[i] );
            }
            SetWindowText( value, str );

            if( InstantChange && apply_callback )
                Apply();

            UpdateWindow( scrollbar );
            UpdateWindow( value );
            break; // case WM_HSCROLL

        case WM_COMMAND :
            if( (HWND)lparam == GetDlgItem( TosterWin, IDC_APPLY ) )
                Apply();
            else if( (HWND)lparam == GetDlgItem( TosterWin, IDC_SAVE ) )
                Save();
            else if( (HWND)lparam == GetDlgItem( TosterWin, IDC_LOAD ) )
                Load();
            else if( (HWND)lparam == GetDlgItem( TosterWin, IDC_INSTANTCHANGE) )
            {
                if( HIWORD(wparam) == BN_CLICKED )
                {
                    if( InstantChange )
                        InstantChange = false;
                    else
                    {
                        InstantChange = true;
                        Apply();
                    }
                }
            }
            else if( HIWORD(wparam) == CBN_SELCHANGE )
            {
                for( int i=0; i<NumUsedListboxes; i++ ) 
                {
                    if( List_ListboxID[i] != LOWORD(wparam) )
                        continue;

                    HWND    list = GetDlgItem( TosterWin, List_ListboxID[i] );
                    int     item = SendMessage( list, CB_GETCURSEL, 0, 0 );

                    List_Value[i] = item;
                }
                
                if( InstantChange && apply_callback )
                    Apply();
            }


            break; // case WM_COMMAND

        case WM_LBUTTONDOWN :
            for( i=0; i<NumUsedColorEdits; i++ )
            {
                GetClientRect( TosterWin, &client_rect );

                int     left    = client_rect.right - 150 -5;
                int     top     = NumVars*20 + 35 + 23*i;
                int     right   = client_rect.right - 150 -5 + 80;
                int     bottom  = NumVars*20 + 35 + 23*i + 20;

                int     mouse_x = LOWORD( lparam );
                int     mouse_y = HIWORD( lparam );

                if( mouse_x > left  &&  mouse_x < right &&
                    mouse_y > top  &&  mouse_y < bottom
                  )
                {
                    CHOOSECOLOR     cc;

                    cc.lStructSize      = sizeof(cc);
                    cc.hwndOwner        = NULL;
                    cc.hInstance        = NULL;
                    cc.rgbResult        = Color_Value[i];//RGB(255,255,255);
                    cc.lpCustColors     = custom_colors;
                    cc.Flags            = CC_ANYCOLOR |
                                          CC_FULLOPEN |
                                          CC_RGBINIT;
                    cc.lCustData        = 0L;
                    cc.lpfnHook         = NULL;
                    cc.lpTemplateName   = NULL;

                    if( ChooseColor( &cc ) )
                    {
                        Color_Value[i] = cc.rgbResult;
                        update_controls();

                        if( InstantChange && apply_callback )
                            Apply();
                    }

                }
            }
            break; // case WM_LBUTTONDOWN


        case WM_PAINT :
            dc = GetDC( TosterWin );
            GetClientRect( TosterWin, &client_rect );

            for( i=0; i<NumUsedColorEdits; i++ )
            {
                HPEN        contour   = CreatePen( PS_SOLID, 1, 0/*Color_Value[i]*/ );
                LOGBRUSH    rect_desc = { BS_SOLID, Color_Value[i], 0 };
                HBRUSH      rect      = CreateBrushIndirect( &rect_desc );

                SelectObject( dc, contour );
                SelectObject( dc, rect );

                Rectangle( dc,
                           client_rect.right - 150 -5,
                           NumVars*20 + 35 + 23*i,
                           client_rect.right - 150 -5 + 80,
                           NumVars*20 + 35 + 23*i + 20
                         );

                CloseHandle( contour );
                CloseHandle( rect );
            }

            ReleaseDC( TosterWin, dc );
            //return DefDlgProv()
            break; // case WM_PAINT


        /*
        case WM_CREATE :
        case WM_GETMINMAXINFO :
            return 0;
            break;

        case WM_NCCREATE :
            return TRUE;
            break;

        case WM_INITDIALOG :
            return TRUE;
            break;

        case WM_DESTROY :
            EndDialog( TosterWin, 0 );
            break;
        */

        //default:
        //    return DefDlgProc( wnd, msg, wparam, lparam );
    }

    return FALSE;

}


//------------------------------------------------------------------------------

__declspec( dllexport ) void
AddIntVariable( int* var, int min, int max, char name[] )
{
    if( NumUsedVars == NumVars )
    {
        MessageBox( NULL, "Too many variables", "Info",
                    MB_OK|MB_ICONINFORMATION
                  );
        return;
    }

    Var_Type[NumUsedVars]           = as_int;
    IntVar_Ptr[NumUsedVars]         = var;
    IntVar_Value[NumUsedVars]       = *var;
    IntVar_MinValue[NumUsedVars]    = min;
    IntVar_MaxValue[NumUsedVars]    = max;
    strcpy( Var_Name[NumUsedVars], name );
    ++NumUsedVars;
}


//------------------------------------------------------------------------------

__declspec( dllexport ) void
AddFloatVariable( float* var, float min, float max, int num_steps, char name[] )
{
    if( NumUsedVars == NumVars )
    {
        MessageBox( NULL, "Too many variables", "Info",
                    MB_OK|MB_ICONINFORMATION
                  );
        return;
    }

    Var_Type[NumUsedVars]           = as_float;
    FloatVar_Ptr[NumUsedVars]       = var;
    FloatVar_Value[NumUsedVars]     = *var;
    IntVar_MinValue[NumUsedVars]    = 0;
    IntVar_MaxValue[NumUsedVars]    = num_steps;
    FloatVar_MinValue[NumUsedVars]  = min;
    FloatVar_Delta[NumUsedVars]     = float(max - min) / num_steps;
    IntVar_Value[NumUsedVars]       = (*var) / FloatVar_Delta[NumUsedVars];
    strcpy( Var_Name[NumUsedVars], name );
    ++NumUsedVars;
}


//------------------------------------------------------------------------------

__declspec( dllexport ) void
AddColorVariable( unsigned int* color, char name[] )
{
    if( NumUsedColorEdits == NumColorEdits )
    {
        MessageBox( NULL, "Too many color edits", "Info",
                    MB_OK|MB_ICONINFORMATION
                  );

        return;
    }

    Color_ValuePtr[NumUsedColorEdits]   = color;
    Color_Value[NumUsedColorEdits]      = *color;
    strcpy( Color_Name[NumUsedColorEdits], name );

    ++NumUsedColorEdits;
}

//------------------------------------------------------------------------------

__declspec( dllexport ) void
AddList( int* value, char items[], char name[] )
{
    if( NumUsedListboxes == NumListboxes )
    {
        MessageBox( NULL, "Too many listboxes edits", "Info",
                    MB_OK|MB_ICONINFORMATION
                  );

        return;
    }

    List_ValuePtr[NumUsedListboxes] = value;
    List_Value[NumUsedListboxes]    = *value;
    strcpy( List_Name[NumUsedListboxes], name );
    strcpy( List_Item[NumUsedListboxes], items );

    ++NumUsedListboxes;
}

//------------------------------------------------------------------------------

__declspec( dllexport ) void
RegisterApplyCallback( VOID* callback )
{
    apply_callback = ( void(*)() ) callback;
}


//------------------------------------------------------------------------------

void
setup_toster()
{
    Var_NameID[0]       = IDC_NAME01;
    Var_ValueID[0]      = IDC_VALUE01;
    Var_ScrollbarID[0]  = IDC_SCROLLBAR01;

    Var_NameID[1]       = IDC_NAME02;
    Var_ValueID[1]      = IDC_VALUE02;
    Var_ScrollbarID[1]  = IDC_SCROLLBAR02;

    Var_NameID[2]       = IDC_NAME03;
    Var_ValueID[2]      = IDC_VALUE03;
    Var_ScrollbarID[2]  = IDC_SCROLLBAR03;

    Var_NameID[3]       = IDC_NAME04;
    Var_ValueID[3]      = IDC_VALUE04;
    Var_ScrollbarID[3]  = IDC_SCROLLBAR04;

    Var_NameID[4]       = IDC_NAME05;
    Var_ValueID[4]      = IDC_VALUE05;
    Var_ScrollbarID[4]  = IDC_SCROLLBAR05;

    Var_NameID[5]       = IDC_NAME06;
    Var_ValueID[5]      = IDC_VALUE06;
    Var_ScrollbarID[5]  = IDC_SCROLLBAR06;

    Var_NameID[6]       = IDC_NAME07;
    Var_ValueID[6]      = IDC_VALUE07;
    Var_ScrollbarID[6]  = IDC_SCROLLBAR07;

    Var_NameID[7]       = IDC_NAME08;
    Var_ValueID[7]      = IDC_VALUE08;
    Var_ScrollbarID[7]  = IDC_SCROLLBAR08;

    Var_NameID[8]       = IDC_NAME09;
    Var_ValueID[8]      = IDC_VALUE09;
    Var_ScrollbarID[8]  = IDC_SCROLLBAR09;

    Var_NameID[9]       = IDC_NAME10;
    Var_ValueID[9]      = IDC_VALUE10;
    Var_ScrollbarID[9]  = IDC_SCROLLBAR10;

    Var_NameID[10]      = IDC_NAME11;
    Var_ValueID[10]     = IDC_VALUE11;
    Var_ScrollbarID[10] = IDC_SCROLLBAR11;

    Var_NameID[11]      = IDC_NAME12;
    Var_ValueID[11]     = IDC_VALUE12;
    Var_ScrollbarID[11] = IDC_SCROLLBAR12;

    Var_NameID[12]      = IDC_NAME13;
    Var_ValueID[12]     = IDC_VALUE13;
    Var_ScrollbarID[12] = IDC_SCROLLBAR13;

    Var_NameID[13]      = IDC_NAME14;
    Var_ValueID[13]     = IDC_VALUE14;
    Var_ScrollbarID[13] = IDC_SCROLLBAR14;

    Var_NameID[14]      = IDC_NAME15;
    Var_ValueID[14]     = IDC_VALUE15;
    Var_ScrollbarID[14] = IDC_SCROLLBAR15;

    Color_NameID[0]     = IDC_COLOR0;
    Color_NameID[1]     = IDC_COLOR1;
    Color_NameID[2]     = IDC_COLOR2;
    Color_NameID[3]     = IDC_COLOR3;
    Color_NameID[4]     = IDC_COLOR4;
    Color_NameID[5]     = IDC_COLOR5;
    Color_NameID[6]     = IDC_COLOR6;
    Color_NameID[7]     = IDC_COLOR7;

    List_NameID[0]      = IDC_LISTNAME1;
    List_ListboxID[0]   = IDC_LIST1;

    List_NameID[1]      = IDC_LISTNAME2;
    List_ListboxID[1]   = IDC_LIST2;

    List_NameID[2]      = IDC_LISTNAME3;
    List_ListboxID[2]   = IDC_LIST3;

    RECT    client_rect;

    GetClientRect( TosterWin, &client_rect );
    SetWindowPos( TosterWin, 
                  HWND_TOP, 
                  GetSystemMetrics(SM_CXFULLSCREEN)-300,
                  0, 
                  300, 
                  GetSystemMetrics(SM_CYFULLSCREEN), 
                  SWP_NOACTIVATE|SWP_NOZORDER|SWP_FRAMECHANGED
                );


    // setup scrollbars

    for( int i=0; i<NumVars; i++ )
    {
        HWND  name      = GetDlgItem( TosterWin, Var_NameID[i] );
        HWND  value     = GetDlgItem( TosterWin, Var_ValueID[i] );
        HWND  scrollbar = GetDlgItem( TosterWin, Var_ScrollbarID[i] );

        SetWindowPos( scrollbar,
                      HWND_TOP,
                      client_rect.right - 150 -5,
                      35 + 20*i,
                      150,
                      15,
                      SWP_NOACTIVATE|SWP_NOZORDER|SWP_FRAMECHANGED
                    );
        ShowWindow( scrollbar, SW_HIDE );
        UpdateWindow( scrollbar );

        SetWindowPos( value,
                      HWND_TOP,
                      client_rect.right - 150 -5  -40 -5,
                      35 + 20*i,
                      40,
                      15,
                      SWP_NOACTIVATE|SWP_NOZORDER|SWP_FRAMECHANGED
                    );
        ShowWindow( value, SW_HIDE );
        UpdateWindow( value );

        SetWindowPos( name,
                      HWND_TOP,
                      5,
                      35 + 20*i,
                      client_rect.right - 150 -5  -40 -5 -5,
                      15,
                      SWP_NOACTIVATE|SWP_NOZORDER|SWP_FRAMECHANGED
                    );
        ShowWindow( name, SW_HIDE );
        UpdateWindow( name );
    }


    // setup color edits

    for( i=0; i<NumColorEdits; i++ )
    {
        HWND  name  = GetDlgItem( TosterWin, Color_NameID[i] );

        SetWindowPos( name,
                      HWND_TOP,
                      5,
                      NumVars*20 + 35 + 23*i,
                      client_rect.right - 150 -5  -40 -5 -5,
                      20,
                      SWP_NOACTIVATE|SWP_NOZORDER|SWP_FRAMECHANGED
                    );
        ShowWindow( name, SW_HIDE );
        UpdateWindow( name );
    }


    // setup listbox

    for( i=0; i<NumListboxes; i++ )
    {
        HWND    name = GetDlgItem( TosterWin, List_NameID[i] );
        HWND    list = GetDlgItem( TosterWin, List_ListboxID[i] );

        SetWindowPos( name,
                      HWND_TOP,
                      5,
                      NumVars*20 + NumColorEdits*20 + 35  + 35 + 23*i,
                      client_rect.right - 150 -5  -40 -5 -5,
                      20,
                      SWP_NOACTIVATE|SWP_NOZORDER|SWP_FRAMECHANGED
                    );
        ShowWindow( name, SW_HIDE );
        UpdateWindow( name );

        SetWindowPos( list,
                      HWND_TOP,
                      client_rect.right - 150 -5,
                      NumVars*20 + NumColorEdits*20 + 35 + 35 + 23*i,
                      80-5,
                      10,
                      SWP_NOACTIVATE|SWP_NOZORDER|SWP_FRAMECHANGED
                    );
        ShowWindow( list, SW_HIDE );
        UpdateWindow( list );

        char*   item = List_Item[i];
        int     n    = 0;
        bool    done = false;

        while( !done )
        {
            char    item_text[50];
            char*   end = strchr( item, ';' );

            if( !end )
            {
                strcpy( item_text, item );
                done = true;
            }
            else
            {
                strncpy( item_text, item, end-item );
                item_text[end-item] = '\0';
            }

            SendMessage( list, CB_INSERTSTRING,
                         n++, (LPARAM)item_text
                       );
            item = end+1;
        }

        SendMessage( list, CB_SETCURSEL, 0, 0 );

    } // for( i=0; i<NumListboxes; i++ )


    for( i=0; i<NumUsedVars; ++i )
    {
        HWND  name      = GetDlgItem( TosterWin, Var_NameID[i] );
        HWND  value     = GetDlgItem( TosterWin, Var_ValueID[i] );
        HWND  scrollbar = GetDlgItem( TosterWin, Var_ScrollbarID[i] );
        char  buf[10];

        SetScrollRange( scrollbar,
                        SB_CTL,
                        IntVar_MinValue[i],
                        IntVar_MaxValue[i],
                        FALSE
                      );

        SetScrollPos( scrollbar, SB_CTL, IntVar_Value[i], FALSE );
        SetWindowText( name, Var_Name[i] );

        if( Var_Type[i] == as_int )
            sprintf( buf, "%i", IntVar_Value[i] );
        else
            sprintf( buf, "%4.4f", FloatVar_Value[i] );

        SetWindowText( value, buf );

        ShowWindow( name, SW_SHOWNORMAL );
        UpdateWindow( name );
        ShowWindow( value, SW_SHOWNORMAL );
        UpdateWindow( value );
        ShowWindow( scrollbar, SW_SHOWNORMAL );
        UpdateWindow( scrollbar );
    }

    // setup apply

    HWND    apply   = GetDlgItem( TosterWin, IDC_APPLY );
    HWND    load    = GetDlgItem( TosterWin, IDC_LOAD );
    HWND    save    = GetDlgItem( TosterWin, IDC_SAVE );
    HWND    instant = GetDlgItem( TosterWin, IDC_INSTANTCHANGE );

    GetClientRect( TosterWin, &client_rect );
    SetWindowPos( apply,
                  HWND_TOP,
                  client_rect.right - 10 - 50,
                  client_rect.bottom - 10 - 30*2,
                  50,
                  20,
                  SWP_NOACTIVATE|SWP_NOZORDER|SWP_FRAMECHANGED
                );
    ShowWindow( apply, SW_SHOWNORMAL );
    UpdateWindow( apply );


    // setup save

    GetClientRect( TosterWin, &client_rect );
    SetWindowPos( save,
                  HWND_TOP,
                  client_rect.right - 10 - 50,
                  client_rect.bottom - 10 - 30,
                  50,
                  20,
                  SWP_NOACTIVATE|SWP_NOZORDER|SWP_FRAMECHANGED
                );
    ShowWindow( save, SW_SHOWNORMAL );
    UpdateWindow( save );


    // setup load

    GetClientRect( TosterWin, &client_rect );
    SetWindowPos( load,
                  HWND_TOP,
                  client_rect.right - 10 - 50 -50-10,
                  client_rect.bottom - 10 - 30,
                  50,
                  20,
                  SWP_NOACTIVATE|SWP_NOZORDER|SWP_FRAMECHANGED
                );
    ShowWindow( load, SW_SHOWNORMAL );
    UpdateWindow( load );


    // setup instant checkbox

    SetWindowPos( instant,
                  HWND_TOP,
                  client_rect.left + 10,
                  client_rect.bottom - 10 - 30,
                  150,
                  20,
                  SWP_NOACTIVATE|SWP_NOZORDER|SWP_FRAMECHANGED
                );
    //SendMessage( instant, BM_SETSTATE, BST_CHECKED, 0 );
    ShowWindow( instant, SW_SHOWNORMAL );
    UpdateWindow( instant );


    // setup state
    /*
    HWND    state = GetDlgItem( TosterWin, IDC_STATE );

    SetWindowPos( state,
                  HWND_TOP,
                  client_rect.right - 10 - 80,
                  client_rect.top + 1,
                  80,
                  15,
                  SWP_NOACTIVATE|SWP_NOZORDER|SWP_FRAMECHANGED
                );
    SetWindowText( state, "ready" );
    ShowWindow( state, SW_SHOWNORMAL );
    UpdateWindow( state );
    */

    // setup console

    HWND    console = GetDlgItem( TosterWin, IDC_CONSOLE );

    //SetWindowLong( console, GWL_STYLE, WS_VSCROLL );
    //SetWindowLong( console, GWL_STYLE, ES_MULTILINE );
    //SetWindowText( console, "toster 1.32\0READY\r" );
    SetWindowPos( console,
                  HWND_TOP,
                  10,
                  client_rect.top + 1,
                  client_rect.right - 20,
                  15,
                  SWP_NOACTIVATE|SWP_NOZORDER|SWP_FRAMECHANGED
                );
    ShowWindow( console, SW_SHOWNORMAL );
    UpdateWindow( console );


    UpdateWindow( TosterWin );
}


//------------------------------------------------------------------------------

__declspec( dllexport ) int
OpenToster()
{
    Toster = LoadLibrary( "toster.dll" );

    if( Toster == NULL )
    {
        MessageBox( NULL,
                    core::win32_error(GetLastError()).c_str(),
                    "error",
                    MB_OK | MB_ICONEXCLAMATION
                  );
        return -13;
    }

    TosterWin = CreateDialog( Toster,
                              MAKEINTRESOURCE(IDD_TOSTER),
                              NULL,
                              &TosterProc
                            );

    if( TosterWin == NULL )
    {
        MessageBox( NULL,
                    core::win32_error(GetLastError()).c_str(),
                    "error",
                    MB_OK | MB_ICONEXCLAMATION
                  );

        return -13;
    }

    setup_toster();
    load_sav();
    update_controls();
    Apply();

    ShowWindow( TosterWin, SW_SHOWNORMAL );
    UpdateWindow( TosterWin );


    return 0;
}

