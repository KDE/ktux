project( ktux )
# Search KDE installation
find_package( KDE4 REQUIRED )
include( KDE4Defaults )
include( MacroLibrary )

add_definitions( ${QT_DEFINITIONS} ${QT_QTDBUS_DEFINITIONS} ${KDE4_DEFINITIONS} )
add_definitions( -DQT_NO_CAST_FROM_ASCII )
add_definitions( -DQT_NO_CAST_TO_ASCII )
add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
include_directories( ${KDE4_INCLUDES} )

# The FindKDE4.cmake module sets _KDE4_PLATFORM_DEFINITIONS with
# definitions like _GNU_SOURCE that are needed on each platform.
# Put configure checks below this line.
set( CMAKE_REQUIRED_DEFINITIONS ${_KDE4_PLATFORM_DEFINITIONS} )

# kscreensaver is installed by kdebase/workspace, so we depend on this here
# and we have to check for it. Alex
find_package( KDE4Workspace )

if( KDE4WORKSPACE_FOUND )
  include_directories( ${KDE4WORKSPACE_INCLUDE_DIR} )
  add_subdirectory( data )
  add_subdirectory( src ) 
else( KDE4WORKSPACE_FOUND )
  message( STATUS "Ktux can't be compiled. Install kdebase/workspace before" )
endif( KDE4WORKSPACE_FOUND )
