#ifndef COLOR_H
#define COLOR_H

#include <iostream>


namespace imaging
{
    /// <summary>
    /// Color type.
    /// </summary>
    struct Color final
    {
        /// <summary>
        /// Red component.
        /// </summary>
        double r;
        
        /// <summary>
        /// Green component.
        /// </summary>
        double g;
            
        /// <summary>
        /// Blue component.
        /// </summary>
        double b;

        /// <summary>
        /// Default constructor. Initialized the color to black.
        /// </summary>
        constexpr Color() : Color(0, 0, 0) { }

        /// <summary>
        /// Constructor.
        /// </summary>
        constexpr Color(double r, double g, double b)
            : r(r), g(g), b(b) { }
    };

    // Example usage: Color c = colors::black();
    namespace colors
    {
        inline Color black()        { return Color{ 0, 0, 0 }; }
        inline Color white()        { return Color{ 1, 1, 1 }; }
        inline Color red()          { return Color{ 1, 0, 0 }; }
        inline Color green()        { return Color{ 0, 1, 0 }; }
        inline Color blue()         { return Color{ 0, 0, 1 }; }
        inline Color yellow()       { return Color{ 1, 1, 0 }; }
        inline Color magenta()      { return Color{ 1, 0, 1 }; }
        inline Color cyan()         { return Color{ 0, 1, 1 }; }
        inline Color orange()       { return Color{ 1, 0.64, 0 }; }
    }
}

imaging::Color operator +(const imaging::Color&, const imaging::Color&);
imaging::Color operator -(const imaging::Color&, const imaging::Color&);
imaging::Color operator *(const imaging::Color&, double);
imaging::Color operator *(double, const imaging::Color&);
imaging::Color operator *(const imaging::Color&, const imaging::Color&);
imaging::Color operator /(const imaging::Color&, double);

imaging::Color& operator +=(imaging::Color&, const imaging::Color&);
imaging::Color& operator -=(imaging::Color&, const imaging::Color&);
imaging::Color& operator *=(imaging::Color&, double);
imaging::Color& operator /=(imaging::Color&, double);

bool operator ==(const imaging::Color&, const imaging::Color&);
bool operator !=(const imaging::Color&, const imaging::Color&);

std::ostream& operator <<(std::ostream&, const imaging::Color&);

#endif
