
OBJs        = main.obj                          \

main.exe    : $(OBJs)
              link                                      \
              -defaultlib:user32.lib                    \
              -defaultlib:gdi32.lib                     \
              -defaultlib:ddraw.lib                     \
              -defaultlib:dxguid.lib                    \
              -defaultlib:..\..\tool\Toster\toster.lib  \
              -defaultlib:..\..\sys\vfx.lib             \
              -opt:ref                                  \
              -out:main.exe                             \
              $(OBJs)                                   \

!include main.mk
