# Changelog for apl-client-library

## [2024.1]
This release adds support for version 2024.1 of the APL specification. Please also see APL Core Library for changes: [apl-core-library CHANGELOG](https://github.com/alexa/apl-core-library/blob/master/CHANGELOG.md)

### Added
- Add Log Command

### Changed

- Bug fixes

## [2023.3]
This release adds support for version 2023.3 of the APL specification. Please also see APL Core Library for changes: [apl-core-library CHANGELOG](https://github.com/alexa/apl-core-library/blob/master/CHANGELOG.md)

### Added
- Support Alexaext extensions
- Support gradient as Frame background
- Support conditional imports
- Exported caption control class cueControl for APL videos

### Changed

- Changed default font to sans-serif to meet the APL Spec
- Bug fixes

## [2023.2]
This release adds support for version 2023.2 of the APL specification. Please also see APL Core Library for changes: [apl-core-library CHANGELOG](https://github.com/alexa/apl-core-library/blob/master/CHANGELOG.md)

### Added
- Add support for the seekTo ControlMedia command

### Changed
- Remove usage of APL Core Library's deprecated getTheme API
- Bug fixes

## [2023.1]
This release adds support for version 2023.1 of the APL specification. Please also see APL Core Library for changes: [apl-core-library CHANGELOG](https://github.com/alexa/apl-core-library/blob/master/CHANGELOG.md)

### Added
- SRT support for APL Video textTrack
- Support for new Porter-Duff blend modes

### Changed
- Bug fixes

## [2022.2]
This release adds support for version 2022.2 of the APL specification. Please also see APL Core Library for changes: [apl-core-library CHANGELOG](https://github.com/alexa/apl-core-library/blob/master/CHANGELOG.md)

### Added
- Support continued command execution during APL document re-inflation

### Changed
- Bug fixes

## [2022.1.1]

### Changed

- Bug fixes

## [2022.1]

This release adds support for version 2022.1 of the APL specification.
Please also see APL Core Library for changes: [apl-core-library
CHANGELOG](https://github.com/alexa/apl-core-library/blob/master/CHANGELOG.md)

### Added

- Added muted property to video component
- Exposed scrollCommandDuration

### Changed

- Improved support for HLS video
- Scale factor can be specified independently per renderer
- Changed clipping behaviour for documents using APL <= 1.5 for backwards compatibility
- Bug fixes

## [1.9.0]

This release adds support for version 1.9 of the APL specification. Please also see APL Core Library for changes: [apl-core-library CHANGELOG](https://github.com/alexa/apl-core-library/blob/master/CHANGELOG.md)

### Added

- Image/VectorGraphics auth headers
- Disallow flags for video, edit text, and dialog
- Custom environment properties

### Changed

- Minor bug fixes

## [1.8.0]

This release adds support for version 1.8 of the APL specification. Please also see APL Core Library for changes: [apl-core-library CHANGELOG](https://github.com/alexa/apl-core-library/blob/master/CHANGELOG.md)

### Added

- Image/VectorGraphics onLoad and onFail handlers
- Video onTrackReady and onTrackFail handlers
- DisplayState and onDisplayStateChange handler

### Changed

- Images are now being retrieved through fetch API -- CORS is now enforced

## [1.7.0]

This release adds support for version 1.7 of the APL specification.

### Added

- E2E Encryption extension

### Changed

- Updated reported APL version to 1.7
- Other bug fixes
