#pragma once

#pragma once
#include "icg_common.h"

class Quad{
private:
    GLuint _vao; ///< vertex array object
    GLuint _pid; ///< GLSL shader program ID 
    GLuint _vbo_vpoint; ///< memory buffer
    int window_width = 0;
    int window_height = 0;
    
public:
    
    struct Parameters{
        vec3 light_dir = vec3(0.0, 0.2, -.98).normalized();
        float K_mie = 1e-4;
        vec3 K_ray = 0.003 * vec3(1.0/0.15, 1.0/0.10, 1/0.05);
        float I_sun = 30;
        float H_0 = 0.2; /*average density found at 20% height*/
        float inner_radius = 1000; /*6371 Km*/
        float outer_radius = 1500; /*6371 + 96Km*/
        float camera_height = 2; /*TODO: we are 1Km up in this case!?!? */
        float cam_angle = 0.3; /*shows terrain minimally*/
        unsigned int nb_samples = 5;
    };
    
public:
    void init(int window_width, int window_height){
        this->window_height = window_height;
        this->window_width = window_width;
        
        ///--- Compile the shaders
        _pid = opengp::load_shaders("quad_vshader.glsl", "quad_fshader.glsl");
        if(!_pid) exit(EXIT_FAILURE);       
        glUseProgram(_pid);
        
        ///--- Vertex one vertex Array
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
     
        ///--- Vertex coordinates
        {
            const GLfloat vpoint[] = { /*V1*/ -1.0f, -1.0f, 0.0f, 
                                       /*V2*/ +1.0f, -1.0f, 0.0f, 
                                       /*V3*/ -1.0f, +1.0f, 0.0f,
                                       /*V4*/ +1.0f, +1.0f, 0.0f };        
            ///--- Buffer
            glGenBuffers(1, &_vbo_vpoint);
            glBindBuffer(GL_ARRAY_BUFFER, _vbo_vpoint);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vpoint), vpoint, GL_STATIC_DRAW);
        
            ///--- Attribute
            GLuint vpoint_id = glGetAttribLocation(_pid, "vpoint");
            glEnableVertexAttribArray(vpoint_id);
            glVertexAttribPointer(vpoint_id, 3, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
        }

        ///--- to avoid the current object being polluted
        glBindVertexArray(0);
        glUseProgram(0);
    }
       
    void cleanup(){
        glBindVertexArray(0);
        glUseProgram(0);
        glDeleteBuffers(1, &_vbo_vpoint);
        glDeleteProgram(_pid);
        glDeleteVertexArrays(1, &_vao);
    }
    
    void draw(const mat4& camera_matrix, const Parameters& pars){
        /// Compute inverse of camera matrix
        mat4 camera_matrix_inverse = camera_matrix.inverse();
        
        glUseProgram(_pid);
        glBindVertexArray(_vao);       
            ///--- Load in parameters 
            glUniformMatrix4fv(glGetUniformLocation (_pid, "camera_matrix_inverse"), 1, GL_FALSE, camera_matrix_inverse.data());
            glUniform3fv(glGetUniformLocation (_pid, "light_dir"), 1, pars.light_dir.data());
            glUniform1f(glGetUniformLocation (_pid, "camera_height"), pars.camera_height);
            glUniform1i(glGetUniformLocation (_pid, "nb_samples"), pars.nb_samples);
            glUniform1f(glGetUniformLocation (_pid, "I_sun"), pars.I_sun);
            glUniform3fv(glGetUniformLocation (_pid, "K_ray"), 1, pars.K_ray.data());
            glUniform1f(glGetUniformLocation (_pid, "K_mie"), pars.K_mie);           
            glUniform1f(glGetUniformLocation (_pid, "H_0"), pars.H_0);
            glUniform1f(glGetUniformLocation (_pid, "inner_radius"), pars.inner_radius);
            glUniform1f(glGetUniformLocation (_pid, "outer_radius"), pars.outer_radius);
            glUniform1f(glGetUniformLocation (_pid, "window_width"), window_width);
            glUniform1f(glGetUniformLocation (_pid, "window_height"), window_height);
            ///--- Draw
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);        
        glUseProgram(0);
    }
};
