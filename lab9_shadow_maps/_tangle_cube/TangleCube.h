#pragma once
#include "icg_common.h"
#include "../attrib_locations.h"

bool rotate_cube = false;

class TangleCube{
protected:
    GLuint _vao; ///< vertex array object
    vec3 _color; ///< Mesh Color
    opengp::Surface_mesh _mesh;

public:
    void init(){
        _color = vec3(1.0, 0.0, 0.0);

        ///--- Vertex one vertex Array
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);

        _mesh.read("tangle_cube.obj");
        //_mesh.triangulate();
        _mesh.update_vertex_normals();

        ///--- Vertex coordinates
        {
            Surface_mesh::Vertex_property<Point> vpoint = _mesh.get_vertex_property<Point>("v:point");
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, _mesh.n_vertices() * sizeof(vec3), vpoint.data(), GL_STATIC_DRAW);

            ///--- Attribute
            glEnableVertexAttribArray(ATTRIB_LOC_vpoint);
            glVertexAttribPointer(ATTRIB_LOC_vpoint, 3, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
        }

        ///--- Normal coordinates
        {
            Surface_mesh::Vertex_property<Normal> vnormal = _mesh.get_vertex_property<Normal>("v:normal");
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, _mesh.n_vertices() * sizeof(vec3), vnormal.data(), GL_STATIC_DRAW);

            ///--- Attribute
            glEnableVertexAttribArray(ATTRIB_LOC_vnormal);
            glVertexAttribPointer(ATTRIB_LOC_vnormal, 3, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
        }


        ///--- Index Buffer
        {
            std::vector<unsigned int> indices;
            for(Surface_mesh::Face_iterator fit = _mesh.faces_begin(); fit != _mesh.faces_end(); ++fit) {
                unsigned int n = _mesh.valence(*fit);
                Surface_mesh::Vertex_around_face_circulator vit = _mesh.vertices(*fit);
                for(unsigned int v = 0; v < n; ++v) {
                    indices.push_back((*vit).idx());
                    ++vit;
                }
            }

            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        }
    }

    void draw(GLuint pid){
        glBindVertexArray(_vao);

        glUniform3fv(glGetUniformLocation(pid, "mesh_color"), 1, _color.data());
        glUniform1i(glGetUniformLocation(pid, "use_color"), 1);

        float scale = 0.65;
        typedef Eigen::Transform<float,3,Eigen::Affine> Transform;
        Transform M = Transform::Identity();
        M *= Eigen::Translation3f(0,0.4,0);
        static float cube_rotation = 0.0;
        static float old_time = glfwGetTime();
        if (rotate_cube) {
          cube_rotation += glfwGetTime() - old_time;
        }
        old_time = glfwGetTime();
        M *= Eigen::AngleAxisf(cube_rotation, Eigen::Vector3f::UnitY());
        M *= Eigen::AlignedScaling3f(scale, scale, scale);
        mat4 model = M.matrix();
        glUniformMatrix4fv(glGetUniformLocation(pid, "model"), 1, GL_FALSE, model.data());

        ///--- Draw
        glDrawElements(GL_TRIANGLES, 3 * _mesh.n_faces(), GL_UNSIGNED_INT, ZERO_BUFFER_OFFSET);
    }
};
