# Alexa Presentation Language (APL) Client Library

<p>
 <a href="https://github.com/alexa/apl-client-library/tree/v1.8.1" alt="version">
 <img src="https://img.shields.io/badge/stable%20version-1.8.1-brightgreen" /></a>
 <a href="https://github.com/alexa/apl-core-library/tree/v1.8.1" alt="APLCore">
 <img src="https://img.shields.io/badge/apl%20core%20library-1.8.1-navy" /></a>
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
    git clone https://github.com/alexa/apl-client-library.git
    git clone https://github.com/zaphoyd/websocketpp.git
    git clone git://github.com/alexa/apl-core-library.git
    ```
1. Build apl-core
    ```
    cd apl-core-library
    source ./apl-dev-env.sh
    apl-build-core
    cd ..
    ```
1. Configure CMake in your working directory
    ```
    WORK_AREA=$(pwd)
    mkdir build
    cd build
    ```
    ```
    cmake ${WORK_AREA}/apl-client-library \
    -DWEBSOCKETPP_INCLUDE_DIR=${WORK_AREA}/websocketpp \
    -DCMAKE_BUILD_TYPE=DEBUG \
    -DAPLCORE_INCLUDE_DIR=${WORK_AREA}/apl-core-library/aplcore/include \
    -DAPLCORE_BUILD_INCLUDE_DIR=${WORK_AREA}/apl-core-library/build/aplcore/include \
    -DYOGA_INCLUDE_DIR=${WORK_AREA}/apl-core-library/build/yoga-prefix/src/yoga \
    -DAPLCORE_RAPIDJSON_INCLUDE_DIR=${WORK_AREA}/apl-core-library/build/rapidjson-prefix/src/rapidjson/include/ \
    -DAPLCORE_LIB_DIR=${WORK_AREA}/apl-core-library/build/aplcore \
    -DYOGA_LIB_DIR=${WORK_AREA}/apl-core-library/build/lib \
    -DAPLCORE_BUILD=${WORK_AREA}/apl-core-library/build \
    -DAPL_CORE=ON \
    -DBUILD_TESTING=OFF \
    -DSANDBOX=ON \
    -DSTANDALONE=ON
    ```
1. Build apl-client
   ```
   make
   ```
1. Run sandbox server
   ```
   cd $WORK_AREA/build/APLClientSandbox/src
   ./APLClientSandbox
   ```
1. Open another terminal to run sandbox GUI
   ```
   cd $WORK_AREA/apl-client-library/APLClientSandbox/GUI
   npm install
   npm start
   ```
1. Open localhost:8000 in your browser

## Security

See [CONTRIBUTING](CONTRIBUTING.md#security-issue-notifications) for more information.

## License

This project is licensed under the Apache-2.0 License.
