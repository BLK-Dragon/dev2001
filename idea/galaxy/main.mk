main_files =  \
..\..\sys\Win32Error.hpp \
..\..\sys\Exception.hpp \
..\..\sys\Noise.hpp \
..\..\sys\Random.hpp \
..\..\sys\Type.hpp \
..\..\tool\Toster\\Toster.hpp \
..\..\sys\Rect.hpp \
..\..\sys\Vector.hpp \
PixelShader.hpp \
ColorInterpolator.hpp \


main.obj : main.cpp $(main_files)
    cl.exe -c -GX -Qwn3 -O2 -Zd -Zi -Qansi main.cpp
