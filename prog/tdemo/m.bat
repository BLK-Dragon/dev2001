gen_makefile -project=tdemo -config=default -makefile=tdemo.make tdemo.xml

nmake -nologo -f tdemo.make %*
