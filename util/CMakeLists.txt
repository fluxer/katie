# FIXME: most utils are not namespaces aware
add_definitions(-DQT_NAMESPACE_COMPAT)

add_subdirectory(fixnonlatin1)
add_subdirectory(gencmap)
add_subdirectory(lexgen)
add_subdirectory(normalize)
add_subdirectory(plugintest)
add_subdirectory(qlalr)
