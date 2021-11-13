enable_testing()

find_package(Qt5Test REQUIRED)

qt5_wrap_ui(ktelephone_ui_SRC src/ui/transfercall.ui)

add_executable(
  test_transfer_call_dialog
  tests/gui/test_transfer_call_dialog.cpp
  src/gui/ktelephonetransfercall.cpp
  ${ktelephone_ui_SRC}
)
target_link_libraries(test_transfer_call_dialog Qt5::Test Qt5::Widgets)
add_test(test_transfer_call_dialog test_transfer_call_dialog)