# Changelog for apl-client-library
## [1.8.3]

### Changed

- Fix latency on renderer destruction
- Fix video player does not reload while destroying elements
- Fix audio node undefined error
- Fix execute commands not handling callbacks correctly

## [1.8.2]

### Changed

- Fixed scrolling issue with SpeakItem command when highlight mode set to line

## [1.8.0]

This release adds support for version 1.8 of the APL specification. Please also see APL Core Library for changes: [apl-core-library CHANGELOG](https://github.com/alexa/apl-core-library/blob/master/CHANGELOG.md)

### Added

- Image/VectorGraphics onLoad and onFail handlers
- Video onTrackReady and onTrackFail handlers
- DisplayState and onDisplayStateChange handler

### Changed

- Bug fixes

## [1.7.0]

This release adds support for version 1.7 of the APL specification.

### Added

- E2E Encryption extension

### Changed

- Updated reported APL version to 1.7
- Other bug fixes
