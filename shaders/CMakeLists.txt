find_package(Vulkan REQUIRED)

file(GLOB_RECURSE GLSL_FRAG_FILES "*.glsl")

#message(Vulkan_glslc_FOUND ${Vulkan_glslc_FOUND})
#message(Vulkan_GLSLC_EXECUTABLE ${Vulkan_GLSLC_EXECUTABLE})

foreach(GLSL ${GLSL_FRAG_FILES})

    get_filename_component(FILE_NAME ${GLSL} NAME_WE)
    set(SPIRV "${FILE_NAME}.spv")
    message(STATUS ${GLSL})

    if(${FILE_NAME} MATCHES frag$)
        message(STATUS "BUILDING FRAGMENT SHADER" ${FILE_NAME})
        add_custom_command(
                OUTPUT ${SPIRV}
                COMMAND ${Vulkan_GLSLC_EXECUTABLE} -fshader-stage=fragment ${GLSL} -o ${SPIRV}
                DEPENDS ${GLSL})
    elseif(${FILE_NAME} MATCHES geom$)
        message(STATUS "BUILDING GEOMETRY SHADER " ${FILE_NAME})
        add_custom_command(
                OUTPUT ${SPIRV}
                COMMAND ${Vulkan_GLSLC_EXECUTABLE} -fshader-stage=geometry ${GLSL} -o ${SPIRV}
                DEPENDS ${GLSL})

    elseif(${FILE_NAME} MATCHES vert$)
        message(STATUS "BUILDING VERTEX SHADER " ${FILE_NAME})
        add_custom_command(
                OUTPUT ${SPIRV}
                COMMAND ${Vulkan_GLSLC_EXECUTABLE} -fshader-stage=vertex ${GLSL} -o ${SPIRV}
                DEPENDS ${GLSL})

    elseif(${FILE_NAME} MATCHES comp$)
        message(STATUS "BUILDING COMPUTE SHADER " ${FILE_NAME})
        add_custom_command(
                OUTPUT ${SPIRV}
                COMMAND ${Vulkan_GLSLC_EXECUTABLE} -fshader-stage=compute ${GLSL} -o ${SPIRV}
                DEPENDS ${GLSL})
    endif()


    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
        Shaders
        DEPENDS ${SPIRV_BINARY_FILES}
        #COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADER_OUTPUT_DIR}
        #COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SPIRV_BINARY_FILES} ${SHADER_OUTPUT_DIR}
)

add_dependencies(IFS Shaders)