gen_makefile -project=core -config=default -makefile=core.make sys.xml 
gen_makefile -project=gui -config=default -makefile=gui.make sys.xml
gen_makefile -project=vfx -config=default -makefile=vfx.make sys.xml

nmake -nologo -f core.make %*
nmake -nologo -f vfx.make %*
nmake -nologo -f gui.make %*

