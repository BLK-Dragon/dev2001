PixelShader_files =  \
PixelShader.hpp \
..\..\sys\Type.hpp \
..\..\sys\Noise.hpp \
..\..\sys\Random.hpp \
..\..\sys\Exception.hpp \


PixelShader.obj : PixelShader.cpp $(PixelShader_files)
    cl.exe -c -GX -Qwn3 -O2 -Zd -Zi -Qansi PixelShader.cpp
