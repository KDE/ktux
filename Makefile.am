## Makefile.am for ktux

# this has all of the subdirectories that make will recurse into.  if
# there are none, comment this out
SUBDIRS = sprites

# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = ktux

# set the include path for X, qt and KDE
INCLUDES         = $(all_includes)

# the library search path. 
ktux_LDFLAGS = $(all_libraries) $(KDE_RPATH)

# the libraries to link against.
ktux_LDADD   = $(LIB_KDEUI)

# which sources should be compiled for ktux
ktux_SOURCES =	main.cpp demowin.cpp spriteanim.cpp \
                spritemisc.cpp spritepm.cpp saver.cpp sprite.cpp

# these are the headers for your project
noinst_HEADERS   = saver.h demowin.h vroot.h sprite.h spriteanim.h \
                    spritemisc.h spritepm.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

# if you "make distclean", this files get removed. If you want to remove
# them while "make clean", use CLEANFILES
DISTCLEANFILES = $(METASOURCES)

# make messages.po. Move this one to ../po/ and "make merge" in po
# the -x is for skipping messages already translated in kdelibs
messages:
	$(XGETTEXT) -C -ki18n -x $(includedir)/kde.pot $(ktux_SOURCES) && mv messages.po ../po/ktux.pot

KDE_ICON = ktux

 # this is where the kdelnk file will go 
#kdelnkdir   = $(kde_appsdir)/System/ScreenSavers
#kdelnk_DATA = ktux.desktop
appsdir   = $(kde_appsdir)/System/ScreenSavers
apps_DATA = ktux.desktop
