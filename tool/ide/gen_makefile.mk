
gen_makefile.exe : main.obj Workspace.obj
                   link                         \
                   -debugtype:cv                \
                   -debug                       \
                   -out:gen_makefile.exe        \
                   -defaultlib:xerces-c_1.lib   \
                   main.obj                     \
                   Workspace.obj                
                   copy gen_makefile.exe "%dragon_tool%"


main.obj   : main.cpp Workspace.hpp
             icl -c -Qwn3 -Od -Qansi -Zi -Qwd 985 main.cpp

Workspace.obj   : Workspace.hpp Workspace.cpp
                  icl -c -Qwn3 -Od -Qansi -Zi -Qwd 985 Workspace.cpp

