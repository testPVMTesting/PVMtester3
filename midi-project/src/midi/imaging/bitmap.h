#ifndef BITMAP_H
#define BITMAP_H

#include "imaging/color.h"
#include "util/grid.h"
#include <memory>
#include <string>
#include <functional>


namespace imaging
{
    /// <summary>
    /// Represents a bitmap, i.e. a 2D grid of colors.
    /// </summary>
    class Bitmap final
    {
    public:
        Bitmap(unsigned width, unsigned height, std::function<Color(const Position&)> initializer);

        /// <summary>
        /// Creates a new bitmap width given <paramref name="width" /> and <paramref name="height" />.
        /// All pixels are initialized to black.
        /// </summary>
        Bitmap(unsigned width, unsigned height);

        /// <summary>
        /// Copy constructor.
        /// </summary>
        Bitmap(const Bitmap&) = default;

        /// <summary>
        /// Checks if the given <paramref name="position" /> is inside the bitmap.
        /// </summary>
        bool is_inside(const Position& position) const;

        /// <summary>
        /// Gives access to the pixel at the given <paramref name="position" />.
        /// </summary>
        Color& operator [](const Position& position);

        /// <summary>
        /// Gives readonly access to the pixel at the given <paramref name="position" />.
        /// </summary>
        const Color& operator [](const Position&) const;

        /// <summary>
        /// Returns the width of the bitmap.
        /// </summary>
        unsigned width() const;

        /// <summary>
        /// Returns the height of the bitmap.
        /// </summary>
        unsigned height() const;

        /// <summary>
        /// Calls the given <paramref name="function" /> once for each pixel position.
        /// This is basically a loop that iterates over the entire bitmap.
        /// No specific order is guaranteed.
        /// </summary>
        void for_each_position(std::function<void(const Position&)> function) const;

        /// <summary>
        /// Overwrites all pixels with the given <paramref name="color" />.
        /// </summary>
        void clear(const Color& color);

        std::shared_ptr<Bitmap> slice(int x, int y, int width, int height) const;

    private:
        Bitmap(std::shared_ptr<Grid<Color>> pixels);

        std::shared_ptr<Grid<Color>> m_pixels;
    };
}

#endif
