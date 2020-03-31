# add_definitions()
set(EXTRA_PARTICLES_LIBS
    KtCore KtGui KtDeclarative
)

set(PARTICLES_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/qdeclarativeparticles.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/particles.cpp
)

set(PARTICLES_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/qdeclarativeparticles_p.h
)

katie_setup_target(qmlparticlesplugin ${PARTICLES_SOURCES} ${PARTICLES_HEADERS})

add_library(qmlparticlesplugin MODULE ${qmlparticlesplugin_SOURCES})
target_link_libraries(qmlparticlesplugin ${EXTRA_PARTICLES_LIBS})

install(
    TARGETS qmlparticlesplugin
    DESTINATION ${KATIE_IMPORTS_PATH}/Qt/labs/particles
    COMPONENT Runtime
)
install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/qmldir
    DESTINATION ${KATIE_IMPORTS_PATH}/Qt/labs/particles
    COMPONENT Runtime
)
