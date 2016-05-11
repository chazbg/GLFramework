#pragma once

class FrameBuffer
{
public:
    FrameBuffer();
    ~FrameBuffer();
    unsigned int getFbo();
    unsigned int getColorTex();
    unsigned int getDepthTex();
private:
    unsigned int fbo;
    unsigned int colorTex;
    unsigned int depthTex;
};