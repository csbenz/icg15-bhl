#pragma once
#include "icg_common.h"
#include "../attrib_locations.h"

class Floor{
protected:
    GLuint _vao; ///< vertex array object
    GLuint _tex; ///< Texture ID

public:
    void init(){
        ///--- Vertex one vertex Array
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);

        ///--- Vertex coordinates
        {
            const GLfloat vpoint[] = { /*V1*/ -1.0f,  0.0f,  1.0f,
                                       /*V2*/  1.0f,  0.0f,  1.0f,
                                       /*V3*/ -1.0f,  0.0f, -1.0f,
                                       /*V4*/  1.0f,  0.0f, -1.0f };
            ///--- Buffer
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vpoint), vpoint, GL_STATIC_DRAW);

            ///--- Attribute
            glEnableVertexAttribArray(ATTRIB_LOC_vpoint);
            glVertexAttribPointer(ATTRIB_LOC_vpoint, 3, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
        }

        ///--- Normal coordinates
        {
            const GLfloat vnormal[] = { 0.0f, +1.0f, 0.0f,
                                        0.0f, +1.0f, 0.0f,
                                        0.0f, +1.0f, 0.0f,
                                        0.0f, +1.0f, 0.0f };
            ///--- Buffer
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vnormal), vnormal, GL_STATIC_DRAW);

            ///--- Attribute
            glEnableVertexAttribArray(ATTRIB_LOC_vnormal);
            glVertexAttribPointer(ATTRIB_LOC_vnormal, 3, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
        }

        ///--- Texture coordinates
        {
            const GLfloat vtexcoord[] = { /*V1*/ 0.0f, 0.0f, 
                                          /*V2*/ 1.0f, 0.0f, 
                                          /*V3*/ 0.0f, 1.0f,
                                          /*V4*/ 1.0f, 1.0f};

            ///--- Buffer
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vtexcoord), vtexcoord, GL_STATIC_DRAW);

            ///--- Attribute
            glEnableVertexAttribArray(ATTRIB_LOC_vtexcoord);
            glVertexAttribPointer(ATTRIB_LOC_vtexcoord, 2, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
        }

        ///--- Load texture
        ///  glActiveTexture(GL_TEXTURE0);
        glGenTextures(1, &_tex);
        glBindTexture(GL_TEXTURE_2D, _tex);
        if (!glfwLoadTexture2D("Floor_texture.tga", 0)) {
          std::cerr << "Couldn't load Floor_texture.tga" << std::endl;
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    void draw(GLuint pid){
        glBindVertexArray(_vao);
        ///--- Bind textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _tex);

        glUniform1i(glGetUniformLocation(pid, "tex"), 0 /*GL_TEXTURE0*/);
        glUniform1i(glGetUniformLocation(pid, "use_color"), 0);
        mat4 model = mat4::Identity();
        glUniformMatrix4fv(glGetUniformLocation(pid, "model"), 1, GL_FALSE, model.data());

        ///--- Draw
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
};
