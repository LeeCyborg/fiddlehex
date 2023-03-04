find_package(Doxygen REQUIRED)

if(DOXYGEN_FOUND)
    message(STATUS "DOXYGEN_VERSION: ${DOXYGEN_VERSION}")
endif()

set(DOCS_TARGET_NAME FiddleHexDmxDocs)

# project info
set(DOXYGEN_PROJECT_NAME ${PROJECT_NAME})
set(DOXYGEN_PROJECT_NUMBER ${PROJECT_VERSION})
set(DOXYGEN_PROJECT_BRIEF ${PROJECT_DESCRIPTION})
# doxygen-awesome
set(DOXYGEN_GENERATE_HTML YES)
set(DOXYGEN_GENERATE_TREEVIEW YES)
set(DOXYGEN_DISABLE_INDEX NO)
set(DOXYGEN_FULL_SIDEBAR NO)
set(DOXYGEN_HTML_EXTRA_STYLESHEET doxygen-awesome-css/doxygen-awesome.css
                                  doxygen-awesome-css/doxygen-awesome-sidebar-only.css
                                  custom-theme.css)
set(DOXYGEN_HTML_COLORSTYLE LIGHT)
# dot
set(DOXYGEN_HAVE_DOT YES)
set(DOXYGEN_DOT_IMAGE_FORMAT svg)
set(DOXYGEN_DOT_TRANSPARENT YES)
set(DOXYGEN_INTERACTIVE_SVG YES)
# do not extract all
set(DOXYGEN_EXTRACT_ALL NO)
set(DOXYGEN_EXTRACT_PRIVATE NO)
set(DOXYGEN_EXTRACT_STATIC  NO)
# hide undocumented
set(DOXYGEN_HIDE_UNDOC_MEMBERS YES)
set(DOXYGEN_HIDE_UNDOC_CLASSES YES)
set(DOXYGEN_HIDE_FRIEND_COMPOUNDS YES)
set(DOXYGEN_HIDE_IN_BODY_DOCS YES)
# strip absolute path
set(DOXYGEN_STRIP_FROM_PATH ${CMAKE_SOURCE_DIR})

doxygen_add_docs(${DOCS_TARGET_NAME}
    "../include"
    "../src"
    COMMENT "Generate html docs"
)
