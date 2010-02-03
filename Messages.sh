#! /bin/sh
#$EXTRACTRC `find . -name \*.ui` >>  rc.cpp
#$XGETTEXT rc.cpp src/*.cpp src/*.h -o $podir/ktux.pot
#rm -f rc.cpp

$XGETTEXT src/*.cpp src/*.h -o $podir/ktux.pot
