#pragma once

#include "GL/glut.h"
#include "DsShowGrabber/DsShowGrabber.h"

// Constants
#define INIT_OK             0x0000
#define BUFFER_FAILED       0x0001
#define BUFFER_FAILED_CAM   0x0002
#define INIT_FAILED         0x0003
#define INIT_FAILED_CAM     0x0004
#define ONE_SHOT_FAILED     0x0005


int width, height, colorDepth;
GLuint texture;
DsShowGrabber *grabber;
unsigned int size;
unsigned char *data;


bool checkForError(int code)
{
    switch(code)
    {
        case INIT_OK: 
            return true;

        case BUFFER_FAILED:
            printf("Getting video buffer failed or video is at it's end\n");
            return false;

        case BUFFER_FAILED_CAM:
            printf("Getting video buffer failed, trying again in 1s.\n");
            return false;
                
        case INIT_FAILED:
            printf("Grabber initialization failed! Video file not available?\n");
            return false;
        
        case INIT_FAILED_CAM:
            printf("Grabber initialization failed! No video capture device?\n");
            return false;
        
        case ONE_SHOT_FAILED:
            printf("Setting one shot mode failed\n");
            return false;
        default:
            printf("Unspecified error!\n");
            return false;
    }
}        


inline void drawQuad(int width, int height)
{
    glBegin(GL_QUADS);
    {
        glTexCoord2i(    0,      0); glVertex2i(    0,      0);
        glTexCoord2i(width,      0); glVertex2i(width,      0);
        glTexCoord2i(width, height); glVertex2i(width, height);
        glTexCoord2i(    0, height); glVertex2i(    0, height);
    }
    glEnd();
}

void createTexture()
{
    GLenum type;
    switch(colorDepth) {
        case 8:  type = GL_LUMINANCE; break;
        case 24: type = GL_BGR; break;
        case 32: type = GL_BGRA; break;
    }
    glGenTextures(1, &texture);  
    glEnable(GL_TEXTURE_RECTANGLE_ARB);
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB, texture);
    glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGB, width, height, 0, type, GL_UNSIGNED_BYTE, data); 
    glDisable(GL_TEXTURE_RECTANGLE_ARB);
}