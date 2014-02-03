/*****************************************************
*
* Stellaris Launchpad Example Project
*
* Copyright (c) 2012 theJPster (www.thejpster.org.uk)
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*
*****************************************************/

/**************************************************
* Includes
***************************************************/

#include "util/util.h"

#include "drivers/lcd/lcd.h"

/**************************************************
* Defines
***************************************************/

#define GLYPH_WIDTH_INDEX       0
#define GLYPH_HEIGHT_INDEX      1
#define GLYPH_OFFSET_INDEX      2
#define GLYPH_NUM_GLYPHS_INDEX  3
#define GLYPH_START_INDEX       4

/**************************************************
* Data Types
**************************************************/

/* None */

/**************************************************
* Function Prototypes
**************************************************/

/* None */

/**************************************************
* Public Data
**************************************************/

extern const unsigned char SevenSeg_XXXL_Num[];
extern const unsigned char BigFont[];

/**************************************************
* Private Data
**************************************************/

#define FONT_FACE_LARGE_WIDTH 64
#define FONT_FACE_LARGE_HEIGHT 120

/**************************************************
* Public Functions
***************************************************/

void font_draw_number_large(
    lcd_row_t x, lcd_col_t y,
    uint16_t number,
    unsigned int pad_width,
    lcd_colour_t fg,
    lcd_colour_t bg
)
{
    char buffer[6];
    char *p;
    int len;
    unsigned int glyph_width = SevenSeg_XXXL_Num[GLYPH_WIDTH_INDEX];
    unsigned int glyph_height = SevenSeg_XXXL_Num[GLYPH_HEIGHT_INDEX];
    unsigned int glyph_size = 1 + ((glyph_width/8) * glyph_height);
    len = sprintf(buffer, "%u", number);
    while(pad_width > len)
    {
        lcd_paint_fill_rectangle(bg, x, x+glyph_width-1, y, y+glyph_height-1);
        x+=glyph_width;
        pad_width--;
    }
    p = buffer;
    while (len)
    {
        unsigned int glyph_num = *p - SevenSeg_XXXL_Num[GLYPH_OFFSET_INDEX];
        if (glyph_num < SevenSeg_XXXL_Num[GLYPH_NUM_GLYPHS_INDEX])
        {        
            const uint8_t *p_glyph = &SevenSeg_XXXL_Num[GLYPH_START_INDEX + (glyph_size * glyph_num)];
            lcd_paint_mono_rectangle(fg, bg, x, x+glyph_width-1, y, y+glyph_height-1, &p_glyph[1]);
        }
        p++;
        len--;
        x+=glyph_width;
    }
}

void font_draw_text_small(
    lcd_row_t x, lcd_col_t y,
    const char *p_message,
    lcd_colour_t fg,
    lcd_colour_t bg,
    bool monospace
)
{
    unsigned int glyph_width = BigFont[GLYPH_WIDTH_INDEX];
    unsigned int glyph_height = BigFont[GLYPH_HEIGHT_INDEX];
    unsigned int glyph_size = 1 + ((glyph_width/8) * glyph_height);
    while (*p_message)
    {
        unsigned int glyph_num = *p_message - BigFont[GLYPH_OFFSET_INDEX];
        if (glyph_num >= BigFont[GLYPH_NUM_GLYPHS_INDEX])
        {
            glyph_num = '?' - BigFont[GLYPH_OFFSET_INDEX];
        }
        const uint8_t *p_glyph = &BigFont[GLYPH_START_INDEX + (glyph_size * glyph_num)];
        lcd_paint_mono_rectangle(fg, bg, x, x+glyph_width-1, y, y+glyph_height-1, &p_glyph[1]);
        p_message++;
        if (monospace)
        {
            x+=glyph_width;
        }
        else
        {
            x+=p_glyph[0];
        }
    }
}

void font_glyph_width_small(char x)
{
    unsigned int glyph_width = BigFont[GLYPH_WIDTH_INDEX];
    unsigned int glyph_width_bytes = (glyph_width+7)/8;
    unsigned int glyph_height = BigFont[GLYPH_HEIGHT_INDEX];
    unsigned int glyph_size = 1 + ((glyph_width/8) * glyph_height);
    unsigned int glyph_num = x - BigFont[GLYPH_OFFSET_INDEX];
    if (glyph_num > BigFont[GLYPH_NUM_GLYPHS_INDEX])
    {
        return;
    }
    const uint8_t *p_glyph = &BigFont[GLYPH_START_INDEX + (glyph_size * glyph_num)];
    size_t max_width = 0;
    printf("Char '%c' : %u given, ", x, p_glyph[0]);
    for(unsigned int y = 0; y < glyph_height; y++)
    {
        uint16_t row = p_glyph[1 + (y * glyph_width_bytes)];
        row = row << 8;
        row |= p_glyph[2 + (y * glyph_width_bytes)];
        size_t width = 16;
        while((row & 0x01) == 0)
        {
            width--;
            row = row >> 1;
            row |= 0x8000;
        }
        if (width > max_width)
        {
            max_width = width;
        }
    }
    printf("%u measured,\n", max_width + 1);
}

/**************************************************
* Private Functions
***************************************************/

/* None */

/**************************************************
* End of file
***************************************************/

