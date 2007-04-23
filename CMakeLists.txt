project(ktux)
add_subdirectory( sprites ) 
add_definitions (-DQT3_SUPPORT)
########### next target ###############

set(ktux_SRCS 
   spriteanim.cpp 
   spritemisc.cpp 
   spritepm.cpp 
   sprite.cpp )

kde4_automoc(${ktux_SRCS})

kde4_add_executable(ktux ${ktux_SRCS})

target_link_libraries(ktux  ${KDE4_KDEUI_LIBS} ${KDE4_KDE3SUPPORT_LIBS} kscreensaver )

install(TARGETS ktux  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES ktux.desktop  DESTINATION  ${SERVICES_INSTALL_DIR}/ScreenSavers )

kde4_install_icons(${ICON_INSTALL_DIR}  )

