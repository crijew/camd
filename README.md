# CAMD standalone

This repository packages the CAMD (Constrained Approximate Minimum Degree)
library from SuiteSparse without the rest of the SuiteSparse source tree.
The CAMD algorithm, public header, upstream demos, and reference outputs are
synced verbatim. A small compatibility layer supplies only the
`SuiteSparse_config` functions CAMD uses.

## Build and test

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

Set `BUILD_SHARED_LIBS=ON` to build a shared library. Installation exports
both `CAMD::CAMD` and the SuiteSparse-compatible `SuiteSparse::CAMD` target.

## Sync from SuiteSparse

```sh
./scripts/sync-upstream.sh        # current upstream dev branch
./scripts/sync-upstream.sh v7.12.3
git diff -- UPSTREAM LICENSE include/camd.h include/SuiteSparse_config_version.h src/upstream tests/upstream
cmake -S . -B build && cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

`UPSTREAM` records the exact SuiteSparse commit and CAMD version. The scheduled
GitHub Actions workflow runs the same sync and test sequence, then opens or
updates a pull request when upstream changes are detected.

## Provenance and license

CAMD is maintained in the
[SuiteSparse repository](https://github.com/DrTimothyAldenDavis/SuiteSparse).
The imported files retain their original copyright and SPDX notices. See
`LICENSE` for CAMD's BSD-3-Clause license.
