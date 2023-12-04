
build.exe   : build.obj
              link                              \
              -opt:ref                          \
              -defaultlib:..\..\sys\core.lib    \
              -defaultlib:advapi32.lib          \
              build.obj

build.obj   : build.cpp
              icl -c -Qwn3 -Od -Qansi build.cpp