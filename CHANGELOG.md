## [Unreleased]

### Added

- `VERSION`: Add inital version
- `cli/libtman`: Add function to check if task ID exists

### Changed

- `src/unit`: Remove unit generation
- `src/unit`: Remove project unit generation
- `src/libtman`: Move column name no `struct tman_context`

### Fixed

- `src`: Remove non-public structures
- `src/task`: Fix memleak in strncpy()
- `src`: Fix access to private structures

### Deleted

- `src`: Delete unused API functions
