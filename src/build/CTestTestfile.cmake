# CMake generated Testfile for 
# Source directory: /home/rushd/Desktop/group55/src
# Build directory: /home/rushd/Desktop/group55/src/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(AlarmTests "/home/rushd/Desktop/group55/src/build/AlarmTest")
set_tests_properties(AlarmTests PROPERTIES  _BACKTRACE_TRIPLES "/home/rushd/Desktop/group55/src/CMakeLists.txt;161;add_test;/home/rushd/Desktop/group55/src/CMakeLists.txt;0;")
add_test(StorageTests "/home/rushd/Desktop/group55/src/build/StorageTests")
set_tests_properties(StorageTests PROPERTIES  _BACKTRACE_TRIPLES "/home/rushd/Desktop/group55/src/CMakeLists.txt;162;add_test;/home/rushd/Desktop/group55/src/CMakeLists.txt;0;")
add_test(VideoTests "/home/rushd/Desktop/group55/src/build/VideoTests")
set_tests_properties(VideoTests PROPERTIES  _BACKTRACE_TRIPLES "/home/rushd/Desktop/group55/src/CMakeLists.txt;163;add_test;/home/rushd/Desktop/group55/src/CMakeLists.txt;0;")
add_test(SystemTests "/home/rushd/Desktop/group55/src/build/SystemTests")
set_tests_properties(SystemTests PROPERTIES  _BACKTRACE_TRIPLES "/home/rushd/Desktop/group55/src/CMakeLists.txt;164;add_test;/home/rushd/Desktop/group55/src/CMakeLists.txt;0;")
add_test(FileTests "/home/rushd/Desktop/group55/src/build/FileTests")
set_tests_properties(FileTests PROPERTIES  _BACKTRACE_TRIPLES "/home/rushd/Desktop/group55/src/CMakeLists.txt;165;add_test;/home/rushd/Desktop/group55/src/CMakeLists.txt;0;")
add_test(ImageTests "/home/rushd/Desktop/group55/src/build/ImageTests")
set_tests_properties(ImageTests PROPERTIES  _BACKTRACE_TRIPLES "/home/rushd/Desktop/group55/src/CMakeLists.txt;166;add_test;/home/rushd/Desktop/group55/src/CMakeLists.txt;0;")
add_test(ConfigTests "/home/rushd/Desktop/group55/src/build/ConfigTests")
set_tests_properties(ConfigTests PROPERTIES  _BACKTRACE_TRIPLES "/home/rushd/Desktop/group55/src/CMakeLists.txt;167;add_test;/home/rushd/Desktop/group55/src/CMakeLists.txt;0;")
subdirs("_deps/sfml-build")
subdirs("_deps/googletest-build")
