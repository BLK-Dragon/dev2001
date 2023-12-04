
OBJs        = main.obj                          \
              PixelShader.obj                   \


main.exe    : $(OBJs)
              link                                      \
              -defaultlib:user32.lib                    \
              -defaultlib:gdi32.lib                     \
              -defaultlib:ddraw.lib                     \
              -defaultlib:dxguid.lib                    \
              -defaultlib:..\..\tool\Toster\toster.lib  \
              -defaultlib:..\..\sys\vfx.lib             \
              -defaultlib:..\..\sys\core.lib            \
              -debug                                    \
              -debugtype:cv                             \
              -profile                                  \
              -out:main.exe                             \
              $(OBJs)                                   \


!include main.mk
!include PixelShader.mk
