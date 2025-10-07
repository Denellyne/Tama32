idf_component_register(SRCS "Tama32.c"
                    INCLUDE_DIRS ".")

add_custom_target(dummyTarget ALL
    DEPENDS
    Tools/PEmbed
    )

    FILE(GLOB_RECURSE var "*.png")
# custom_output will always be rebuilt because it depends on always_rebuild
add_custom_command(
    OUTPUT Tools/PEmbed
    COMMAND make build
    COMMAND ../bin/main ../../../Sprites/spritesData.h ${var}
    COMMAND echo "PNGs converted"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/main/Tools/PEmbed/src
    DEPENDS
        alwaysRebuild
    )

# Dummy output which is never actually produced. Anything that depends on
# this will always be rebuilt.
add_custom_command(
    OUTPUT alwaysRebuild
    COMMAND echo "Converting PNG to XBM"
    )
