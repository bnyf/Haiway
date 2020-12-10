cmake_minimum_required(VERSION 3.13)
include(ExternalProject)
project(freertos_examples)
add_definitions(-D CMAKE_TOOLCHAIN_FILE=${HAIWAY}/kernel/tools/cmake/toolchains/xtensa-esp32.cmake)
aux_source_directory(src DIR_SRCS)
add_executable(afr_demo ${DIR_SRCS})

# Tell IDF build to link against this target.
set(IDF_PROJECT_EXECUTABLE afr_demo)

# Add some extra components. IDF_EXTRA_COMPONENT_DIRS is an variable used by ESP-IDF
# to collect extra components.
get_filename_component(
    EXTRA_COMPONENT_DIRS
    "components/foo" ABSOLUTE
)
list(APPEND IDF_EXTRA_COMPONENT_DIRS ${EXTRA_COMPONENT_DIRS})

add_definitions(-D AFR_ESP_FREERTOS_TCP)
# As of now there's no offical way to redefine config files outside of FreeRTOS source tree.
# This is a temporary approach to inject an include path so that this takes precedence over the
# config file directory inside FreeRTOS.
include_directories(BEFORE freertos-configs)

# jerry
set(JERRYSCRIPT_SOURCE $ENV{HAIWAY}/libs/jerryscript)
set(JERRY_GLOBAL_HEAP_SIZE "(64)")
include_directories(${JERRYSCRIPT_SOURCE}/jerry-core/include ${JERRYSCRIPT_SOURCE}/jerry-ext/include ${JERRYSCRIPT_SOURCE}/jerry-port/default/include)
# Xtensa processor architecture optimization
set(EXTERNAL_COMPILE_FLAGS -ffunction-sections -fdata-sections -fstrict-volatile-bitfields -mlongcalls -nostdlib -w)
string(REPLACE ";" "|" EXTERNAL_COMPILE_FLAGS_ALT_SEP "${EXTERNAL_COMPILE_FLAGS}")
# externalproject
externalproject_add(jerryscript_build
  PREFIX ${COMPONENT_DIR}
  SOURCE_DIR ${JERRYSCRIPT_SOURCE}
  BUILD_IN_SOURCE 0
  BINARY_DIR jerryscript
  INSTALL_COMMAND "" # Do not install to host
  LIST_SEPARATOR | # Use the alternate list separator
  CMAKE_ARGS
    -DJERRY_GLOBAL_HEAP_SIZE=${JERRY_GLOBAL_HEAP_SIZE}
    -DJERRY_CMDLINE=OFF
    -DENABLE_LTO=OFF # FIXME: This option must be turned off or the cross-compiler settings will be overwritten
    -DCMAKE_C_COMPILER_WORKS=true # cross-compiler
    -DCMAKE_SYSTEM_NAME=Generic
    -DCMAKE_SYSTEM_PROCESSOR=xtensa
    -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
    -DEXTERNAL_COMPILE_FLAGS=${EXTERNAL_COMPILE_FLAGS_ALT_SEP}
    -DCMAKE_EXE_LINKER_FLAGS=${CMAKE_EXE_LINKER_FLAGS}
    -DCMAKE_LINKER=${CMAKE_LINKER}
    -DCMAKE_AR=${CMAKE_AR}
    -DCMAKE_NM=${CMAKE_NM}
    -DCMAKE_RANLIB=${CMAKE_RANLIB}
    -DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER
)
add_dependencies(afr_demo jerryscript_build)
set(COMPONENT_BUILD_PATH ${CMAKE_BINARY_DIR}/jerryscript)

# Add freertos as an subdirectory. AFR_BOARD tells which board to target.
set(AFR_BOARD espressif.esp32_devkitc CACHE INTERNAL "")
add_subdirectory($ENV{HAIWAY}/kernel kernel)

# Link against the mqtt demo so that we can use it. Dependencies of this demo are transitively
# linked.
target_link_libraries(afr_demo 
                    AFR::core_mqtt
                    ${COMPONENT_BUILD_PATH}/lib/libjerry-core.a
                    ${COMPONENT_BUILD_PATH}/lib/libjerry-ext.a
                    ${COMPONENT_BUILD_PATH}/lib/libjerry-port-default-minimal.a)
