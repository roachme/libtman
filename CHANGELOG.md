## [Unreleased]

### Added

- `src`: Add name and length validation functions
- `src`: Add support to check board feature usage

### Fixed

- `src/libtman`: Fix potential memory leak
- `src/libtman`: Fix project renaming


## [0.0.20] - (2025-07-10)

### Added

- `VERSION`: Add inital version
- `cli/libtman`: Add function to check if task ID exists
- `src`: Add skeleton for board functions
- `src`: Add new error code for memory failure

### Changed

- `src/unit`: Remove unit generation
- `src/unit`: Remove project unit generation
- `src/libtman`: Move column name no `struct tman_context`

### Fixed

- `src`: Remove non-public structures
- `src/task`: Fix memleak in strncpy()
- `src`: Fix access to private structures
- `src/common`: Add missing option `-P` for hook calls
- `src/libtman`: Replace struct with typedef
- `src`: Move library limits into one place

### Deleted

- `src`: Delete unused API functions
- `src`: Move hooks to CLI part
- `src/common`: Remove plugin command generation
- `src/libtman`: Delete unused member in struct option
- `src/libtman`: Delete id member from `tman_ctx_t`
- `src`: Remove plugin notion from lib part
