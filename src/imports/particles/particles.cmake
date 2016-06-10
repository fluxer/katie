# add_definitions()
set(EXTRA_PARTICLES_LIBS KtDeclarative)

set(PARTICLES_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/particles/qdeclarativeparticles.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/particles/particles.cpp
)

set(PARTICLES_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/particles/qdeclarativeparticles_p.h
)

katie_resources("${PARTICLES_SOURCES}")
katie_resources("${PARTICLES_HEADERS}")
katie_setup_flags()

add_library(qmlparticlesplugin ${KATIE_TYPE} ${PARTICLES_SOURCES} ${PARTICLES_HEADERS})
target_link_libraries(qmlparticlesplugin ${EXTRA_PARTICLES_LIBS})

install(
    TARGETS qmlparticlesplugin
    DESTINATION ${QT_IMPORTS_PATH}/Qt/labs/particles
)
install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/particles/qmldir
    DESTINATION ${QT_IMPORTS_PATH}/Qt/labs/particles
)
