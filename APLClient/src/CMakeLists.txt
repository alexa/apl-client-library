cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

add_library(APLClient SHARED
AplClientBinding.cpp
AplConfiguration.cpp
AplCoreAudioPlayer.cpp
AplCoreAudioPlayerFactory.cpp
AplCoreMediaPlayer.cpp
AplCoreMediaPlayerFactory.cpp
Extensions/AplCoreExtensionManager.cpp
Extensions/AttentionSystem/AplAttentionSystemExtension.cpp
Extensions/AudioPlayer/AplAudioPlayerExtension.cpp
Extensions/AudioPlayer/AplAudioPlayerAlarmsExtension.cpp
Extensions/Backstack/AplBackstackExtension.cpp
Telemetry/AplMetricsRecorder.cpp
Telemetry/AplMetricsRecorderInterface.cpp
Telemetry/DownloadMetricsEmitter.cpp
Telemetry/NullAplMetricsRecorder.cpp
AplCoreConnectionManager.cpp
AplCoreEngineLogBridge.cpp
AplCoreGuiRenderer.cpp
AplCoreMetrics.cpp
AplCoreTextMeasurement.cpp
AplCoreLocaleMethods.cpp
AplClientRenderer.cpp)

target_include_directories(APLClient PUBLIC "${APLClient_SOURCE_DIR}/include")

find_package(aplcore)
target_link_libraries(APLClient PUBLIC apl::core)

if (NOT MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-error -Wno-reorder -fPIC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error -Wno-reorder -fPIC")
elseif(MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive- /GS /TP /W3 /Zc:wchar_t /DAPL_CORE_UWP /Zc:inline /Zc:twoPhase- /fp:precise /DWIN32_LEAN_AND_MEAN /DNOGDI /DWIN32 /D_WINDOWS")
endif()

install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/" DESTINATION include)
install(TARGETS APLClient DESTINATION lib)
