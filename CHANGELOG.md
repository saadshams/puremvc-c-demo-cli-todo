# Changelog

All notable changes to this project will be documented in this file.

## [1.0.0] - 2026-02-20
### Added
- Initial Todo CLI implementation
- File isolation in tests using copy strategy to avoid mutating original data
- Support for short and long CLI options:
    - `--title` / `-t`
    - `--completed` / `-c`
    - `--help` / `-h`
    - `--version` / `-v`
- Storage abstraction layer - JSON and text storage support
- Integration tests for:
  - list
  - add
  - edit
  - delete