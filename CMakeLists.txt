if(Q_WS_X11)

   project(ktux)

   #find_package(Strigi REQUIRED)

   # kscreensaver is installed by kdebase/workspace, so we depend on this here
   # and we have to check for it. Alex
   find_package(KDE4Workspace)

   if(KDE4WORKSPACE_FOUND)
      include_directories(${KDE4WORKSPACE_INCLUDE_DIR})

      add_subdirectory( sprites ) 

      set(ktux_SRCS 
         spriteanim.cpp 
         spritemisc.cpp 
         spritepm.cpp 
         sprite.cpp )


      kde4_add_executable(ktux ${ktux_SRCS})

      target_link_libraries(ktux ${KDE4_KDEUI_LIBS} ${STRIGI_LIBRARIES} ${QT_QT3SUPPORT_LIBRARY} ${KDE4WORKSPACE_KSCREENSAVER_LIBRARY})

      install(TARGETS ktux  ${INSTALL_TARGETS_DEFAULT_ARGS} )


      ########### install files ###############

      install( FILES ktux.desktop  DESTINATION  ${SERVICES_INSTALL_DIR}/ScreenSavers )

      kde4_install_icons(${ICON_INSTALL_DIR}  )

   else(KDE4WORKSPACE_FOUND)
      message(STATUS "Ktux can't be compiled. Install kdebase/workspace before")
   endif(KDE4WORKSPACE_FOUND)

endif(Q_WS_X11)
