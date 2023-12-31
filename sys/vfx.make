#===============================================================================
#
# 'vfx' project MAKEFILE
# generated by gen_makefile 1.1 (c) Ghost Dragon 2001
#
#===============================================================================

default_CPPopt          = -c -nologo -GR -GX -O2 -Qwn3 -Qwd 985 -Qsox- -DTRACING
debug_CPPopt            = -c -nologo -GR -GX -Od -Zi -Qwn3 -Qwd 985 -Qsox- -DTRACING


vfx_cppobjs =                                                                  \
    _default\Render2D.obj                                                      \
    _default\Noise.obj                                                         \
    _default\Random.obj                                                        \
    _default\PixelShader.obj                                                   \


_default\vfx.lib        : __before_all__ $(vfx_cppobjs) 
                          
                          lib.exe                                              \
                          -out:_default\vfx.lib                                \
                          $(vfx_cppobjs) 
                          

__before_all__          : 
                          if not exist _default mkdir _default


#-------------------------------------------------------------------------------
#
#   'Render2D.cpp'

Render2D_cpp__files =                                                          \
    Render2D.hpp                                                               \
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
    Fill.hpp                                                                   \
    BitOps.hpp                                                                 \

_default\Render2D.obj   : Render2D.cpp $(Render2D_cpp__files)
                          icl.exe $(default_CPPopt) -Fo_default/ Render2D.cpp


#-------------------------------------------------------------------------------
#
#   'Noise.cpp'

Noise_cpp__files =                                                             \
    Noise.hpp                                                                  \
    Random.hpp                                                                 \
    Type.hpp                                                                   \
    Exception.hpp                                                              \
    Msg.hpp                                                                    \
    FixedPoint.hpp                                                             \

_default\Noise.obj      : Noise.cpp $(Noise_cpp__files)
                          icl.exe $(default_CPPopt) -Fo_default/ Noise.cpp


#-------------------------------------------------------------------------------
#
#   'Random.cpp'

Random_cpp__files =                                                            \
    Random.hpp                                                                 \
    Type.hpp                                                                   \

_default\Random.obj     : Random.cpp $(Random_cpp__files)
                          icl.exe $(default_CPPopt) -Fo_default/ Random.cpp


#-------------------------------------------------------------------------------
#
#   'PixelShader.cpp'

PixelShader_cpp__files =                                                       \
    PixelShader.hpp                                                            \
    Type.hpp                                                                   \
    Noise.hpp                                                                  \
    Random.hpp                                                                 \
    Exception.hpp                                                              \
    Msg.hpp                                                                    \
    FixedPoint.hpp                                                             \

_default\PixelShader.obj: PixelShader.cpp $(PixelShader_cpp__files)
                          icl.exe $(default_CPPopt) -Fo_default/ PixelShader.cpp

