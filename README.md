# Alexa Presentation Language (APL) Client Library

<p>
 <a href="https://github.com/alexa/apl-client-library/tree/v2024.1.0" alt="version">
 <img src="https://img.shields.io/badge/stable%20version-2024.1.0-brightgreen" /></a>
 <a href="https://github.com/alexa/apl-core-library/tree/v2024.1.0" alt="APLCore">
 <img src="https://img.shields.io/badge/apl%20core%20library-2024.1.0-navy" /></a>
</p>
 
## Introduction

The APL Client Library provides communication between APLViewhostWeb and apl-core-library. Its job is making sure that APL Documents are correctly rendered and getting updated based on user's interaction.

The APL Client Library package in this GitHub repo includes:

- The APL Client Library
- A sandbox that enables users to test APL Client Library's functionalities
- apl-client-js: APLViewhostWeb

The APL Client Library depends on the following additional GitHub repos:

- [APL Core Library](https://github.com/alexa/apl-core-library)

## Installation

### Prerequisites

* [cmake](https://cmake.org/install/) - the easiest way to install on Mac is using `brew install cmake`
* [Asio](https://think-async.com/Asio/) - the easiest way to install on Mac is using `brew install asio`
* [websocketpp](https://github.com/zaphoyd/websocketpp) - fetched in CMake if `FETCH_CONTENT` flag is set to `ON`

### Sandbox Installation Steps

1. Clone the necessary repos in your working directory
    ```
    WORK_AREA=$(pwd)
    git clone https://github.com/alexa/apl-client-library.git
    git clone https://github.com/zaphoyd/websocketpp.git
    git clone git://github.com/alexa/apl-core-library.git
    ```
1. Prepare the export directory
    ```
    cd ${WORK_AREA}
    mkdir apl-exports
    export APL_EXPORTS_PATH=${WORK_AREA}/apl-exports
    ```
1. Build apl-core
    ```
    cd ${WORK_AREA}/apl-core-library
    source ./apl-dev-env.sh
    mkdir build
    cd build
    cmake -DBUILD_TESTS=OFF -DCOVERAGE=OFF -DENABLE_PIC=ON -DBUILD_ALEXAEXTENSIONS=ON -DCMAKE_INSTALL_PREFIX=${APL_EXPORTS_PATH} ..
    make -j8
    make install
    ```
1. Configure CMake in your working directory
    ```
    cd ${WORK_AREA}
    mkdir build
    cd build
    ```
    ```
    cmake ${WORK_AREA}/apl-client-library \
        -DCMAKE_BUILD_TYPE=DEBUG \
        -DCMAKE_PREFIX_PATH=${APL_EXPORTS_PATH} \
        -DWEBSOCKETPP_INCLUDE_DIR=${WORK_AREA}/websocketpp \
        -DSANDBOX=ON
    ```
1. Build apl-client
   ```
   make -j8
   ```
1. Run sandbox server
   ```
   cd ${WORK_AREA}/build/APLClientSandbox/src
   ./APLClientSandbox
   ```
1. Open another terminal to run sandbox GUI
   ```
   cd ${WORK_AREA}/apl-client-library/APLClientSandbox/GUI
   npm install
   npm start
   ```
1. Open localhost:8000 in your browser

### Run unit tests
To include unit tests, when comfiguring CMake
```
cmake ${WORK_AREA}/apl-client-library \
    -DCMAKE_BUILD_TYPE=DEBUG \
    -DCMAKE_PREFIX_PATH=${APL_EXPORTS_PATH} \
    -DWEBSOCKETPP_INCLUDE_DIR=${WORK_AREA}/websocketpp \
    -DSANDBOX=ON \
    -DBUILD_UNIT_TESTS=ON
```

### Export the client library
When configuring CMake, add `CMAKE_INSTALL_PREFIX`
```
cmake ${WORK_AREA}/apl-client-library \
    -DCMAKE_BUILD_TYPE=DEBUG \
    -DCMAKE_PREFIX_PATH=${APL_EXPORTS_PATH} \
    -DCMAKE_INSTALL_PREFIX=${APL_EXPORTS_PATH}
```
then
```
make install
```

## Security

See [CONTRIBUTING](CONTRIBUTING.md#security-issue-notifications) for more information.

## License

This project is licensed under the Apache-2.0 License.
