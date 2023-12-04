//==============================================================================
//
//  DDraw error   
//
//==============================================================================
//
//  externals: 

    #include "DDrawError.hpp"

namespace core
{
//==============================================================================
//
//  publics:

std::string
ddraw_error( HRESULT code )
{
    switch( code )
    {
        case DDERR_ALREADYINITIALIZED :
            return std::string("already initialized"); 
            break;

        case DDERR_BLTFASTCANTCLIP :
            return std::string("bltfast() can't clip");
            break;

        case DDERR_CANNOTATTACHSURFACE :
            return std::string("can't attach surface");
            break; 

        case DDERR_CANNOTDETACHSURFACE :
            return std::string("can't detach surface");
            break;

        case DDERR_CANTCREATEDC :
            return std::string("can't create DC");
            break;

        case DDERR_CANTDUPLICATE :
            return std::string("can't duplicate");
            break;

        case DDERR_CANTLOCKSURFACE :
            return std::string("can't lock surface");
            break;

        case DDERR_CANTPAGELOCK :
            return std::string("can't pagelock");
            break;
        
        case DDERR_CANTPAGEUNLOCK :
            return std::string("can't pageunlock");
            break;
             
        case DDERR_CLIPPERISUSINGHWND :
            return std::string("clipper is using HWND");
            break;
             
        case DDERR_COLORKEYNOTSET :
            return std::string("colorkey not set");
            break;
             
        case DDERR_CURRENTLYNOTAVAIL :
            return std::string("currently not available");
            break;
        /*     
        case DDERR_DDSCAPSCOMPLEXREQUIRED :
            return std::string("DDSCAPSCOMPLEXREQUIRED");
            break;
        */     
        case DDERR_DCALREADYCREATED :
            return std::string("DC already created");
            break;
             
        case DDERR_DEVICEDOESNTOWNSURFACE :
            return std::string("device doesn't own surface");
            break;
             
        case DDERR_DIRECTDRAWALREADYCREATED :
            return std::string("ddraw already created");
            break;
             
        case DDERR_EXCEPTION :
            return std::string("exception");
            break;
             
        case DDERR_EXCLUSIVEMODEALREADYSET :
            return std::string("exclusive mode already set");
            break;
        /*     
        case DDERR_EXPIRED :
            return std::string("EXPIRED");
            break;
        */     
        case DDERR_GENERIC :
            return std::string("generic");
            break;
             
        case DDERR_HEIGHTALIGN :
            return std::string("height align");
            break;
             
        case DDERR_HWNDALREADYSET :
            return std::string("HWND already set");
            break;
             
        case DDERR_HWNDSUBCLASSED :
            return std::string("HWND subclassed");
            break;
             
        case DDERR_IMPLICITLYCREATED :
            return std::string("implicitly created");
            break;
             
        case DDERR_INCOMPATIBLEPRIMARY :
            return std::string("incompatible primary");
            break;
             
        case DDERR_INVALIDCAPS :
            return std::string("invalid caps");
            break;
             
        case DDERR_INVALIDCLIPLIST :
            return std::string("invalid cliplist");
            break;
             
        case DDERR_INVALIDDIRECTDRAWGUID :
            return std::string("invalid ddraw GUID");
            break;
             
        case DDERR_INVALIDMODE :
            return std::string("invalid mode");
            break;
             
        case DDERR_INVALIDOBJECT :
            return std::string("invalid object");
            break;
             
        case DDERR_INVALIDPARAMS :
            return std::string("invalid params");
            break;
             
        case DDERR_INVALIDPIXELFORMAT :
            return std::string("invalid pixel format");
            break;
             
        case DDERR_INVALIDPOSITION :
            return std::string("invalid position");
            break;
             
        case DDERR_INVALIDRECT :
            return std::string("invalid rect");
            break;
        /*     
        case DDERR_INVALIDSTREAM :
            return std::string("INVALIDSTREAM");
            break;
        */     
        case DDERR_INVALIDSURFACETYPE :
            return std::string("invalid surface type");
            break;
             
        case DDERR_LOCKEDSURFACES :
            return std::string("locked surfaces");
            break;
             
        case DDERR_MOREDATA :
            return std::string("more data");
            break;
        /*     
        case DDERR_NEWMODE :
            return std::string("NEWMODE");
            break;
        */     
        case DDERR_NO3D :
            return std::string("no 3D");
            break;
             
        case DDERR_NOALPHAHW :
            return std::string("no alpha hw");
            break;
             
        case DDERR_NOBLTHW :
            return std::string("no blt hw");
            break;
             
        case DDERR_NOCLIPLIST :
            return std::string("no cliplist");
            break;
             
        case DDERR_NOCLIPPERATTACHED :
            return std::string("no clipper attached");
            break;
             
        case DDERR_NOCOLORCONVHW :
            return std::string("no colorconv hw");
            break;
             
        case DDERR_NOCOLORKEY :
            return std::string("no colorkey");
            break;
             
        case DDERR_NOCOLORKEYHW :
            return std::string("no colorkey hw");
            break;
             
        case DDERR_NOCOOPERATIVELEVELSET :
            return std::string("no cooperative level set");
            break;
             
        case DDERR_NODC :
            return std::string("no DC");
            break;
             
        case DDERR_NODDROPSHW :
            return std::string("no ddrops hw");
            break;
             
        case DDERR_NODIRECTDRAWHW :
            return std::string("no ddraw hw");
            break;
             
        case DDERR_NODIRECTDRAWSUPPORT :
            return std::string("no ddraw support");
            break;
        /*     
        case DDERR_NODRIVERSUPPORT :
            return std::string("NODRIVERSUPPORT");
            break;
        */     
        case DDERR_NOEMULATION :
            return std::string("no emulation");
            break;
             
        case DDERR_NOEXCLUSIVEMODE :
            return std::string("no exclusive mode");
            break;
             
        case DDERR_NOFLIPHW :
            return std::string("no flip hw");
            break;
             
        case DDERR_NOFOCUSWINDOW :
            return std::string("no focus window");
            break;
             
        case DDERR_NOGDI :
            return std::string("no GDI");
            break;
             
        case DDERR_NOHWND :
            return std::string("no HWND");
            break;
             
        case DDERR_NOMIPMAPHW :
            return std::string("no mipmap hw");
            break;
             
        case DDERR_NOMIRRORHW :
            return std::string("no mirror hw");
            break;
        /*     
        case DDERR_NOMONITORINFORMATION :
            return std::string("NOMONITORINFORMATION");
            break;
        */     
        case DDERR_NONONLOCALVIDMEM :
            return std::string("no nonlocal vidmem");
            break;
             
        case DDERR_NOOPTIMIZEHW :
            return std::string("no optimize hw");
            break;
             
        case DDERR_NOOVERLAYDEST :
            return std::string("no overlay dest");
            break;
             
        case DDERR_NOOVERLAYHW :
            return std::string("no overlay hw");
            break;
             
        case DDERR_NOPALETTEATTACHED :
            return std::string("no palette attached");
            break;
             
        case DDERR_NOPALETTEHW :
            return std::string("no palette hw");
            break;
             
        case DDERR_NORASTEROPHW :
            return std::string("no rasterop hw");
            break;
             
        case DDERR_NOROTATIONHW :
            return std::string("no rotation hw");
            break;
        /*     
        case DDERR_NOSTEREOHARDWARE :
            return std::string("NOSTEREOHARDWARE");
            break;
        */     
        case DDERR_NOSTRETCHHW :
            return std::string("no stretch hw");
            break;
        /*     
        case DDERR_NOSURFACELEFT :
            return std::string("NOSURFACELEFT");
            break;
        */     
        case DDERR_NOT4BITCOLOR :
            return std::string("not 4bit color");
            break;
             
        case DDERR_NOT4BITCOLORINDEX :
            return std::string("not 4bit colorindex");
            break;
             
        case DDERR_NOT8BITCOLOR :
            return std::string("not 8bit color");
            break;
             
        case DDERR_NOTAOVERLAYSURFACE :
            return std::string("not overlay surface");
            break;
             
        case DDERR_NOTEXTUREHW :
            return std::string("no texture hw");
            break;
             
        case DDERR_NOTFLIPPABLE :
            return std::string("not flippable");
            break;
             
        case DDERR_NOTFOUND :
            return std::string("not found");
            break;
             
        case DDERR_NOTINITIALIZED :
            return std::string("not initialized");
            break;
             
        case DDERR_NOTLOADED :
            return std::string("not loaded");
            break;
             
        case DDERR_NOTLOCKED :
            return std::string("not locked");
            break;
             
        case DDERR_NOTPAGELOCKED :
            return std::string("not pagelocked");
            break;
             
        case DDERR_NOTPALETTIZED :
            return std::string("not palettized");
            break;
             
        case DDERR_NOVSYNCHW :
            return std::string("no vsync hw");
            break;
             
        case DDERR_NOZBUFFERHW :
            return std::string("no zbuffer hw");
            break;
             
        case DDERR_NOZOVERLAYHW :
            return std::string("no zoverlay hw");
            break;
             
        case DDERR_OUTOFCAPS :
            return std::string("out of caps");
            break;
             
        case DDERR_OUTOFMEMORY :
            return std::string("out of memory");
            break;
             
        case DDERR_OUTOFVIDEOMEMORY :
            return std::string("out of videomemory");
            break;
        /*     
        case DDERR_OVERLAPPINGRECTS :
            return std::string("OVERLAPPINGRECTS");
            break;
        */     
        case DDERR_OVERLAYCANTCLIP :
            return std::string("overlay can't clip");
            break;
             
        case DDERR_OVERLAYCOLORKEYONLYONEACTIVE :
            return std::string("overlay colorkey only one active");
            break;
             
        case DDERR_OVERLAYNOTVISIBLE :
            return std::string("overlay not visible");
            break;
             
        case DDERR_PALETTEBUSY :
            return std::string("palette busy");
            break;
             
        case DDERR_PRIMARYSURFACEALREADYEXISTS :
            return std::string("primary surface already exists");
            break;
             
        case DDERR_REGIONTOOSMALL :
            return std::string("region too small");
            break;
             
        case DDERR_SURFACEALREADYATTACHED :
            return std::string("surface already attached");
            break;
             
        case DDERR_SURFACEALREADYDEPENDENT :
            return std::string("surface already dependent");
            break;
             
        case DDERR_SURFACEBUSY :
            return std::string("surface busy");
            break;
             
        case DDERR_SURFACEISOBSCURED :
            return std::string("surface is obscured");
            break;
             
        case DDERR_SURFACELOST :
            return std::string("surface lost");
            break;
             
        case DDERR_SURFACENOTATTACHED :
            return std::string("surface not attached");
            break;
        /*     
        case DDERR_TESTFINISHED :
            return std::string("TESTFINISHED");
            break;
        */     
        case DDERR_TOOBIGHEIGHT :
            return std::string("too big height");
            break;
             
        case DDERR_TOOBIGSIZE :
            return std::string("too big size");
            break;
             
        case DDERR_TOOBIGWIDTH :
            return std::string("too big width");
            break;
             
        case DDERR_UNSUPPORTED :
            return std::string("unsupported");
            break;
             
        case DDERR_UNSUPPORTEDFORMAT :
            return std::string("unsupported format");
            break;
             
        case DDERR_UNSUPPORTEDMASK :
            return std::string("unsupported mask");
            break;
             
        case DDERR_UNSUPPORTEDMODE :
            return std::string("unsupported mode");
            break;
             
        case DDERR_VERTICALBLANKINPROGRESS :
            return std::string("vertical blank in progress");
            break;
             
        case DDERR_VIDEONOTACTIVE :
            return std::string("video not active");
            break;
             
        case DDERR_WASSTILLDRAWING :
            return std::string("was still drawing");
            break;
             
        case DDERR_WRONGMODE :
            return std::string("wrong mode");
            break;
             
        case DDERR_XALIGN :
            return std::string("xalign");
            break;
    } 
    
    return std::string("undocumented DDraw error");
}

} // namespace core
