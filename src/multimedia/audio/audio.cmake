set(MULTIMEDIA_HEADERS
    ${MULTIMEDIA_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/audio/qaudio.h
    ${CMAKE_CURRENT_SOURCE_DIR}/audio/qaudioformat.h
    ${CMAKE_CURRENT_SOURCE_DIR}/audio/qaudioinput.h
    ${CMAKE_CURRENT_SOURCE_DIR}/audio/qaudiooutput.h
    ${CMAKE_CURRENT_SOURCE_DIR}/audio/qaudiodeviceinfo.h
    ${CMAKE_CURRENT_SOURCE_DIR}/audio/qaudioengineplugin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/audio/qaudioengine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/audio/qaudiodevicefactory_p.h
)

set(MULTIMEDIA_SOURCES
    ${MULTIMEDIA_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/audio/qaudio.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/audio/qaudioformat.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/audio/qaudiodeviceinfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/audio/qaudiooutput.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/audio/qaudioinput.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/audio/qaudioengineplugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/audio/qaudioengine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/audio/qaudiodevicefactory.cpp
)

if(WITH_ALSA AND ALSA_FOUND)
    set(MULTIMEDIA_HEADERS
        ${MULTIMEDIA_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/audio/qaudiooutput_alsa_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/audio/qaudioinput_alsa_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/audio/qaudiodeviceinfo_alsa_p.h
    )
    set(MULTIMEDIA_SOURCES
        ${MULTIMEDIA_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/audio/qaudiodeviceinfo_alsa_p.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/audio/qaudiooutput_alsa_p.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/audio/qaudioinput_alsa_p.cpp
    )
    set(EXTRA_MULTIMEDIA_LIBS
        ${EXTRA_MULTIMEDIA_LIBS}
        ${ALSA_LIBRARIES}
    )
    include_directories(${ALSA_INCLUDE_DIRS})
    add_definitions(-DHAS_ALSA)
endif()

