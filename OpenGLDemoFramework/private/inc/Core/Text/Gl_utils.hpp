#ifndef __Gl_utils_h_
#define __Gl_utils_h_

#include <GL/glew.h>
#include <vector>
#include <Core/Shader.hpp>

namespace gl_utils
{
    class GLOverlayContext {
    public:
        GLOverlayContext() :
        texindex(-1), shindex(-1),    name(-1),
        size0(-1), type0(-1), normalized0(-1), stride0(-1), offset0(-1), enabled0(-1),
        depthEnabled(-1), i0p(0),
        overlayShader(0),
        overlayTex((GLuint) -1),
        overlayUVBuffer((GLuint) -1),
        texOffsetId(-1),
        texSizeId(-1),
        textOffsetId(-1),
        textureId(-1),
        indexId(-1)

        {

        }

        ~GLOverlayContext()
        {
            if (0 != overlayShader)
            {
                glDeleteProgram(overlayShader);
            }

            if ((GLuint) -1 != overlayTex)
            {
                glDeleteTextures(1, &overlayTex);
            }

            if ((GLuint) -1 != overlayUVBuffer)
            {
                glDeleteBuffers(1, &overlayUVBuffer);
            }
        }

        void init(unsigned char * buffer, unsigned int width, unsigned int height)
        {
            overlayShader = loadShaders();

            glGenTextures(1, &overlayTex);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, overlayTex);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, buffer);

            glGenBuffers(1, &overlayUVBuffer);

            const GLfloat textureVertices[] = {
                0.0f, 1.0f,
                1.0f, 1.0f,
                0.0f, 0.0f,
                1.0f, 0.0f
            };

            glBindBuffer(GL_ARRAY_BUFFER, overlayUVBuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, textureVertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 2, GL_FLOAT, 0, 0, 0);
        }

        void storeOriginal()
        {
            glGetIntegerv(GL_DEPTH_TEST, &depthEnabled);

            //Save previous shader and set the current program to the texture drawing shader
            glGetIntegerv(GL_CURRENT_PROGRAM, &shindex);

            //Get the current texture binding. Set the binding to the overlay texture
            glGetIntegerv(GL_TEXTURE_BINDING_2D, &texindex);

            //Save original vertex buffers
            glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &name);
            glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled0);
            glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_SIZE, &size0);
            glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride0);
            glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_TYPE, &type0);
            glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, &normalized0);
            glGetVertexAttribPointerv(0, GL_VERTEX_ATTRIB_ARRAY_POINTER, (void**)&i0p);
        }

        void restoreOriginal()
        {
            //Restore depth test
            if (depthEnabled)
            {
                glEnable(GL_DEPTH_TEST);
            }

            //Restore texture binding. Restore texture data?
            glBindTexture(GL_TEXTURE_2D, texindex);

            //Restore original shader. Restore shader uniforms?
            glUseProgram(shindex);

            //Restore original vertex buffers. Restore buffer data?
            glBindBuffer(GL_ARRAY_BUFFER, name);

            if (enabled0)
            {
                glEnableVertexAttribArray(0);
            }
            glVertexAttribPointer(0, size0, type0, normalized0, stride0, i0p);
        }

        void setToOverlay()
        {
            //Disable depth test
            glDisable(GL_DEPTH_TEST);

            //Enable the overlay texture shader
            glUseProgram(overlayShader);

            //Enable the overlay texture
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, overlayTex);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }

        void drawOverlay(unsigned int totalGlyphs,
                unsigned int index,
                const unsigned int width,
                const unsigned int height,
                const unsigned int x,
                const unsigned int y,
                const float offsetX,
                const float offsetY)
        {
            glUniform1i(indexId, index);
            glUniform2f(texOffsetId, x/256.0f, y/64.0f);
            glUniform2f(texSizeId, width/256.0f, height/64.0f);
            glUniform2f(textOffsetId, offsetX, offsetY);
            glUniform1i(textureId, 0);

            glBindBuffer(GL_ARRAY_BUFFER, overlayUVBuffer);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, 0, 0, 0);

            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            glDisableVertexAttribArray(0);
        }

    private:
        GLint texindex;
        GLint shindex;
        GLint name;
        GLint size0, type0, normalized0, stride0, offset0, enabled0;
        GLint depthEnabled;
        GLint* i0p;
        GLuint overlayShader;
        GLuint overlayTex;
        GLuint overlayUVBuffer;
        GLint texOffsetId;
        GLint texSizeId;
        GLint textOffsetId;
        GLint textureId;
        GLint indexId;

        GLuint loadShaders()
        {

			GLuint programID = LoadShaders("Shaders/overlay.vs", "Shaders/overlay.fs");
            
            texOffsetId = glGetUniformLocation(programID, "texOffset");
            texSizeId = glGetUniformLocation(programID, "texSize");
            textOffsetId   = glGetUniformLocation(programID, "textOffset");
            textureId = glGetUniformLocation(programID, "texture");
            indexId = glGetUniformLocation(programID, "index");

            return programID;
        }
    };
} //END namespace


#endif
