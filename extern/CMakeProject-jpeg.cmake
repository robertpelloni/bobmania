<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> main
list(APPEND JPEG_SRC
  "libjpeg/ansi2knr.c"
  "libjpeg/cdjpeg.c"
  "libjpeg/cjpeg.c"
  "libjpeg/ckconfig.c"
  "libjpeg/djpeg.c"
  "libjpeg/jaricom.c"
  "libjpeg/jcapimin.c"
  "libjpeg/jcapistd.c"
  "libjpeg/jcarith.c"
  "libjpeg/jccoefct.c"
  "libjpeg/jccolor.c"
  "libjpeg/jcdctmgr.c"
  "libjpeg/jchuff.c"
  "libjpeg/jcinit.c"
  "libjpeg/jcmainct.c"
  "libjpeg/jcmarker.c"
  "libjpeg/jcmaster.c"
  "libjpeg/jcomapi.c"
  "libjpeg/jcparam.c"
  "libjpeg/jcprepct.c"
  "libjpeg/jcsample.c"
  "libjpeg/jctrans.c"
  "libjpeg/jdapimin.c"
  "libjpeg/jdapistd.c"
  "libjpeg/jdarith.c"
  "libjpeg/jdatadst.c"
  "libjpeg/jdcoefct.c"
  "libjpeg/jdcolor.c"
  "libjpeg/jddctmgr.c"
  "libjpeg/jdhuff.c"
  "libjpeg/jdinput.c"
  "libjpeg/jdmainct.c"
  "libjpeg/jdmarker.c"
  "libjpeg/jdmaster.c"
  "libjpeg/jdmerge.c"
  "libjpeg/jdpostct.c"
  "libjpeg/jdsample.c"
  "libjpeg/jdtrans.c"
  "libjpeg/jerror.c"
  "libjpeg/jfdctflt.c"
  "libjpeg/jfdctfst.c"
  "libjpeg/jfdctint.c"
  "libjpeg/jutils.c"
  "libjpeg/jidctflt.c"
  "libjpeg/jidctfst.c"
  "libjpeg/jidctint.c"
  "libjpeg/jmemansi.c"
  "libjpeg/jmemmgr.c"
  "libjpeg/jmemname.c"
  "libjpeg/jmemnobs.c"
  "libjpeg/jquant1.c"
  "libjpeg/jquant2.c"
)
<<<<<<< HEAD
=======

source_group("" FILES ${JPEG_SRC})

add_library("jpeg" ${JPEG_SRC})

disable_project_warnings("jpeg")

set_property(TARGET "jpeg" PROPERTY FOLDER "External Libraries")

if(MSVC)
  sm_add_compile_definition("jpeg" _CRT_SECURE_NO_WARNINGS)
elseif(ANDROID)
  sm_add_compile_definition("jpeg" STDC_HEADERS=1)
endif(MSVC)
=======
if(WITH_SYSTEM_JPEG)
  find_package(JPEG REQUIRED)
  set(JPEG_LIBRARIES ${JPEG_LIBRARIES} PARENT_SCOPE)
else()
  set(JPEG_DIR "${SM_EXTERN_DIR}/libjpeg")
>>>>>>> main

source_group("" FILES ${JPEG_SRC})

add_library("jpeg" ${JPEG_SRC})

disable_project_warnings("jpeg")

set_property(TARGET "jpeg" PROPERTY FOLDER "External Libraries")

<<<<<<< HEAD
if(MSVC)
  sm_add_compile_definition("jpeg" _CRT_SECURE_NO_WARNINGS)
elseif(ANDROID)
  sm_add_compile_definition("jpeg" STDC_HEADERS=1)
endif(MSVC)
=======
  source_group("Source Files" FILES ${JPEG_SRC})
  source_group("Header Files" FILES ${JPEG_HPP})

  add_library("jpeg" ${JPEG_SRC} ${JPEG_HPP})

  disable_project_warnings("jpeg")

  set_property(TARGET "jpeg" PROPERTY FOLDER "External Libraries")

  if(MSVC)
    sm_add_compile_definition("jpeg" _CRT_SECURE_NO_WARNINGS)
  endif(MSVC)

  target_include_directories("jpeg" PUBLIC "${JPEG_DIR}")
endif()
>>>>>>> origin/unified-ui-features-13937230807013224518
>>>>>>> main
