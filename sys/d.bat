gen_makefile -project=core -config=debug -makefile=core_debug.make sys.xml 
gen_makefile -project=gui -config=debug -makefile=gui_debug.make sys.xml
gen_makefile -project=vfx -config=debug -makefile=vfx_debug.make sys.xml

nmake -nologo -f core_debug.make %*
nmake -nologo -f vfx_debug.make %*
nmake -nologo -f gui_debug.make %*
