/**
 * @file main.cpp
 * @brief Main class. 
 * 
 * Captures the DirectShow input from either a video file or a camera device.
 * 
 * @author Florian Muecke, University of Augsburg
 * @version Revision 7
 * @date 24.03.2007
 * @created 23.03.2007
 **/

// includes
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include <DsShowGrabber/DsShowGrabber.h>
#include "DSHelper.h"

const char *filename;

//=============================================================================
/// CLEANUP: cleanup unneded data
//-----------------------------------------------------------------------------
void cleanup(void)
{
    //grabber->stop();
    delete grabber;
    grabber = 0;
}


//=============================================================================
/// Update input image
//-----------------------------------------------------------------------------
void updateImage()
{
#ifdef CAMERA
    // grab new image
    while(!grabber->grabFrame(data, size)) { Sleep(10); }
    //if (!grabber->grabFrame(data, size)) {
        //checkForError(BUFFER_FAILED_CAM);
		//Sleep(1000);
        //return;
    //}
#else
	// get next video image, waiting endlessly
    if (!grabber->getBuffer(data, size, INFINITE))
    {
        checkForError(BUFFER_FAILED);
        system("pause");
        exit(1);
    }
#endif

    glBindTexture(GL_TEXTURE_RECTANGLE_ARB, texture);
    glTexSubImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, 0, 0, width, height, GL_BGR, 
                    GL_UNSIGNED_BYTE, data);
    glutPostRedisplay();
}


//=============================================================================
/// KEYBOARD: the keyboard callback proc
//-----------------------------------------------------------------------------
void keyboard (unsigned char key, int x, int y)
{ 
    if (key == 27 || key == 'q') exit(0);
    glutPostRedisplay();
}


//=============================================================================
/// DISPLAY: the OGL display function
//-----------------------------------------------------------------------------
void display(void)
{
    glEnable(GL_TEXTURE_RECTANGLE_ARB);
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB, texture);
    
    drawQuad(width, height);
    
    glDisable(GL_TEXTURE_RECTANGLE_ARB);
    glutSwapBuffers();
    //glutReshapeWindow(width, height); //this way the window size stays !
}


//=============================================================================
/// INITIALIZE: init needed environments
//-----------------------------------------------------------------------------
bool initialize()
{
    // Print help text
    printf("\nKEYS:\n"
           "q - exit program\n\n");

    // Create new grabber object instance
    grabber = new DsShowGrabber();

#ifdef CAMERA
    //---
    //@device:pnp:\\?\pci#ven_109e&dev_036e&subsys_3000144f&rev_02#4&3b8be48b&0&3830#{65e8773d-8f56-11d0-a3b9-00a0c9223196}\global
    //@device:pnp:\\?\usb#vid_041e&pid_4055#5&1df089d4&0&1#{65e8773d-8f56-11d0-a3b9-00a0c9223196}\{9b365890-165f-11d0-a195-0020afd156e4}
    //---
	// try to initialize the grabber by choosing the first available video device
    if (!grabber->init(NULL, 
        "#vid_041e&pid_4055#5&1df089d4&0&1#",   //USB Webcam devicename
        640, 480, 24))
    {
        delete grabber;
        return INIT_FAILED_CAM;
    }
    // capture images by the grabber object itself
    if (grabber->SetCallback(grabber,0)!=S_OK)
    {
        delete grabber;
        return INIT_FAILED_CAM;
    }
#else
    // try to initialize the grabber, loading a test video without looping
    if (!grabber->init(filename, true))  
    {
        delete grabber;
        return INIT_FAILED;
    }
	// if no frames should be dropped, you are not allowed to set a callback
    // and have to set one shot mode to true
    //if (!grabber->setOneShot(true))
    //{
    //    printf("Setting one shot mode failed\n");
    //    delete grabber;
    //    return ONE_SHOT_FAILED;
    //}
#endif	

    colorDepth  = grabber->getDepth();
    width       = grabber->getWidth();
    height      = grabber->getHeight();
    size        = grabber->getBufferSize();
    data        = new unsigned char[size];
    
    grabber->start();

    Sleep(1000);
#ifdef CAMERA
	// grab new image
	if (!grabber->grabFrame(data, size)) {
		Sleep(1000);
        //return BUFFER_FAILED_CAM;
    }
#else
	// get next video image, waiting endlessly
    if (!grabber->getBuffer(data, size, INFINITE))
    {
        return BUFFER_FAILED;
    }
#endif
    
    createTexture();

    return INIT_OK;
}


//=============================================================================
/// RESHAPE
//-----------------------------------------------------------------------------
void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);    
    glLoadIdentity();               
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);     
    glLoadIdentity();               
}


//=============================================================================
/// MAIN: that's where the music is played...
//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    filename = TEXT("..\\media\\sequenz1.avi");
   
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(300, 100);
    //glutInitWindowSize(width, height);
    glutCreateWindow("DirectShow Grabber");
    
    if(!checkForError(initialize())) { system("PAUSE"); exit(1); }
    
    glutReshapeWindow(width, height);

    glutIdleFunc(updateImage);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutMainLoop();

    cleanup();
 
    return 0;
}