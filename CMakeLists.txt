cmake_minimum_required(VERSION 3.16)
project(MidiGuitarPedals)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#adding my own custom git functions, to save some boilerplate:
include (CMake/GitFunctions.cmake)

#calling my custom function to grab JUCE 6 from git:
update_from_git(juce "https://github.com/juce-framework/juce" develop)

#Adds all the module sources so they appear correctly in the IDE
set_property(GLOBAL PROPERTY USE_FOLDERS YES)
option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Enable Module Source Groups" ON)

#set any of these to "ON" if you want to build one of the juce examples
#or extras (Projucer/AudioPluginHost, etc):
option(JUCE_BUILD_EXTRAS "Build JUCE Extras" OFF)
option(JUCE_BUILD_EXAMPLES "Build JUCE Examples" OFF)

#adding juce as a subdirectory:
add_subdirectory(${juce_SOURCE_DIR})

option(BUILD_EMPRESS_REVERB "Build Empress Reverb" ON)

if (${BUILD_EMPRESS_REVERB})
    add_subdirectory(EmpressReverb)
endif()
