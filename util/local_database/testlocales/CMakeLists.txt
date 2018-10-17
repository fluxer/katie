cmake_minimum_required(VERSION 2.8.12.2 FATAL_ERROR)

project(testlocales CXX)

include(FeatureSummary)

set(CMAKE_AUTOMOC TRUE)

find_package(Katie REQUIRED)

add_definitions(${KATIE_DEFINITIONS})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${KATIE_INCLUDES}
)

set(testlocales_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/localemodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/localewidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(testlocales ${testlocales_SOURCES})
target_link_libraries(testlocales ${KATIE_GUI_LIBRARIES})

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)