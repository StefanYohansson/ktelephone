cmake_minimum_required(VERSION 2.8.11)
project(ktelephone)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library

find_package(Qt5 REQUIRED COMPONENTS Widgets Sql)
include (FindPkgConfig)
if (PKG_CONFIG_FOUND) # true if pkg-config works on the system
  pkg_check_modules(PJSIP REQUIRED libpjproject)
  include_directories(${PJSIP_INCLUDE_DIRS})
endif()

set(ktelephone_SRC
  src/main.cpp
  src/ktelephonemanager.cpp
  src/ktelephoneguide.cpp
  src/ktelephone.cpp
  src/ktelephonepreferences.cpp
  src/ua.cpp
  src/uamanager.cpp
)

# Create code from a list of Qt designer ui files.
#set(CMAKE_AUTOUIC ON) # use this if you have CMake 3.x instead of the following
qt5_wrap_ui(ktelephone_SRC src/ui/ktelephone.ui src/ui/startguide.ui src/ui/settings.ui)

# Tell CMake to create the helloworld executable
add_executable(ktelephone ${ktelephone_SRC})

# Use the Widgets module from Qt 5.
target_link_libraries(ktelephone Qt5::Widgets Qt5::Sql)

target_link_libraries(ktelephone ${PJSIP_LIBRARIES})

# Install the executable
install(TARGETS ktelephone DESTINATION bin)
