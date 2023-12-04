gen_makefile -project=tdemo -config=optimized -makefile=tdemo_optimized.make tdemo.xml

nmake -nologo -f tdemo_optimized.make %*
