project(ktux)
add_subdirectory( sprites ) 




########### next target ###############

set(ktux_SRCS 
   spriteanim.cpp 
   spritemisc.cpp 
   spritepm.cpp 
   sprite.cpp )

kde4_automoc(${ktux_SRCS})

kde4_add_executable(ktux ${ktux_SRCS})

target_link_libraries(ktux  ${KDE4_KDEUI_LIBS} kscreensaver )

install(TARGETS ktux  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES ktux.desktop  DESTINATION  ${SERVICES_INSTALL_DIR}/ScreenSavers )

kde4_install_icons(${ICON_INSTALL_DIR}  )




#original Makefile.am contents follow:

### Makefile.am for ktux
#
## this has all of the subdirectories that make will recurse into.  if
## there are none, comment this out
#SUBDIRS = sprites
#
## this is the program that gets installed.  it's name is used for all
## of the other Makefile.am variables
#bin_PROGRAMS = ktux
#
## set the include path for X, qt and KDE
#INCLUDES         = $(all_includes)
#
## the library search path. 
#ktux_LDFLAGS = $(all_libraries) $(KDE_RPATH)
#
## the libraries to link against.
#ktux_LDADD   = $(LIB_KDEUI) -lkscreensaver
#
## which sources should be compiled for ktux
#ktux_SOURCES =	spriteanim.cpp spritemisc.cpp spritepm.cpp sprite.cpp
#
## these are the headers for your project
#noinst_HEADERS   = sprite.h spriteanim.h spritemisc.h spritepm.h
#
## let automoc handle all of the meta source files (moc)
#METASOURCES = AUTO
#
## make messages.po. Move this one to ../po/ and "make merge" in po
## the -x is for skipping messages already translated in kdelibs
#messages:
#	$(XGETTEXT) *.cpp -o $(podir)/ktux.pot
#
#KDE_ICON = ktux
#
#appsdir   = $(kde_appsdir)/System/ScreenSavers
#apps_DATA = ktux.desktop
#
