#pragma once
#include "icg_common.h"

class Cube {
private:
    GLuint _vao;          ///< vertex array object
    GLuint _vbo_position; ///< memory buffer for positions
    GLuint _vbo_index;    ///< memory buffer for indices
    GLuint _pid;          ///< GLSL shader program ID
    GLuint _num_indices;
public:
    GLuint program_id() const{
        return _pid;
    }

    void init(){
        // Compile the shaders.
        _pid = opengp::load_shaders("cube_vshader.glsl", "cube_fshader.glsl");
        if(!_pid)
          exit(EXIT_FAILURE);
        glUseProgram(_pid);

        // Vertex one vertex array
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);

        // Position buffer
        const GLfloat position[] = {-1.0f, -1.0f,  1.0f,  // left, bottom, front
                                     1.0f, -1.0f,  1.0f,  // right, bottom, front
                                     1.0f,  1.0f,  1.0f,  // right, top, front
                                    -1.0f,  1.0f,  1.0f,  // left, top, front
                                    -1.0f, -1.0f, -1.0f,  // left, bottom, back
                                     1.0f, -1.0f, -1.0f,  // right, bottom, back
                                     1.0f,  1.0f, -1.0f,  // right, top, back
                                    -1.0f,  1.0f, -1.0f}; // left, top, back*/

        glGenBuffers(1, &_vbo_position);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo_position);
        glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);

        // position shader attribute
        GLuint loc_position = glGetAttribLocation(_pid, "position"); ///< Fetch attribute ID for vertex positions
        glEnableVertexAttribArray(loc_position); /// Enable it
        glVertexAttribPointer(loc_position, 3, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);

        // Index buffer
        // TODO 4: Fill in the indices of all the remaining triangles
        const GLuint index[] = {0, 1, 2,  // Front face triangle 1
                                0, 2, 3}; // Front face triangle 2

        _num_indices = sizeof(index) / sizeof(GLuint);

        glGenBuffers(1, &_vbo_index);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo_index);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
    
        // to avoid the current object being polluted
        glBindVertexArray(0);
    }
       
    void cleanup(){     
        glDeleteBuffers(1, &_vbo_position);
        glDeleteBuffers(1, &_vbo_index);
        glDeleteVertexArrays(1, &_vao);
    }

    void draw(const mat4& model, const mat4& view, const mat4& projection){
        glUseProgram(_pid);
        glBindVertexArray(_vao);
            // Setup MVP
            mat4 MVP = projection*view*model;
            GLuint MVP_id = glGetUniformLocation(_pid, "mvp");
            glUniformMatrix4fv(MVP_id, 1, GL_FALSE, MVP.data());

            //Draw
            glDrawElements(GL_TRIANGLES, _num_indices, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glUseProgram(0);
    }
};
