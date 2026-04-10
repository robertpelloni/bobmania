<<<<<<< HEAD
set(MAD_SRC "libmad/bit.c"
            "libmad/decoder.c"
            "libmad/fixed.c"
            "libmad/frame.c"
            "libmad/huffman.c"
            "libmad/layer12.c"
            "libmad/layer3.c"
            "libmad/stream.c"
            "libmad/synth.c"
            "libmad/timer.c"
            "libmad/version.c")
=======
<<<<<<< HEAD
list(APPEND MAD_SRC
  "mad-0.15.1b/bit.c"
  "mad-0.15.1b/decoder.c"
  "mad-0.15.1b/fixed.c"
  "mad-0.15.1b/frame.c"
  "mad-0.15.1b/huffman.c"
  "mad-0.15.1b/layer12.c"
  "mad-0.15.1b/layer3.c"
  "mad-0.15.1b/stream.c"
  "mad-0.15.1b/synth.c"
  "mad-0.15.1b/timer.c"
  "mad-0.15.1b/version.c"
)

list(APPEND MAD_HPP
  "mad-0.15.1b/bit.h"
  "mad-0.15.1b/decoder.h"
  "mad-0.15.1b/fixed.h"
  "mad-0.15.1b/frame.h"
  "mad-0.15.1b/global.h"
  "mad-0.15.1b/huffman.h"
  "mad-0.15.1b/layer12.h"
  "mad-0.15.1b/layer3.h"
  "mad-0.15.1b/mad.h"
  "mad-0.15.1b/stream.h"
  "mad-0.15.1b/synth.h"
  "mad-0.15.1b/timer.h"
  "mad-0.15.1b/version.h"
)

if(ANDROID)
  #list(APPEND MAD_HPP "mad-0.15.1b/mad_android/config.h")
endif(ANDROID)

source_group("" FILES ${MAD_SRC} ${MAD_HPP})

add_library("mad" ${MAD_SRC} ${MAD_HPP})
=======
if(WITH_SYSTEM_MAD)
  find_package(Mad REQUIRED)
else()
  set(MAD_DIR "${SM_EXTERN_DIR}/mad-0.15.1b")
>>>>>>> main

set(MAD_HPP "libmad/bit.h"
            "libmad/config.h"
            "libmad/decoder.h"
            "libmad/fixed.h"
            "libmad/frame.h"
            "libmad/global.h"
            "libmad/huffman.h"
            "libmad/layer12.h"
            "libmad/layer3.h"
            "libmad/mad.h"
            "libmad/stream.h"
            "libmad/synth.h"
            "libmad/timer.h"
            "libmad/version.h")

set(MAD_DAT "libmad/D.dat"
            "libmad/imdct_s.dat"
            "libmad/qc_table.dat"
            "libmad/rq_table.dat"
            "libmad/sf_table.dat")

source_group("Source Files" FILES ${MAD_SRC})
source_group("Header Files" FILES ${MAD_HPP})
source_group("Data Files" FILES ${MAD_DAT})

add_library("mad" STATIC ${MAD_SRC} ${MAD_HPP} ${MAD_DAT})

<<<<<<< HEAD
set_property(TARGET "mad" PROPERTY FOLDER "External Libraries")
=======
  add_library("mad" STATIC ${MAD_SRC} ${MAD_HPP} ${MAD_DAT})
>>>>>>> origin/unified-ui-features-13937230807013224518
>>>>>>> main

disable_project_warnings("mad")

<<<<<<< HEAD
if(ENDIAN_BIG)
  set(WORDS_BIGENDIAN 1)
endif()

target_compile_definitions("mad" PRIVATE $<$<CONFIG:Debug>:DEBUG>)
target_compile_definitions("mad" PRIVATE HAVE_CONFIG_H)

if(MSVC)
  target_compile_definitions("mad" PRIVATE _CRT_SECURE_NO_WARNINGS)
  # TODO: Remove the need for this check since it's tied to 32-bit builds from
  # first glance.
  target_compile_definitions("mad" PRIVATE ASO_ZEROCHECK)
  target_compile_definitions("mad" PRIVATE $<$<CONFIG:Debug>:FPM_DEFAULT>)
  if(SM_WIN32_ARCH MATCHES "x64")
    target_compile_definitions("mad" PRIVATE $<$<CONFIG:Release>:FPM_64BIT>)
    target_compile_definitions("mad" PRIVATE $<$<CONFIG:MinSizeRel>:FPM_64BIT>)
    target_compile_definitions("mad" PRIVATE $<$<CONFIG:RelWithDebInfo>:FPM_64BIT>)
  else()
    target_compile_definitions("mad" PRIVATE $<$<CONFIG:Release>:FPM_INTEL>)
    target_compile_definitions("mad" PRIVATE $<$<CONFIG:MinSizeRel>:FPM_INTEL>)
    target_compile_definitions("mad" PRIVATE $<$<CONFIG:RelWithDebInfo>:FPM_INTEL>)
  endif()
  # TODO: Provide a proper define for inline.
  target_compile_definitions("mad" PRIVATE inline=__inline)
