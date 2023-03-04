<div id="top"></div>

<br/>

<div align="center">
  <h3 align="center">stlc</h3>
  
  <p align="center">
    Standard Template Library for C.
    <br />
    <a href="https://github.com/joshiayush/stlc/tree/master/docs"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/joshiayush/stlc/wiki">Wiki</a>
    ·
    <a href="https://github.com/joshiayush/stlc/issues">Report Bug</a>
    ·
    <a href="https://github.com/joshiayush/stlc/issues">Request Feature</a>
  </p>
</div>

<div align='center'>

<a href="https://github.com/joshiayush/stlc"><strong>stlc</strong></a> is a simple library without any dependancies providing support for **StringStream**, **Vector**, **Map**, and **File System Routines**.

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

**Build stlc**

Next step is to build `stlc` as a shared library using our build system `cmake`.

```shell
cmake -B build/ -S .
cd build
make install
```

This will build `stlc` library as `libstlc.so` file inside the `build` directory.

**Build tests**

To build tests,

```shell
cmake -B build/ -S . -DBUILD_TESTS:BOOL=ON
cd build
make install
```

Now run the `tests`,

```shell
./tests/tests  # Runs the unit tests
```

<div align="right">
  <a href="#top">
  
  ![Back to top][back_to_top]
  
  </a>
</div>

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
