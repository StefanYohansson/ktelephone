cmake_minimum_required(VERSION 2.8.11)
project(ktelephone)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets Sql Multimedia)
include (FindPkgConfig)
if (PKG_CONFIG_FOUND)
  pkg_check_modules(PJSIP REQUIRED libpjproject)
  include_directories(${PJSIP_INCLUDE_DIRS})
endif()

set(ktelephone_SRC
  src/main.cpp
  src/ktelephonemanager.cpp
  src/ktelephoneguide.cpp
  src/ktelephone.cpp
  src/ktelephonepreferences.cpp
  src/mycall.cpp
  src/ua.cpp
  src/uamanager.cpp
  src/ktelephonecall.cpp
)

qt5_wrap_ui(ktelephone_SRC src/ui/ktelephone.ui src/ui/startguide.ui src/ui/settings.ui src/ui/call.ui)

add_executable(ktelephone ${ktelephone_SRC})

target_link_libraries(ktelephone Qt5::Widgets Qt5::Sql Qt5::Multimedia)

target_link_libraries(ktelephone ${PJSIP_LIBRARIES})

install(TARGETS ktelephone DESTINATION bin)
