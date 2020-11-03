# CMakeList.txt: file di progetto CMake di primo livello. Eseguire la configurazione globale
# e includere qui i sottoprogetti.
#
cmake_minimum_required (VERSION 3.8)

project ("FindPath")

# Includere i sottoprogetti.
add_subdirectory ("FindPath")

# this is so we can #include <gtest/gtest.h>
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
