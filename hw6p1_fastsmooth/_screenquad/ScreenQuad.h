#pragma once
#include "icg_common.h"

class ScreenQuad{
protected:
    GLuint _vao; ///< vertex array object
    GLuint _pid; ///< GLSL shader program ID 
    GLuint _vbo; ///< memory buffer

    GLuint _tex; ///< Texture ID
    GLuint _temp_tex; ///< Texture ID

    GLint _mode = 0;
    GLint _SIZE = 0;

    static const GLint _SIZE_MAX = 100;
    GLfloat _kernel[_SIZE_MAX];

    GLint _array_size;
    GLfloat _std = 2.0;
public:
    void init(GLuint scene_texture, GLuint temp_texture){
        
        ///--- Compile the shaders
        _pid = opengp::load_shaders("_screenquad/ScreenQuad_vshader.glsl", "_screenquad/ScreenQuad_fshader.glsl");
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
            glGenBuffers(1, &_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, _vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vpoint), vpoint, GL_STATIC_DRAW);
        
            ///--- Attribute
            GLuint vpoint_id = glGetAttribLocation(_pid, "vpoint");
            glEnableVertexAttribArray(vpoint_id);
            glVertexAttribPointer(vpoint_id, 3, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
        }
        
        ///--- Texture coordinates
        {
            const GLfloat vtexcoord[] = { /*V1*/ 0.0f, 0.0f, 
                                          /*V2*/ 1.0f, 0.0f, 
                                          /*V3*/ 0.0f, 1.0f,
                                          /*V4*/ 1.0f, 1.0f}; 
            
            ///--- Buffer
            glGenBuffers(1, &_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, _vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vtexcoord), vtexcoord, GL_STATIC_DRAW);
        
            ///--- Attribute
            GLuint vtexcoord_id = glGetAttribLocation(_pid, "vtexcoord");
            glEnableVertexAttribArray(vtexcoord_id);
            glVertexAttribPointer(vtexcoord_id, 2, GL_FLOAT, DONT_NORMALIZE, ZERO_STRIDE, ZERO_BUFFER_OFFSET);
        }
        
        ///--- Load/Assign texture
        this->_tex = scene_texture;
        glBindTexture(GL_TEXTURE_2D, _tex);
        GLuint tex_id = glGetUniformLocation(_pid, "tex");
        glUniform1i(tex_id, 0 /*GL_TEXTURE0*/);
        
        this->_temp_tex = temp_texture;
        glBindTexture(GL_TEXTURE_2D, _temp_tex);
        GLuint temp_tex_id = glGetUniformLocation(_pid, "temp_tex");
        glUniform1i(temp_tex_id, 1 /*GL_TEXTURE1*/);
    
        setKernel();
        
        ///--- to avoid the current object being polluted
        glBindVertexArray(0);
        glUseProgram(0);
    }
    
    void changeMode(GLuint mode){
        this->_mode = mode;
    }
    
    void incStd(){
        // Changing std by 0.05 => changins variance by 0.25
        _std += 0.05;
        std::cout << "std = " << _std << std::endl << std::flush;

        setKernel();
    }
    
    void decStd(){

        if(_std - 0.05 > 0.0){
            _std -= 0.05;
        }
        else{
            _std = 0.001;
        }
        std::cout << "std = " << _std << std::endl << std::flush;

        setKernel();
    }
    
    void setKernel(){
        ///--- Kernel
        GLfloat tot_weights = 0.0;
        
        _SIZE = 2 * 3 * ceil(_std);
        _array_size = 1+2*_SIZE; // Center + left + right
        // We limit the size of total array to SIZE_MAX
        if(_array_size > _SIZE_MAX){
            _SIZE = (_SIZE_MAX-1)/2;
            _array_size = _SIZE_MAX;
        }

        for(int i=-_SIZE; i<=_SIZE; i++){
            float weight = exp(-(i*i)/(2.0*pow(_std, 4)));
            tot_weights += weight;
            _kernel[i+_SIZE] = weight;
        }
        for(int i=0;i<_array_size;i++){
            _kernel[i] /= tot_weights;
        }
    }
       
    void cleanup(){
        // TODO cleanup
    }
    
    void draw(int times){
        glUseProgram(_pid);
        glBindVertexArray(_vao);
            glUniform1i(glGetUniformLocation(_pid, "times"), times);
            glUniform1f(glGetUniformLocation(_pid, "tex_width"), _width);
            glUniform1f(glGetUniformLocation(_pid, "tex_height"), _height);
            glUniform1i(glGetUniformLocation(_pid, "mode"), _mode);
            glUniform1fv(glGetUniformLocation(_pid, "kernel"), _array_size, _kernel);
            glUniform1i(glGetUniformLocation(_pid, "SIZE"), _SIZE);
            glUniform1f(glGetUniformLocation(_pid, "std"), _std);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, _tex);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, _temp_tex);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);        
        glBindVertexArray(0);        
        glUseProgram(0);
    }
};
