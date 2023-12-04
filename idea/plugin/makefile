
main.exe    : main.obj
              link.exe              \
              -debugtype:cv         \
              -debug                \
              kernel32.lib          \
              main.obj

main.obj    : main.cpp
              icl.exe -c -MD -GX -Od -Zi main.cpp


orange.dll  : orange.obj
              link.exe              \
              -dll                  \
              -debugtype:cv         \
              -debug                \
              -export:CreateFruit   \
              -export:DestroyFruit  \
              kernel32.lib          \
              orange.obj

orange.obj  : orange.cpp
              icl.exe -c -MD -GX -Od -Zi orange.cpp

lemon.dll   : lemon.obj
              link.exe              \
              -dll                  \
              -debugtype:cv         \
              -debug                \
              -export:CreateFruit   \
              -export:DestroyFruit  \
              kernel32.lib          \
              lemon.obj

lemon.obj   : lemon.cpp
              icl.exe -c -MD -GX -Od -Zi lemon.cpp
