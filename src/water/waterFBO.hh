#pragma once

#include <GL/glew.h>
#include <GL/glut.h>

class waterFBO
{
    public:
        waterFBO();
        int createFrameBuffer();
        int createTextureAttachment(int width, int height);
        int createDepthTextureAttachment(int width, int height);
        int createDepthBufferAttachment(int width, int height);
};

waterFBO::waterFBO()
{
    //initialiseReflectionFrameBuffer();
	//initialiseRefractionFrameBuffer();
}