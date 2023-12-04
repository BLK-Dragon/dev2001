
Toster.dll      : Toster.obj Toster.res
                  link                              \
                 -dll                               \
                 -implib:toster.lib                 \
                 -defaultlib:user32.lib             \
                 -defaultlib:gdi32.lib              \
                 -defaultlib:comdlg32.lib           \
                 -out:toster.dll                    \
                  Toster.obj Toster.res

Toster.obj      : Toster.cpp
                  icl -c -O2 -Qwn3 Toster.cpp

Toster.res      : Toster.rc
                  rc -r Toster.rc