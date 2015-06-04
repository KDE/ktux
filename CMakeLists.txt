cmake_minimum_required(VERSION 3.0.0)
project( ktux )

find_package(ECM 1.3.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})
include(ECMInstallIcons)

find_package(Qt5 5.2.0 CONFIG REQUIRED
  Core
  Widgets
)

find_package(KF5 5.0.0 REQUIRED
  Config
  CoreAddons
)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(FeatureSummary)

add_definitions( -DQT_NO_CAST_FROM_ASCII )
add_definitions( -DQT_NO_CAST_TO_ASCII )
add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)

add_subdirectory(data)
add_subdirectory(src)
