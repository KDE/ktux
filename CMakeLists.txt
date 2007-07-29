project(ktux)

add_subdirectory( sprites ) 
add_definitions (-DQT3_SUPPORT)
########### next target ###############

set(ktux_SRCS 
   spriteanim.cpp 
   spritemisc.cpp 
   spritepm.cpp 
   sprite.cpp )


kde4_add_executable(ktux ${ktux_SRCS})

find_package(Strigi)

target_link_libraries(ktux ${KDE4_KDEUI_LIBS} ${STRIGI_LIBRARIES} ${QT_QT3SUPPORT_LIBRARY} kscreensaver )

install(TARGETS ktux  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES ktux.desktop  DESTINATION  ${SERVICES_INSTALL_DIR}/ScreenSavers )

kde4_install_icons(${ICON_INSTALL_DIR}  )

