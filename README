This is a tutorial of Ariadne on hybrid evolution, written as a simple self contained example (SSCE).
To compile the tutorial, use the same procedure as for the compilation of the library. For instance using a Ninja back-end with multiple configurations enabled:

```bash
$ cmake -S . -B build -G "Ninja Multi-Config"
```

In case any of the Ariadne or MPFR libraries is not in a standard directory provide the directories where the `find_package` utility will search.

```bash
$ cmake -S . -B build -D CMAKE_PREFIX_PATH:PATH="/home/gkaf/lib/ariadne/cxx;/home/gkaf/lib/mpfr" -G "Ninja Multi-Config"
```

Compile with:

```bash
cmake --build build --config Release --target all
```
