#include "imaging/bmp-format.h"
#include <algorithm>
#include <assert.h>
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <cstring>


using namespace imaging;

namespace
{
#   pragma pack(push, r1, 1)
    struct FILE_HEADER
    {
        uint16_t FileType;     /* File type, always 4D42h ("BM") */
        uint32_t FileSize;     /* Size of the file in bytes */
        uint16_t Reserved1;    /* Always 0 */
        uint16_t Reserved2;    /* Always 0 */
        uint32_t BitmapOffset; /* Starting position of image data in bytes */
    };

    struct BITMAP_HEADER_V5
    {
        uint32_t Size;            /* Size of this header in bytes */
        int32_t  Width;           /* Image width in pixels */
        int32_t  Height;          /* Image height in pixels */
        uint16_t Planes;          /* Number of color planes */
        uint16_t BitsPerPixel;    /* Number of bits per pixel */
        uint32_t Compression;     /* Compression methods used */
        uint32_t SizeOfBitmap;    /* Size of bitmap in bytes */
        int32_t  HorzResolution;  /* Horizontal resolution in pixels per meter */
        int32_t  VertResolution;  /* Vertical resolution in pixels per meter */
        uint32_t ColorsUsed;      /* Number of colors in the image */
        uint32_t ColorsImportant; /* Minimum number of important colors */
        uint32_t RedMask;         /* Mask identifying bits of red component */
        uint32_t GreenMask;       /* Mask identifying bits of green component */
        uint32_t BlueMask;        /* Mask identifying bits of blue component */
        uint32_t AlphaMask;       /* Mask identifying bits of alpha component */
        uint32_t CSType;          /* Color space type */
        int32_t  RedX;            /* X coordinate of red endpoint */
        int32_t  RedY;            /* Y coordinate of red endpoint */
        int32_t  RedZ;            /* Z coordinate of red endpoint */
        int32_t  GreenX;          /* X coordinate of green endpoint */
        int32_t  GreenY;          /* Y coordinate of green endpoint */
        int32_t  GreenZ;          /* Z coordinate of green endpoint */
        int32_t  BlueX;           /* X coordinate of blue endpoint */
        int32_t  BlueY;           /* Y coordinate of blue endpoint */
        int32_t  BlueZ;           /* Z coordinate of blue endpoint */
        uint32_t GammaRed;        /* Gamma red coordinate scale value */
        uint32_t GammaGreen;      /* Gamma green coordinate scale value */
        uint32_t GammaBlue;       /* Gamma blue coordinate scale value */
        uint32_t Intent;
        uint32_t ProfileData;
        uint32_t ProfileSize;
        uint32_t Reserved;
    };

    struct BITMAP_FILE_V5
    {
        FILE_HEADER      file_header;
        BITMAP_HEADER_V5 bitmap_header;
    };

    struct ARGB
    {
        uint8_t b;
        uint8_t g;
        uint8_t r;
        uint8_t a;
    };

    struct RGB
    {
        uint8_t b;
        uint8_t g;
        uint8_t r;
    };
#   pragma pack(pop, r1)


    ARGB to_argb(const Color& c)
    {
        uint8_t a = 255;
        uint8_t r = uint8_t(c.r * 255);
        uint8_t g = uint8_t(c.g * 255);
        uint8_t b = uint8_t(c.b * 255);

        return ARGB{ b, g, r, a };
    }
}

void imaging::save_as_bmp(const std::string& path, const Bitmap& bitmap)
{
    std::ofstream out(path, std::ios::binary);
    save_as_bmp(out, bitmap);
}

void imaging::save_as_bmp(std::ostream& out, const Bitmap& bitmap)
{
    BITMAP_FILE_V5 header;
    memset(&header, 0, sizeof(header));

    header.file_header.FileType = 0x4D42;
    header.file_header.FileSize = sizeof(BITMAP_FILE_V5) + 4 * bitmap.width() * bitmap.height();
    header.file_header.Reserved1 = 0;
    header.file_header.Reserved2 = 0;
    header.file_header.BitmapOffset = sizeof(BITMAP_FILE_V5);

    header.bitmap_header.Size = sizeof(BITMAP_HEADER_V5);
    header.bitmap_header.Width = bitmap.width();
    header.bitmap_header.Height = bitmap.height();
    header.bitmap_header.Planes = 1;
    header.bitmap_header.BitsPerPixel = 32;
    header.bitmap_header.Compression = 0;
    header.bitmap_header.SizeOfBitmap = 0;
    header.bitmap_header.HorzResolution = 3779;
    header.bitmap_header.VertResolution = 3779;
    header.bitmap_header.ColorsUsed = 0;
    header.bitmap_header.ColorsImportant = 0;
    header.bitmap_header.RedMask = 0x00FF0000;
    header.bitmap_header.GreenMask = 0x0000FF00;
    header.bitmap_header.BlueMask = 0x000000FF;
    header.bitmap_header.AlphaMask = 0xFF000000;
    header.bitmap_header.CSType = 0x73524742;
    header.bitmap_header.Intent = 4;
    
    out.write(reinterpret_cast<char*>(&header), sizeof(header));

    std::unique_ptr<ARGB[]> scanline = std::make_unique<ARGB[]>(bitmap.width());

    for (int y = bitmap.height() - 1; y >= 0; --y)
    {
        for (unsigned x = 0; x < bitmap.width(); ++x)
        {
            Position pos(x, y);

            scanline[x] = to_argb(bitmap[pos]);
        }

        out.write(reinterpret_cast<char*>(scanline.get()), sizeof(ARGB) * bitmap.width());
    }
}
