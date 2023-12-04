del *.mk
chkdep -q *.cpp final_build.cfg

nmake %*
