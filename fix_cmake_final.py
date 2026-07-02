import os

with open('src/CMakeLists.txt', 'r') as f:
    content = f.read()

target1 = """    XCODE_ATTRIBUTE_LIBRARY_SEARCH_PATHS "${SM_XCODE_DIR}/ffmpeg/lib ${SM_XCODE_DIR}/Libraries"
    XCODE_ATTRIBUTE_OTHER_LDFLAGS "-lbz2"

  set_target_properties("${SM_EXE_NAME}" """
replacement1 = """    XCODE_ATTRIBUTE_LIBRARY_SEARCH_PATHS "${SM_XCODE_DIR}/ffmpeg/lib ${SM_XCODE_DIR}/Libraries"
    XCODE_ATTRIBUTE_OTHER_LDFLAGS "-lbz2"
  )

  set_target_properties("${SM_EXE_NAME}" """
content = content.replace(target1, replacement1)

target2 = """else() # Unix / Linux
  # TODO: Remember to find and locate the zip archive files.
  if (HAS_FFMPEG)
  set_target_properties("${SM_EXE_NAME}"
                        PROPERTIES LINK_FLAGS_DEBUG "/NODEFAULTLIB:msvcrt.lib")
  set_target_properties("${SM_EXE_NAME}"
                        PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
  set_target_properties("${SM_EXE_NAME}"
                        PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")

elseif(APPLE)"""
replacement2 = """else() # Unix / Linux
  # TODO: Remember to find and locate the zip archive files.
  if (HAS_FFMPEG)
  endif()
  set_target_properties("${SM_EXE_NAME}"
                        PROPERTIES LINK_FLAGS_DEBUG "/NODEFAULTLIB:msvcrt.lib")
  set_target_properties("${SM_EXE_NAME}"
                        PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
  set_target_properties("${SM_EXE_NAME}"
                        PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")

elseif(APPLE)"""
content = content.replace(target2, replacement2)

target3 = """if(NOT APPLE)
  list(APPEND SM_INCLUDE_DIRS
    "${SM_EXTERN_DIR}/glew-1.5.8/include"
    "${SM_EXTERN_DIR}/jsoncpp/include"
    "${SM_EXTERN_DIR}/libjpeg"
    "${SM_EXTERN_DIR}/zlib"
  )
  if(MSVC)
list(APPEND SM_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
            "${SM_SRC_DIR}/generated")
list(APPEND SM_INCLUDE_DIRS "${JPEG_INCLUDE_DIR}")
if(NOT APPLE)"""
replacement3 = """if(NOT APPLE)
  list(APPEND SM_INCLUDE_DIRS
    "${SM_EXTERN_DIR}/glew-1.5.8/include"
    "${SM_EXTERN_DIR}/jsoncpp/include"
    "${SM_EXTERN_DIR}/libjpeg"
    "${SM_EXTERN_DIR}/zlib"
  )
endif()
if(MSVC)
list(APPEND SM_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
            "${SM_SRC_DIR}/generated")
endif()
list(APPEND SM_INCLUDE_DIRS "${JPEG_INCLUDE_DIR}")
if(NOT APPLE)"""
content = content.replace(target3, replacement3)

with open('src/CMakeLists.txt', 'w') as f:
    f.write(content)
