include(CMakeProject-lua.cmake)

if(NOT ANDROID)
include(CMakeProject-glew.cmake)
endif()

include(CMakeProject-json.cmake)
<<<<<<< HEAD
if (NOT SYSTEM_PCRE_FOUND)
  include(CMakeProject-pcre.cmake)
=======
include(CMakeProject-mad.cmake)
include(CMakeProject-zlib.cmake)
include(CMakeProject-jpeg.cmake)
if(WITH_OGG)
  if(WITH_SYSTEM_OGG)
    find_package(Ogg REQUIRED)
    find_package(Vorbis REQUIRED)
    find_package(VorbisFile REQUIRED)
  else()
    include(CMakeProject-ogg.cmake)
    include(CMakeProject-vorbis.cmake)
    include(CMakeProject-vorbisfile.cmake)
  endif()
>>>>>>> origin/unified-ui-features-13937230807013224518
endif()
include(CMakeProject-pcre.cmake)
include(CMakeProject-tomcrypt.cmake)
include(CMakeProject-tommath.cmake)
if (APPLE OR MSVC)
  include(CMakeProject-zlib.cmake)
endif()
include(CMakeProject-png.cmake)
<<<<<<< HEAD

if (NOT MSVC)
  include(CMakeProject-jpeg.cmake)
  if (APPLE OR ANDROID)
    include(CMakeProject-mad.cmake)
  endif()
endif()
=======
>>>>>>> origin/unified-ui-features-13937230807013224518
