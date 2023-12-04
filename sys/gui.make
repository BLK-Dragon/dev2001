#===============================================================================
#
# 'gui' project MAKEFILE
# generated by gen_makefile 1.1 (c) Ghost Dragon 2001
#
#===============================================================================

default_CPPopt          = -c -nologo -GR -GX -O2 -Qwn3 -Qwd 985 -Qsox- -DTRACING
debug_CPPopt            = -c -nologo -GR -GX -Od -Zi -Qwn3 -Qwd 985 -Qsox- -DTRACING


gui_cppobjs =                                                                  \
    _default\GUImanager.obj                                                    \
    _default\WidgetView.obj                                                    \


_default\gui.lib        : __before_all__ $(gui_cppobjs) 
                          
                          lib.exe                                              \
                          -out:_default\gui.lib                                \
                          $(gui_cppobjs) 
                          

__before_all__          : 
                          if not exist _default mkdir _default


#-------------------------------------------------------------------------------
#
#   'GUImanager.cpp'

GUImanager_cpp__files =                                                        \
    GUImanager.hpp                                                             \
    Surface.hpp                                                                \
    Core.hpp                                                                   \
    DDrawError.hpp                                                             \
    Exception.hpp                                                              \
    Msg.hpp                                                                    \
    Win32error.hpp                                                             \
    DInputError.hpp                                                            \
    Type.hpp                                                                   \
    Vector.hpp                                                                 \
    Rect.hpp                                                                   \
    WidgetView.hpp                                                             \
    Sprite.hpp                                                                 \
    Image.hpp                                                                  \
    File.hpp                                                                   \
    ImageFormat.hpp                                                            \
    Font.hpp                                                                   \
    ParamSet.hpp                                                               \

_default\GUImanager.obj : GUImanager.cpp $(GUImanager_cpp__files)
                          icl.exe $(default_CPPopt) -Fo_default/ GUImanager.cpp


#-------------------------------------------------------------------------------
#
#   'WidgetView.cpp'

WidgetView_cpp__files =                                                        \
    WidgetView.hpp                                                             \
    Sprite.hpp                                                                 \
    Image.hpp                                                                  \
    File.hpp                                                                   \
    Core.hpp                                                                   \
    DDrawError.hpp                                                             \
    Exception.hpp                                                              \
    Msg.hpp                                                                    \
    Win32error.hpp                                                             \
    DInputError.hpp                                                            \
    Type.hpp                                                                   \
    Vector.hpp                                                                 \
    Rect.hpp                                                                   \
    ImageFormat.hpp                                                            \
    Surface.hpp                                                                \
    Font.hpp                                                                   \
    ParamSet.hpp                                                               \
    GUImanager.hpp                                                             \
    BitOps.hpp                                                                 \

_default\WidgetView.obj : WidgetView.cpp $(WidgetView_cpp__files)
                          icl.exe $(default_CPPopt) -Fo_default/ WidgetView.cpp
