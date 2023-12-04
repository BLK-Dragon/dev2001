gen_makefile -project=dwig -config=default -makefile=dwig.make dwig.xml

cls
nmake -nologo -f dwig.make %*
