/*
 * TextRenderer.hpp
 *
 *  Created on: Dec 13, 2014
 *      Author: jci
 */

#ifndef TEXTRENDERER_HPP_
#define TEXTRENDERER_HPP_

#include <Core/Text/glyphs.hpp>
#include <Core/Text/Gl_utils.hpp>

class TextRenderer
{
public:
    TextRenderer() : isInit(false) {}
    ~TextRenderer() {}
    void render(std::string text, float offsetX, float offsetY)
    {
        //SAVE STATE
        glContext.storeOriginal();
        glContext.setToOverlay();
        for (unsigned int n = 0; n < text.length(); n++)
        {
            GlyphParams* glyph = glyphs.getGlyph(text[n]);
            drawGlyph(text.length(), n, glyph, offsetX, offsetY);
        }

        //RESTORE STATE
        glContext.restoreOriginal();
    }

    void init()
    {
        if (false == isInit)
        {
            glContext.init(glyphs.getBitmapBuff(), 256, 64);
            isInit = true;
        }
    }

private:
    gl_utils::GLOverlayContext glContext;
    Glyphs glyphs;
    bool isInit;

    void drawGlyph(unsigned int totalGlyphs, unsigned int index, GlyphParams* glyph, float offsetX, float offsetY)
    {
        glContext.drawOverlay(totalGlyphs,
            index,
            glyph->width,
            glyph->height,
            glyph->x,
            glyph->y,
            offsetX,
            offsetY);
    }
};


#endif /* TEXTRENDERER_HPP_ */