elseif(APPLE OR UNIX)
  target_compile_definitions("mad" PRIVATE FPM_64BIT=1)
endif(MSVC)

target_include_directories("mad" PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/libmad")
target_include_directories("mad" PUBLIC "libmad")

configure_file("config.mad.in.h" "libmad/config.h")
=======
  disable_project_warnings("mad")

<<<<<<< HEAD
if(MSVC)
  sm_add_compile_definition("mad" _CRT_SECURE_NO_WARNINGS)
elseif(APPLE)
  sm_add_compile_definition("mad" HAVE_ASSERT_H=1)
  sm_add_compile_definition("mad" HAVE_DLFCN_H=1)
  sm_add_compile_definition("mad" HAVE_ERRNO_H=1)
  sm_add_compile_definition("mad" HAVE_FORK=1)
  sm_add_compile_definition("mad" HAVE_INTTYPES_H=1)
  sm_add_compile_definition("mad" HAVE_LIMITS_H=1)
  sm_add_compile_definition("mad" HAVE_MEMORY_H=1)
  sm_add_compile_definition("mad" HAVE_WAITPID=1)
  sm_add_compile_definition("mad" SIZEOF_LONG=4)
  sm_add_compile_definition("mad" FPM_64BIT=1)
elseif(ANDROID)
  sm_add_compile_definition("mad" HAVE_ASSERT_H=1)
  sm_add_compile_definition("mad" HAVE_ERRNO_H=1)
  sm_add_compile_definition("mad" HAVE_FCNTL=1)
  sm_add_compile_definition("mad" HAVE_FCNTL_H=1)
  sm_add_compile_definition("mad" HAVE_FORK=1)
  sm_add_compile_definition("mad" HAVE_INTTYPES_H=1)
  sm_add_compile_definition("mad" HAVE_LIMITS_H=1)
  sm_add_compile_definition("mad" HAVE_PIPE=1)
  sm_add_compile_definition("mad" HAVE_STDLIB_H=1)
  sm_add_compile_definition("mad" HAVE_STRING_H=1)
  sm_add_compile_definition("mad" HAVE_SYS_TYPES_H=1)
  sm_add_compile_definition("mad" HAVE_SYS_WAIT_H=1)
  sm_add_compile_definition("mad" HAVE_UNISTD_H=1)
  sm_add_compile_definition("mad" HAVE_WAITPID=1)
  sm_add_compile_definition("mad" OPT_SPEED=1)
  sm_add_compile_definition("mad" SIZEOF_INT=4)
  sm_add_compile_definition("mad" STDC_HEADERS=1)
  sm_add_compile_definition("mad" FPM_ARM=1)
endif(MSVC)
=======
  if(ENDIAN_BIG)
    set(WORDS_BIGENDIAN 1)
  endif()

  sm_add_compile_definition("mad" $<$<CONFIG:Debug>:DEBUG>)
  sm_add_compile_definition("mad" HAVE_CONFIG_H)

  if(MSVC)
    sm_add_compile_definition("mad" _CRT_SECURE_NO_WARNINGS)
    # TODO: Remove the need for this check since it's tied to 32-bit builds from
    # first glance.
    sm_add_compile_definition("mad" ASO_ZEROCHECK)
    sm_add_compile_definition("mad" $<$<CONFIG:Debug>:FPM_DEFAULT>)
    if(SM_WIN32_ARCH MATCHES "x64")
      sm_add_compile_definition("mad" $<$<CONFIG:Release>:FPM_64BIT>)
      sm_add_compile_definition("mad" $<$<CONFIG:MinSizeRel>:FPM_64BIT>)
      sm_add_compile_definition("mad" $<$<CONFIG:RelWithDebInfo>:FPM_64BIT>)
    else()
      sm_add_compile_definition("mad" $<$<CONFIG:Release>:FPM_INTEL>)
      sm_add_compile_definition("mad" $<$<CONFIG:MinSizeRel>:FPM_INTEL>)
      sm_add_compile_definition("mad" $<$<CONFIG:RelWithDebInfo>:FPM_INTEL>)
    endif()
    # TODO: Provide a proper define for inline.
    sm_add_compile_definition("mad" inline=__inline)
  elseif(APPLE OR UNIX)
    sm_add_compile_definition("mad" FPM_64BIT=1)
  endif(MSVC)

  target_include_directories("mad" PUBLIC "${MAD_DIR}")

  configure_file("${SM_EXTERN_DIR}/config.mad.in.h" "${MAD_DIR}/config.h")
endif()
>>>>>>> origin/unified-ui-features-13937230807013224518
>>>>>>> main
