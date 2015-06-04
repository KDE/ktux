add_definitions(-DTRANSLATION_DOMAIN=\"ktux\")

set( ktux_SRCS
  spriteanim.cpp
  spritemisc.cpp
  spritepm.cpp
  sprite.cpp
)

add_executable( ktux ${ktux_SRCS} )
target_link_libraries(ktux
    Qt5::Widgets
    KF5::ConfigCore
    KF5::ConfigGui
    KF5::CoreAddons
    KF5::I18n
)

install( TARGETS ktux ${INSTALL_TARGETS_DEFAULT_ARGS} )

# install( FILES ktux.desktop DESTINATION ${SERVICES_INSTALL_DIR}/ScreenSavers )
