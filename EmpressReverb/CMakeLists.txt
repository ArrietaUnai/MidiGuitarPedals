project(EmpressReverb VERSION 1.0.0)

juce_add_gui_app(${PROJECT_NAME} PRODUCT_NAME "EmpressReverb")
juce_generate_juce_header(${PROJECT_NAME})

add_definitions(-DIMAGE_PEDAL="${CMAKE_SOURCE_DIR}/EmpressReverb/Resources/EmpressReverb.png")
add_definitions(-DIMAGE_BYPASS="${CMAKE_SOURCE_DIR}/EmpressReverb/Resources/BypassResized.png")

add_definitions(-DIMAGE_KNOB_MARKER="${CMAKE_SOURCE_DIR}/EmpressReverb/Resources/KnobMarkerFull.png")

add_definitions(-DIMAGE_PRESET1="${CMAKE_SOURCE_DIR}/EmpressReverb/Resources/PresetLights/Preset1Resized.png")
add_definitions(-DIMAGE_PRESET2="${CMAKE_SOURCE_DIR}/EmpressReverb/Resources/PresetLights/Preset2Resized.png")
add_definitions(-DIMAGE_PRESET3="${CMAKE_SOURCE_DIR}/EmpressReverb/Resources/PresetLights/Preset3Resized.png")
add_definitions(-DIMAGE_PRESET4="${CMAKE_SOURCE_DIR}/EmpressReverb/Resources/PresetLights/Preset4Resized.png")
add_definitions(-DIMAGE_PRESET5="${CMAKE_SOURCE_DIR}/EmpressReverb/Resources/PresetLights/Preset5Resized.png")
add_definitions(-DIMAGE_PRESET6="${CMAKE_SOURCE_DIR}/EmpressReverb/Resources/PresetLights/Preset6Resized.png")
add_definitions(-DIMAGE_PRESET7="${CMAKE_SOURCE_DIR}/EmpressReverb/Resources/PresetLights/Preset7Resized.png")

add_definitions(-DIMAGE_MODE_BLUE="${CMAKE_SOURCE_DIR}/EmpressReverb/Resources/Colors/BlueMode.png")
add_definitions(-DIMAGE_MODE_GREEN="${CMAKE_SOURCE_DIR}/EmpressReverb/Resources/Colors/GreenMode.png")
add_definitions(-DIMAGE_MODE_RED="${CMAKE_SOURCE_DIR}/EmpressReverb/Resources/Colors/RedMode.png")
add_definitions(-DIMAGE_MODE_YELLOW="${CMAKE_SOURCE_DIR}/EmpressReverb/Resources/Colors/YellowMode.png")

target_sources(${PROJECT_NAME} PRIVATE
        Source/defines.h
        Source/EmpressReverb.h
        Source/EmpressReverb.cpp
        Source/FootSwitch.cpp
        Source/FootSwitch.h
        Source/Knobs.cpp
        Source/Knobs.h
        Source/LightsTimer.h
        Source/LightsTimer.cpp
        Source/EmpressReverbStates.h
        Source/EmpressReverbStates.cpp
        Source/MidiDeviceList.cpp
        Source/MidiDeviceList.h
        Source/Main.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:${PROJECT_NAME},JUCE_PROJECT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:${PROJECT_NAME},JUCE_VERSION>")

target_link_libraries(${PROJECT_NAME} PRIVATE
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_data_structures
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
        juce::juce_core)
