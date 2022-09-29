cmake_minimum_required(VERSION 2.8.11)
project(ktelephone)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets Sql Multimedia)
find_package(KF5WidgetsAddons)
include (FindPkgConfig)
if (PKG_CONFIG_FOUND)
  pkg_check_modules(PJSIP REQUIRED libpjproject)
  include_directories(${PJSIP_INCLUDE_DIRS})
  if(DEFINED ENV{BUILD_STATIC})
    link_directories(${PJSIP_STATIC_LIBRARY_DIRS}) 
  endif()
endif()

include_directories(src/telephony)
include_directories(src/gui)

file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(ktelephone_SRC
  src/main.cpp
  src/gui/ktelephonemanager.cpp
  src/gui/ktelephoneabout.cpp
  src/gui/ktelephoneguide.cpp
  src/gui/ktelephone.cpp
  src/gui/ktelephonepreferences.cpp
  src/gui/ktelephonecall.cpp
  src/gui/ktelephonetransfercall.cpp
  src/gui/ktelephoneutil.cpp
  src/telephony/mycall.cpp
  src/telephony/ua.cpp
  src/telephony/uamanager.cpp)

qt5_add_resources(ktelephone_SRC resources.qrc)

qt5_wrap_ui(ktelephone_SRC src/ui/ktelephone.ui src/ui/startguide.ui src/ui/settings.ui src/ui/call.ui src/ui/about.ui src/ui/transfercall.ui)

add_executable(ktelephone ${ktelephone_SRC})

target_link_libraries(ktelephone KF5::WidgetsAddons Qt5::Widgets Qt5::Sql Qt5::Multimedia)

target_link_libraries(ktelephone ${PJSIP_LIBRARIES})

install(TARGETS ktelephone DESTINATION bin)

if (ENABLE_TESTING)
  include(tests/CMakeLists.txt)
endif()