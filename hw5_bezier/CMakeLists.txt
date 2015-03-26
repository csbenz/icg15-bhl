get_filename_component(EXERCISENAME ${CMAKE_CURRENT_LIST_DIR} NAME)

file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")
add_executable(${EXERCISENAME} ${SOURCES} ${HEADERS})
target_link_libraries(${EXERCISENAME} ${COMMON_LIBS})

target_deploy_file(${EXERCISENAME} ../external/AntTweakBar/lib/x64/AntTweakBar64.dll) 

target_deploy_file(${EXERCISENAME} _quad/quad_vshader.glsl) 
target_deploy_file(${EXERCISENAME} _quad/quad_fshader.glsl)
target_deploy_file(${EXERCISENAME} _quad/quad_texture.tga)

target_deploy_file(${EXERCISENAME} _cube/cube_vshader.glsl)
target_deploy_file(${EXERCISENAME} _cube/cube_fshader.glsl)

target_deploy_file(${EXERCISENAME} _mesh/phong_vshader.glsl)
target_deploy_file(${EXERCISENAME} _mesh/phong_fshader.glsl)

target_deploy_file(${EXERCISENAME} _point/point_vshader.glsl)
target_deploy_file(${EXERCISENAME} _point/point_fshader.glsl)
target_deploy_file(${EXERCISENAME} _point/point_selection_vshader.glsl)
target_deploy_file(${EXERCISENAME} _point/point_selection_fshader.glsl)
target_deploy_file(${EXERCISENAME} _bezier/bezier_fshader.glsl)
target_deploy_file(${EXERCISENAME} _bezier/bezier_vshader.glsl)

target_deploy_file(${EXERCISENAME} _mesh/tangle_cube.obj)
target_deploy_file(${EXERCISENAME} _mesh/cube.obj)
target_deploy_file(${EXERCISENAME} _mesh/sphere.obj)


