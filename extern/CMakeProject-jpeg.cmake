<<<<<<< HEAD
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

  configure_file("${SM_EXTERN_DIR}/config.jpeg.in.h" "${JPEG_DIR}/jconfig.h")

  if(NOT HAVE_PROTOTYPES)
    list(APPEND JPEG_SRC "${JPEG_DIR}/ansi2knr.c")
  endif()

  list(APPEND JPEG_SRC
              "${JPEG_DIR}/jaricom.c"
              "${JPEG_DIR}/jcapimin.c"
              "${JPEG_DIR}/jcapistd.c"
              "${JPEG_DIR}/jcarith.c"
              "${JPEG_DIR}/jccoefct.c"
              "${JPEG_DIR}/jccolor.c"
              "${JPEG_DIR}/jcdctmgr.c"
              "${JPEG_DIR}/jchuff.c"
              "${JPEG_DIR}/jcinit.c"
              "${JPEG_DIR}/jcmainct.c"
              "${JPEG_DIR}/jcmarker.c"
              "${JPEG_DIR}/jcmaster.c"
              "${JPEG_DIR}/jcomapi.c"
              "${JPEG_DIR}/jcparam.c"
              "${JPEG_DIR}/jcprepct.c"
              "${JPEG_DIR}/jcsample.c"
              "${JPEG_DIR}/jctrans.c"
              "${JPEG_DIR}/jdapimin.c"
              "${JPEG_DIR}/jdapistd.c"
              "${JPEG_DIR}/jdarith.c"
              "${JPEG_DIR}/jdatadst.c"
              "${JPEG_DIR}/jdcoefct.c"
              "${JPEG_DIR}/jdcolor.c"
              "${JPEG_DIR}/jddctmgr.c"
              "${JPEG_DIR}/jdhuff.c"
              "${JPEG_DIR}/jdinput.c"
              "${JPEG_DIR}/jdmainct.c"
              "${JPEG_DIR}/jdmarker.c"
              "${JPEG_DIR}/jdmaster.c"
              "${JPEG_DIR}/jdmerge.c"
              "${JPEG_DIR}/jdpostct.c"
              "${JPEG_DIR}/jdsample.c"
              "${JPEG_DIR}/jdtrans.c"
              "${JPEG_DIR}/jerror.c"
              "${JPEG_DIR}/jfdctflt.c"
              "${JPEG_DIR}/jfdctfst.c"
              "${JPEG_DIR}/jfdctint.c"
              "${JPEG_DIR}/jutils.c"
              "${JPEG_DIR}/jidctflt.c"
              "${JPEG_DIR}/jidctfst.c"
              "${JPEG_DIR}/jidctint.c"
              "${JPEG_DIR}/jmemmgr.c"
              "${JPEG_DIR}/jmemnobs.c"
              "${JPEG_DIR}/jquant1.c"
              "${JPEG_DIR}/jquant2.c")

  list(APPEND JPEG_HPP
              "${JPEG_DIR}/jconfig.h"
              "${JPEG_DIR}/jdct.h"
              "${JPEG_DIR}/jerror.h"
              "${JPEG_DIR}/jinclude.h"
              "${JPEG_DIR}/jmemsys.h"
              "${JPEG_DIR}/jmorecfg.h"
              "${JPEG_DIR}/jpegint.h"
              "${JPEG_DIR}/jpeglib.h"
              "${JPEG_DIR}/jversion.h")

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
