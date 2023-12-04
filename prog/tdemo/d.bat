gen_makefile -project=tdemo -config=debug -makefile=tdemo_debug.make tdemo.xml

nmake -nologo -f tdemo_debug.make %*
