#--- The exercise name is nothing else than the directory
get_filename_component(EXERCISENAME ${CMAKE_CURRENT_LIST_DIR} NAME)
add_executable(${EXERCISENAME} cube.h main.cpp _quad/Quad.h)
target_link_libraries(${EXERCISENAME} ${COMMON_LIBS})
target_deploy_file(${EXERCISENAME} cube_vshader.glsl)
target_deploy_file(${EXERCISENAME} cube_fshader.glsl)
target_deploy_file(${EXERCISENAME} _quad/quad_vshader.glsl)
target_deploy_file(${EXERCISENAME} _quad/quad_fshader.glsl)
target_deploy_file(${EXERCISENAME} _quad/quad_texture.tga)
