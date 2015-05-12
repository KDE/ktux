set( ktux_SRCS
  spriteanim.cpp
  spritemisc.cpp
  spritepm.cpp
  sprite.cpp
)

kde4_add_executable( ktux ${ktux_SRCS} )
target_link_libraries( ktux ${KDE4_KDEUI_LIBS} ${KDE4WORKSPACE_KSCREENSAVER_LIBRARY} )

install( TARGETS ktux ${INSTALL_TARGETS_DEFAULT_ARGS} )
install( FILES ktux.desktop DESTINATION ${SERVICES_INSTALL_DIR}/ScreenSavers )
