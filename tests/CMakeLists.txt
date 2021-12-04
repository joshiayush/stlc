# Copyright (C) 2021  Ayush Joshi
# 
# This program is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
# 
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.
# 
# See the GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License along with
# this program. If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.16.3)

project(cjson LANGUAGES CXX C)

set(CJSON_INCLUDE_DIRS "../include")

# Note: This requires cmake to be run each time the files 
# have changed to configure the build system 
file(GLOB_RECURSE CJSON_SRC_FILES "../cjson/*.c")

# finding googletest library
find_package(GTest REQUIRED)
include_directories(${GTest_INCLUDE_DIRS} ${CJSON_INCLUDE_DIRS})

add_executable(tests tests.cc ${CJSON_SRC_FILES})
target_link_libraries(tests ${GTEST_LIBRARIES} pthread)

# post build command to execute the tests as soon as we built them
add_custom_command(TARGET tests POST_BUILD COMMAND tests)