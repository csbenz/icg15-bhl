get_filename_component(EXERCISENAME ${CMAKE_CURRENT_LIST_DIR} NAME)
add_executable(${EXERCISENAME} main.cpp Triangle.h)
target_link_libraries(${EXERCISENAME} ${COMMON_LIBS})
target_deploy_file(${EXERCISENAME} triangle_vshader.glsl) 
target_deploy_file(${EXERCISENAME} triangle_fshader.glsl)
