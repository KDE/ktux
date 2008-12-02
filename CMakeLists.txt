if(Q_WS_X11)

   project(ktux)

   #find_package(Strigi REQUIRED)

   # kscreensaver is installed by kdebase/workspace, so we depend on this here
   # and we have to check for it. Alex
   find_path( KSCREENSAVER_INCLUDE_DIR kscreensaver.h
            PATHS ${KDE4_INCLUDE_DIR}
   )
   find_library( KSCREENSAVER_LIBRARY kscreensaver
            PATHS ${KDE4_LIB_DIR}
   )

   if(KSCREENSAVER_INCLUDE_DIR  AND  KSCREENSAVER_LIBRARY)
      include_directories(${KSCREENSAVER_INCLUDE_DIR})

      add_subdirectory( sprites ) 

      set(ktux_SRCS 
         spriteanim.cpp 
         spritemisc.cpp 
         spritepm.cpp 
         sprite.cpp )


      kde4_add_executable(ktux ${ktux_SRCS})

      target_link_libraries(ktux ${KDE4_KDEUI_LIBS} ${STRIGI_LIBRARIES} ${QT_QT3SUPPORT_LIBRARY} ${KSCREENSAVER_LIBRARY})

      install(TARGETS ktux  ${INSTALL_TARGETS_DEFAULT_ARGS} )


      ########### install files ###############

      install( FILES ktux.desktop  DESTINATION  ${SERVICES_INSTALL_DIR}/ScreenSavers )

      kde4_install_icons(${ICON_INSTALL_DIR}  )

   else(KSCREENSAVER_INCLUDE_DIR  AND  KSCREENSAVER_LIBRARY)
      message(STATUS "Ktux can't be compiled. Install kdebase before")
   endif(KSCREENSAVER_INCLUDE_DIR  AND  KSCREENSAVER_LIBRARY)

endif(Q_WS_X11)
