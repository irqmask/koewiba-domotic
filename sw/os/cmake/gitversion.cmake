# Use an annotated Git-Tag to assign a version to the koewiba software package
# The Git-Tag tag needs to be set in the main branch
# The format must be: v<major>.<minor> e.g. v0.1, v42.0

find_package(Git)
if (NOT GIT_FOUND)
    message(FATAL_ERROR "git executable not found!")
endif () 


# Git get version Major.minor.bugfix from tag
execute_process(
    COMMAND           "${GIT_EXECUTABLE}" describe --long
    RESULT_VARIABLE   GIT_RESULT
    OUTPUT_VARIABLE   GIT_DESCRIBE_OUTPUT
    ERROR_VARIABLE    GIT_ERROR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_STRIP_TRAILING_WHITESPACE    
)

if( NOT GIT_RESULT EQUAL 0 )
    set(VERSION_MAJOR 0)
    set(VERSION_MINOR 0)
    set(VERSION_BUGFIX 0)
else()
    # strip leading 'v'
    string(SUBSTRING ${GIT_DESCRIBE_OUTPUT} 1 -1 PURE_VERSION) 
    # split version string into major minor and bugfix part
    string(REPLACE "." "-" PURE_VERSION ${PURE_VERSION})
    string(REPLACE "-" ";" VERSION_LIST ${PURE_VERSION})
    list(GET VERSION_LIST 0 VERSION_MAJOR)
    list(GET VERSION_LIST 1 VERSION_MINOR)
    
    # check if a bugfix version exists or if we are right on the tag
    list(GET VERSION_LIST 2 VERSION_BUGFIX)
endif()


# Git get short-hash
execute_process(
    COMMAND           "${GIT_EXECUTABLE}" rev-parse --short=8 HEAD
    RESULT_VARIABLE   GIT_RESULT
    OUTPUT_VARIABLE   VERSION_SHORT_HASH
    ERROR_VARIABLE    GIT_ERROR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_STRIP_TRAILING_WHITESPACE
)
if( NOT GIT_RESULT EQUAL 0 )
    message(FATAL_ERROR "Failed to execute Git: ${GIT_ERROR}" )
endif()


# check if git repository is modified
execute_process(
    COMMAND           "${GIT_EXECUTABLE}" status --porcelain
    RESULT_VARIABLE   GIT_RESULT
    OUTPUT_VARIABLE   GIT_STATUS_OUTPUT
    ERROR_VARIABLE    GIT_ERROR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_STRIP_TRAILING_WHITESPACE    
)

if( NOT GIT_RESULT EQUAL 0 )
    set(VERSION_REPO_DIRTY 1)
    message(FATAL_ERROR "git status not checkable! ${GIT_ERROR}")
else()
    string(LENGTH "${GIT_STATUS_OUTPUT}" LENGTH_GIT_STATUS_OUTPUT)
    if(LENGTH_GIT_STATUS_OUTPUT GREATER 0)
        set(VERSION_REPO_DIRTY 1)
    else()
        set(VERSION_REPO_DIRTY 0)
    endif()
endif()

if(VERSION_REPO_DIRTY EQUAL 1)
    message("Version for ${VERSION_SOURCE_DIR} is: ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_BUGFIX}-${VERSION_SHORT_HASH} DIRTY")
else()
    message("Version for ${VERSION_SOURCE_DIR} is: ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_BUGFIX}-${VERSION_SHORT_HASH}")
endif()

configure_file(${CMAKE_SOURCE_DIR}/include/version.h.in ${CMAKE_SOURCE_DIR}/include/version.h @ONLY)
