This is an example of using Ariadne for evaluating the hybrid evolution of a buck converter model. It supports single configuration generators at the moment (e.g. Unix Makefiles, Ninja).

A configuration preset is provided with the following setup:
- `binaryDir` (`CMAKE_BINARY_DIR:PATH`): `${sourceDir}/build/Release`
- `generator` (`CMAKE_GENERATOR:STRING`): `Ninja`
- `CMAKE_BUILD_TYPE:STRING`: `Release`
- `CMAKE_PREFIX_PATH:PATH`: `$env{HOME}/opt/ariadne/cxx;$env{HOME}/opt/mpfr;$env{HOME}/opt/gmp`

The configuration presets expect the packages GMP, MPFR, and Ariadne to be installed in `${HOME}/opt`.

To avoid having the executable linked with unexpected libraries, the `-Wl,--disable-new-dtags` flag is used when linking. This places the paths for GMP, MPRF, and Ariadne in RPATH instead RUNPATH, overriding any system libraries. During installation all RPATH/RUNPATH are removed.

To configure the package with the presets, from the top level directory of the repository use the command 
```bash
cmake -S . --preset default-config
```
and then build as usual:
```bash
cmake --build build/Release --target all
```

To configure manually:
```bash
cmake -S . -B build -G "Ninja" 
```
If the required libraries are not in the standard directories, also provide the directories where the `find_package` utility will search. For instance:

```bash
cmake -S . -B build -G "Ninja" -D CMAKE_PREFIX_PATH:PATH="${HOME}/opt/ariadne/cxx;${HOME}/opt/mpfr;${HOME}/opt/gmp"
```
To build the program issues the command: 
```bash
cmake --build build --target all
```

The executable is generated in: `${CMAKE_BINARY_DIR}/bin/hybrid_evolution_tutorial`
