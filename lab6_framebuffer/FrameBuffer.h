#pragma once
#include "icg_common.h"

#ifdef WITH_OPENCV
    #include "opencv2/core/core.hpp"       ///< cv::Mat
    #include "opencv2/highgui/highgui.hpp" ///< cv::imShow
    #include "opencv2/contrib/contrib.hpp" ///< cvtConvert
    #include "opencv2/imgproc/types_c.h"   ///< CV_BGRA2RGBA
#endif

class FrameBuffer{
protected:
    bool _init;
    int _width;
    int _height;
    GLuint _fbo;
    GLuint _depth_rb;
    GLuint _color_tex;
    
public:
    FrameBuffer(int image_width, int image_height){
        this->_width = image_width;
        this->_height = image_height;        
    }
        
    ///--- Warning: ovverrides viewport!!
    void bind() {
        glViewport(0, 0, _width, _height);
        glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
        const GLenum buffers[] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1 /*length of buffers[]*/, buffers);
    }
    
    void unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    int init(bool use_interpolation = false) {        
        ///--- Create color attachment
        {
            glGenTextures(1, &_color_tex);
            glBindTexture(GL_TEXTURE_2D, _color_tex);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
            if(use_interpolation){
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            } else {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            }
            
            ///--- Create texture for the color attachment
            /// See Table.2 https://www.khronos.org/opengles/sdk/docs/man3/docbook4/xhtml/glTexImage2D.xml
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 
                         _width, _height, 0, 
                         GL_RGB, GL_UNSIGNED_BYTE, NULL); ///< how to load from buffer
        }
        
        ///--- Create render buffer (for depth channel)
        {
#if 0
            glGenRenderbuffersEXT(1, &_depth_rb);
            glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, _depth_rb);
            glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT32, _width, _height);
	    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
#else 
            glGenRenderbuffers(1, &_depth_rb);
            glBindRenderbuffer(GL_RENDERBUFFER, _depth_rb);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, _width, _height);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
#endif
        }
    
        ///--- Tie it all together
        {
            glGenFramebuffers(1, &_fbo);
#if 0
	    glBindFramebuffer(GL_FRAMEBUFFER_EXT, _fbo);
#else
            glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
#endif
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 /*location = 0*/, GL_TEXTURE_2D, _color_tex, 0 /*level*/);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depth_rb);
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cerr << "!!!ERROR: Framebuffer not OK :(" << std::endl;
            glBindFramebuffer(GL_FRAMEBUFFER, 0); ///< avoid pollution
        }
        
        return _color_tex;
    }

    void cleanup() {
        glDeleteTextures(1, &_color_tex);
        glDeleteRenderbuffers(1, &_depth_rb);
#if 0
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0 /*UNBIND*/);
        glDeleteFramebuffersEXT(1, &_fbo);
#else
        glBindFramebuffer(GL_FRAMEBUFFER, 0 /*UNBIND*/);
        glDeleteFramebuffers(1, &_fbo);
#endif
    }
    
public:
    void display_color_attachment(const char* title) {
#ifdef WITH_OPENCV 
        ///--- Fetch from opengl
        cv::Mat image = cv::Mat(_height, _width, CV_8UC3, cv::Scalar(0));
        glBindTexture(GL_TEXTURE_2D, _color_tex);
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data);
        glBindTexture(GL_TEXTURE_2D, 0);
        ///--- Postprocess before showing/saving
        cv::flip(image, image, 0 /*flip rows*/ ); ///< OpenGL / OpenCV top left origin
        cv::cvtColor(image, image, CV_BGRA2RGBA); ///< OpenCV uses BGRA
        cv::imshow(title, image);
        // cv::waitKey(0); ///< wait for key to be pressed (not necessary with opengl loop)
#else
        std::cout <<"!!!WARNING: sorry, you do not seem to have OpenCV properly configured." << std::endl;
#endif
    }
};
