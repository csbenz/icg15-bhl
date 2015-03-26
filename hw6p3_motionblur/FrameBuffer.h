#pragma once
#include "icg_common.h"

#ifdef WITH_OPENCV
    #include "opencv2/core/core.hpp"       ///< cv::Mat
    #include "opencv2/highgui/highgui.hpp" ///< cv::imShow
    #include "opencv2/contrib/contrib.hpp" ///< cvtConvert
    #include "opencv2/imgproc/types_c.h"   ///< CV_BGRA2RGBA
#endif

namespace {
class FullscreenQuad{
protected:
    GLuint _vao; ///< vertex array object
    GLuint _pid; ///< GLSL shader program ID 
    GLuint _vbo; ///< memory buffer
    GLuint _color_tex; ///< Color texture ID
    GLuint _velocity_tex; ///< Velocity texture ID
public:
    void init(GLuint existing_color_texture, GLuint existing_velocity_texture) {
        ///--- Compile the shaders
        _pid = opengp::load_shaders("FrameBuffer_vshader.glsl", "FrameBuffer_fshader.glsl");
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
        if(existing_color_texture== -1)
        {
            glGenTextures(1, &_color_tex); 
            glBindTexture(GL_TEXTURE_2D, _color_tex);
            glfwLoadTexture2D("quad_texture.tga", 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        } else {
            _color_tex = existing_color_texture;
            glBindTexture(GL_TEXTURE_2D, _color_tex);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        GLuint color_tex_id = glGetUniformLocation(_pid, "colorTex");
        glUniform1i(color_tex_id, 0 /*GL_TEXTURE0*/);

        ///--- Assign velocity texture
        _velocity_tex = existing_velocity_texture;
        glBindTexture(GL_TEXTURE_2D, _velocity_tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        GLuint velocity_tex_id = glGetUniformLocation(_pid, "velocityTex");
        glUniform1i(velocity_tex_id, 1 /*GL_TEXTURE1*/);

        ///--- to avoid the current object being polluted
        glBindVertexArray(0);
        glUseProgram(0);
    }
       
    void cleanup(){
        /// TODO cleanup
    }
    
    void draw(){
        glUseProgram(_pid);
        glBindVertexArray(_vao);      
            ///--- Bind textures
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, _color_tex);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, _velocity_tex);
            
            ///--- Draw
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);        
        glUseProgram(0);
    }
};
} ///< anonymous


class FrameBuffer : public FullscreenQuad{
protected:
    bool _init;
    int _width;
    int _height;
    GLuint _fbo;
    GLuint _depth_rb;
    GLuint _color_tex; 
    GLuint _velocity_tex;
    FullscreenQuad _quad; ///< to render the FBO
    
public:
    FrameBuffer(int image_width, int image_height){
        this->_width = image_width;
        this->_height = image_height;        
    }
    
    GLuint color_tex_id() {
        return _color_tex;
    }
    
    void clear(){
        glViewport(0, 0, _width, _height);
        glBindFramebuffer(GL_FRAMEBUFFER, _fbo);        
            glDrawBuffer(GL_COLOR_ATTACHMENT0);
            glClearColor(1.0, 1.0, 1.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glDrawBuffer(GL_COLOR_ATTACHMENT1);
            glClearColor(0.0, 0.0, 0.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);      
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    ///--- Warning: ovverrides viewport!!
    void bind() {
        glViewport(0, 0, _width, _height);
        glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
        const GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        glDrawBuffers(2 /*length of buffers[]*/, buffers);
    }
    
    void unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void init(bool linear_interpolation=false) {        
        ///--- Create color attachment
        {
            glGenTextures(1, &_color_tex);
            glBindTexture(GL_TEXTURE_2D, _color_tex);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            if(linear_interpolation){
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            } else {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            }
            /// See Table.2 https://www.khronos.org/opengles/sdk/docs/man3/docbook4/xhtml/glTexImage2D.xml
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, _width, _height, 0, 
                         GL_RGB, GL_UNSIGNED_BYTE, NULL); ///< how to load from buffer
        }
        
        ///--- Create render buffer (for depth channel)
        {
            glGenRenderbuffers(1, &_depth_rb);
            glBindRenderbuffer(GL_RENDERBUFFER, _depth_rb);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, _width, _height);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
        }

        {
            glGenTextures(1, &_velocity_tex);
            glBindTexture(GL_TEXTURE_2D, _velocity_tex);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            if(linear_interpolation){
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            } else {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            }
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, _width, _height, 0, 
                         GL_RGB, GL_FLOAT, NULL);
        }

        ///--- Tell the quad it will render our texture
        FullscreenQuad::init(_color_tex, _velocity_tex);

        ///--- Tie it all together
        {
            glGenFramebuffers(1, &_fbo);
            glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 /*location = 0*/, GL_TEXTURE_2D, _color_tex, 0 /*level*/);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1 /*location = 1*/, GL_TEXTURE_2D, _velocity_tex, 0 /*level*/);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depth_rb);
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cerr << "!!!ERROR: Framebuffer not OK :(" << std::endl;
            glBindFramebuffer(GL_FRAMEBUFFER, 0); ///< avoid pollution
        }
    }

    void cleanup() {
        glDeleteTextures(1, &_color_tex);
        glDeleteTextures(2, &_velocity_tex);
        glDeleteRenderbuffers(1, &_depth_rb);
        glBindFramebuffer(GL_FRAMEBUFFER, 0 /*UNBIND*/);
        glDeleteFramebuffers(1, &_fbo);
        FullscreenQuad::cleanup();
    }
    
    void draw(){
        glUseProgram(FullscreenQuad::_pid);
        glUniform1f(glGetUniformLocation(FullscreenQuad::_pid, "tex_width"), _width);
        glUniform1f(glGetUniformLocation(FullscreenQuad::_pid, "tex_height"), _height);       
        FullscreenQuad::draw();
    }
    
public:
    void display_velocity_attachment(const char* title) {
#ifdef WITH_OPENCV 
        ///--- Fetch from opengl
        cv::Mat image = cv::Mat(_height, _width, CV_32FC3, cv::Scalar(0));
        glBindTexture(GL_TEXTURE_2D, _velocity_tex);
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, image.data);
        glBindTexture(GL_TEXTURE_2D, 0);
        ///--- Postprocess before showing/saving
        cv::flip(image, image, 0 /*flip rows*/ ); ///< OpenGL / OpenCV top left origin
        // cv::cvtColor(image, image, CV_BGRA2RGBA); ///< OpenCV uses BGRA
        cv::normalize(image, image, 0.0, 1.0, cv::NORM_MINMAX);
        cv::imshow(title, image);
        // cv::waitKey(0); ///< wait for key to be pressed (not necessary with opengl loop)
#else
        std::cout <<"!!!WARNING: sorry, you do not seem to have OpenCV properly configured." << std::endl;
#endif
    } 
};
