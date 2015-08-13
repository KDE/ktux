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
)

install( TARGETS ktux ${INSTALL_TARGETS_DEFAULT_ARGS} )
install( PROGRAMS org.kde.ktux.desktop  DESTINATION  ${KDE_INSTALL_APPDIR}/ktux )
