#pragma once
#include "icg_common.h"

#ifdef WITH_OPENCV
    #include "opencv2/core/core.hpp"       ///< cv::Mat
    #include "opencv2/highgui/highgui.hpp" ///< cv::imShow
    #include "opencv2/contrib/contrib.hpp" ///< cvtConvert
    #include "opencv2/imgproc/types_c.h"   ///< CV_BGRA2RGBA
#endif

class ShadowBuffer{
protected:
    bool _init;
    int _width;
    int _height;
    GLuint _fbo;
    GLuint _depth_tex;
    GLint _previous_viewport[4];
    
public:
    ShadowBuffer(int image_width = 512, int image_height = 512){
        this->_width = image_width;
        this->_height = image_height;        
    }
        
    ///--- Warning: ovverrides viewport!!
    void bind() {      
        glGetIntegerv(GL_VIEWPORT, _previous_viewport);  // Store the previous viewport
        glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
        glViewport(0, 0, _width, _height);
    }
    
    void unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(_previous_viewport[0], _previous_viewport[1], _previous_viewport[2], _previous_viewport[3]);  // Reset the viewport
    }

    // TODO: Analyze this function to understand how to create an FBO
    //       with only a depth attachment and without writing to any
    //       color buffer.
    int init() {
        ///--- Create color attachment
        {
            glActiveTexture(GL_TEXTURE1);
            glGenTextures(1, &_depth_tex);
            glBindTexture(GL_TEXTURE_2D, _depth_tex);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }
    
        ///--- Tie it all together
        {
            glGenFramebuffers(1, &_fbo);
            glBindFramebuffer(GL_FRAMEBUFFER, _fbo);           
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _depth_tex, 0);

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cerr << "!!!ERROR: Framebuffer not OK :(" << std::endl;

            // TODO: Note that writing to any color is disabled, only depth is written.
            glDrawBuffer(GL_NONE);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
        
        return _depth_tex;
    }

    void cleanup() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0 /*UNBIND*/);
        glDeleteFramebuffers(1, &_fbo);
        glDeleteTextures(1, &_depth_tex);
    }
    
public:
    void display() {
#ifdef WITH_OPENCV 
        ///--- Fetch from opengl
        glActiveTexture(GL_TEXTURE1);
        cv::Mat image = cv::Mat(_height, _width, CV_32FC1, cv::Scalar(0));
        glBindTexture(GL_TEXTURE_2D, _depth_tex);
        glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, image.data);
        glBindTexture(GL_TEXTURE_2D, 0);
        ///--- Postprocess before showing/saving
        cv::flip(image, image, 0 /*flip rows*/ ); ///< OpenGL / OpenCV top left origin
        cv::imshow("lightdepth", image);
#else
        std::cout <<"!!!WARNING: sorry, you do not seem to have OpenCV properly configured." << std::endl;
#endif
    }
    void unshow() {  // :-)
#ifdef WITH_OPENCV
        cvDestroyWindow("lightdepth");
#else
        std::cout <<"!!!WARNING: sorry, you do not seem to have OpenCV properly configured." << std::endl;
#endif
    }
};
