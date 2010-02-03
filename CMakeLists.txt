project( ktux )

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
