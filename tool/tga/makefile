
tga_tool.exe    : tga_tool.obj
                  link                          \
                  -out:tga_tool.exe             \
                  -opt:ref                      \
                  tga_tool.obj

tga_tool.obj    : tga_tool.cpp
                  icl -c -O2 -Qwn2 -Zp1 tga_tool.cpp

