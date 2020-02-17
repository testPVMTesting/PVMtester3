#include "imaging/color.h"

using namespace imaging;


Color operator +(const Color& c1, const Color& c2)
{
    return Color(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b);
}

Color operator -(const Color& c1, const Color& c2)
{
    return Color(c1.r - c2.r, c1.g - c2.g, c1.b - c2.b);
}

Color operator *(const Color& c, double f)
{
    return Color(c.r * f, c.g * f, c.b * f);
}

Color operator *(double f, const Color& c)
{
    return c * f;
}

Color operator *(const Color& c1, const Color& c2)
{
    return Color(c1.r * c2.r, c1.g * c2.g, c1.b * c2.b);
}

Color operator /(const Color& c, double f)
{
    return c * (1 / f);
}

Color& operator +=(Color& c1, const Color& c2)
{
    return c1 = c1 + c2;
}

Color& operator -=(Color& c1, const Color& c2)
{
    return c1 = c1 - c2;
}

Color& operator *=(Color& c, double f)
{
    return c = c * f;
}

Color& operator /=(Color& c, double f)
{
    return c = c / f;
}

bool operator ==(const Color& c1, const Color& c2)
{
    return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
}

bool operator !=(const Color& c1, const Color& c2)
{
    return !(c1 == c2);
}

std::ostream& operator <<(std::ostream& out, const Color& c)
{
    return out << "RGB[" << c.r << "," << c.g << "," << c.b << "]";
}
