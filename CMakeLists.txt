project(ktux)

add_subdirectory( sprites ) 

########### next target ###############

set(ktux_SRCS 
   spriteanim.cpp 
   spritemisc.cpp 
   spritepm.cpp 
   sprite.cpp )


kde4_add_executable(ktux ${ktux_SRCS})

find_package(Strigi REQUIRED)

target_link_libraries(ktux ${KDE4_KDEUI_LIBS} ${STRIGI_LIBRARIES} ${QT_QT3SUPPORT_LIBRARY} kscreensaver )

install(TARGETS ktux  ${INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( FILES ktux.desktop  DESTINATION  ${SERVICES_INSTALL_DIR}/ScreenSavers )

kde4_install_icons(${ICON_INSTALL_DIR}  )

