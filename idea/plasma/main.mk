main_files =  \
Color.hpp \
..\..\sys\Core.hpp \
..\..\sys\DDrawError.hpp \
..\..\sys\Exception.hpp \
..\..\sys\Win32error.hpp \
..\..\sys\DInputError.hpp \
..\..\sys\Type.hpp \
..\..\sys\Win32Error.hpp \
..\..\sys\Noise.hpp \
..\..\sys\Random.hpp \
..\..\tool\Toster\Toster.hpp \


main.obj : main.cpp $(main_files)
    icl.exe -c -GX -Qwd 985 -MD main.cpp
