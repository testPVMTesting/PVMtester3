#ifndef BMP_FORMAT_H
#define BMP_FORMAT_H

#include "imaging/bitmap.h"


namespace imaging
{
    void save_as_bmp(const std::string& path, const Bitmap& bitmap);
    void save_as_bmp(std::ostream& out, const Bitmap& bitmap);
}

#endif
