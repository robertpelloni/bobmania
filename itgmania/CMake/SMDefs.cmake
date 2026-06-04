# Set up version numbers according to the new scheme.
file(READ "${SM_ROOT_DIR}/VERSION.md" SM_VERSION_STRING)
string(STRIP "${SM_VERSION_STRING}" SM_VERSION_STRING)
string(REPLACE "." ";" SM_VERSION_LIST "${SM_VERSION_STRING}")
list(GET SM_VERSION_LIST 0 SM_VERSION_MAJOR)
list(GET SM_VERSION_LIST 1 SM_VERSION_MINOR)
list(GET SM_VERSION_LIST 2 SM_VERSION_PATCH)


set(SM_VERSION_TRADITIONAL
    "${SM_VERSION_MAJOR}.${SM_VERSION_MINOR}.${SM_VERSION_PATCH}")

execute_process(COMMAND git rev-parse --short HEAD
                WORKING_DIRECTORY "${SM_ROOT_DIR}"
                OUTPUT_VARIABLE SM_VERSION_GIT_HASH
                RESULT_VARIABLE ret
                OUTPUT_STRIP_TRAILING_WHITESPACE)

if(NOT (ret STREQUAL "0"))
  message(
    WARNING
      "git was not found on your path. If you collect bug reports, please add git to your path and rerun cmake."
    )
  set(SM_VERSION_GIT_HASH "UNKNOWN")
  if(WITH_FULL_RELEASE)
    set(SM_VERSION_GIT
        "${SM_VERSION_MAJOR}.${SM_VERSION_MINOR}.${SM_VERSION_PATCH}")
  else()
    if(WITH_NIGHTLY_RELEASE)
      set(SM_VERSION_GIT
        "${SM_VERSION_MAJOR}.${SM_VERSION_MINOR}.${SM_VERSION_PATCH}-NIGHTLY-${SM_VERSION_GIT_HASH}")
    else()
      set(SM_VERSION_GIT
          "${SM_VERSION_MAJOR}.${SM_VERSION_MINOR}.${SM_VERSION_PATCH}-BETA-${SM_VERSION_GIT_HASH}")
    endif()
  endif()
else()
  if(WITH_FULL_RELEASE)
    set(SM_VERSION_GIT
        "${SM_VERSION_MAJOR}.${SM_VERSION_MINOR}.${SM_VERSION_PATCH}")
  else()
    if(WITH_NIGHTLY_RELEASE)
      set(SM_VERSION_GIT
        "${SM_VERSION_MAJOR}.${SM_VERSION_MINOR}.${SM_VERSION_PATCH}-NIGHTLY-git-${SM_VERSION_GIT_HASH}")
    else()
      set(SM_VERSION_GIT
          "${SM_VERSION_MAJOR}.${SM_VERSION_MINOR}.${SM_VERSION_PATCH}-BETA-git-${SM_VERSION_GIT_HASH}")
    endif()
  endif()
endif()

string(TIMESTAMP SM_TIMESTAMP_DATE "%Y%m%d")
string(TIMESTAMP SM_TIMESTAMP_TIME "%H:%M:%S" UTC)
