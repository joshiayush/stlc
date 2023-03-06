<div id="top"></div>

# Introduction

[**stlc**](https://github.com/joshiayush/stlc) is a simple library without any dependencies providing support for **StringStream**, **Vector**, **Map**, and **File System Routines**.

The **`STL`** is a collection of reusable data structures and algorithms that provide a standardized way of writing efficient and reliable code in C. It can save time and effort for programmers by providing pre-built functions and structures that can be easily used in a variety of applications.

</div>

<div align="right">
  <a href="#top">
  
  ![Back to top][back_to_top]
  
  </a>
</div>

## Installation

Clone the repository.

```shell
git clone https://github.com/joshiayush/stlc.git
```

**Install `libstlc`**

```shell
sudo chmod +x ./tools/install_libstlc.sh
./tools/install_libstlc.sh
```

**Build tests**

To build tests,

```shell
cmake -B build/ -S . -DBUILD_TESTS:BOOL=ON
cmake --build build/
```

Now run the `tests`,

```shell
./build/tests/tests  # Runs the unit tests
```

<div align="right">
  <a href="#top">
  
  ![Back to top][back_to_top]
  
  </a>
</div>

## Usage

### Creating and Initializing a `StringStream`

```c
#include <stlc/bool.h>
#include <stlc/sstream/sstream.h>

int main() {
  // Creating an StringStream instance
  StringStream sstream = StringStreamStrAlloc("Hello, World!");

  // Printing the contents of the StringStream
  PrintSstream(&sstream, FALSE);

  // Deallocating the StringStream
  StringStreamDealloc(&sstream);
}
```

### Using `StringStream` for File I/O

```c
#include <stdio.h>

#include <stlc/sstream/sstream.h>

int main() {
  // Creating an empty StringStream
  StringStream sstream = StringStreamAlloc();

  FILE* file = fopen("/path/to/file.txt", "r");

  // Read file contents into the stream
  StringStreamReadFile(&sstream, file, 0);

  // Return the resource acquired
  fclose(file);

  PrintSstream(&sstream, FALSE);
  printf("Total bytes stored %d.", sstream.length);
}
```

## Contribution

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement", "bug", or "documentation". Don't forget to give the project a star! Thanks again!

Project [**stlc**][_stlc] is hosted on [GitHub][_github]. If you want to contribute changes please make sure to read the [`CONTRIBUTING.md`][_stlc_contrib_f] file. You can also contribute changes to the [`CONTRIBUTING.md`][_stlc_contrib_f] file itself.

<div align="right">
  <a href="#top">
  
  ![Back to top][back_to_top]
  
  </a>
</div>

<!-- Definitions -->

[_stlc]: https://www.github.com/joshiayush/stlc
[_github]: https://www.github.com

<!-- Attached links -->

[back_to_top]: https://img.shields.io/badge/-Back%20to%20top-lightgrey

<!-- Files -->

[_stlc_contrib_f]: https://www.github.com/joshiayush/stlc/blob/master/CONTRIBUTING.md
