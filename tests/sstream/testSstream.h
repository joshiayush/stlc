// Copyright (C) 2021  Ayush Joshi
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.
//
// See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program. If not, see <https://www.gnu.org/licenses/>.

#include <gtest/gtest.h>

#include "data/sstream/accessors.h"
#include "data/sstream/sstream.h"

class SstreamTest : public ::testing::Test {
 protected:
  /**
   * @brief Deallocates @a stringstream instance from the free store.
   */
  void TearDown() override { stringstream_dealloc(&sstream); }

 protected:
  stringstream sstream;
};

TEST_F(SstreamTest, stringStreamDefAllocFunctionTest) {
  sstream = stringstream_def_alloc();
  ASSERT_NE(sstream.data, (void*)0);
  ASSERT_EQ(sstream.length, SSTREAM_DEFAULT_SIZE);
  uint32_t capacity = 1;
  while (capacity < sstream.length)
    capacity <<= 1;
  ASSERT_EQ(sstream.capacity, capacity);
}

TEST_F(SstreamTest, stringStreamAllocFunctionWithArbitraryLengthTest) {
  sstream = stringstream_alloc(2147483648);
  ASSERT_NE(sstream.data, (void*)0);
  ASSERT_EQ(sstream.length, 2147483648);
  uint32_t capacity = 1;
  while (capacity < sstream.length)
    capacity <<= 1;
  ASSERT_EQ(sstream.capacity, capacity);
}