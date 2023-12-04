gui_objs = \
GUImanager.obj \
WidgetView.obj \


gui.lib : $(gui_objs) 
    lib.exe \
    $(gui_objs)

GUImanager_files = \
GUImanager.hpp \
Surface.hpp \
Core.hpp \
DDrawError.hpp \
Exception.hpp \
Win32error.hpp \
DInputError.hpp \
Type.hpp \
Vector.hpp \
Rect.hpp \
WidgetView.hpp \
Sprite.hpp \
Image.hpp \
File.hpp \
ImageFormat.hpp \
Font.hpp \
ParamSet.hpp \

GUImanager.obj : GUImanager.cpp $(GUImanager_files)
    icl.exe -c -GR -GX -Od -Qwn3 -Qwd 985 -Zi -DTRACING GUImanager.cpp

WidgetView_files = \
WidgetView.hpp \
Sprite.hpp \
Image.hpp \
File.hpp \
Core.hpp \
DDrawError.hpp \
Exception.hpp \
Win32error.hpp \
DInputError.hpp \
Type.hpp \
Vector.hpp \
Rect.hpp \
ImageFormat.hpp \
Surface.hpp \
Font.hpp \
ParamSet.hpp \
GUImanager.hpp \
Graph.hpp \
BitOps.hpp \

WidgetView.obj : WidgetView.cpp $(WidgetView_files)
    icl.exe -c -GR -GX -Od -Qwn3 -Qwd 985 -Zi -DTRACING WidgetView.cpp

