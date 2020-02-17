#include "imaging/bitmap.h"
#include "util/array.h"
#include "logging.h"
#include <algorithm>
#include <assert.h>
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <cstring>


using namespace imaging;

Bitmap::Bitmap(std::shared_ptr<Grid<Color>> pixels)
    : m_pixels(pixels)
{
    // NOP
}

Bitmap::Bitmap(unsigned width, unsigned height, std::function<Color(const Position&)> initializer)
    : Bitmap(std::make_shared<ConcreteGrid<Color>>(width, height, initializer))
{
    // NOP
}

Bitmap::Bitmap(unsigned width, unsigned height)
    : Bitmap(width, height, [](const Position&) { return colors::black(); })
{
    // NOP
}

unsigned Bitmap::width() const
{
    return m_pixels->width();
}

unsigned Bitmap::height() const
{
    return m_pixels->height();
}

bool Bitmap::is_inside(const Position& p) const
{
    return p.x < width() && p.y < height();
}

Color& Bitmap::operator[](const Position& p)
{
    assert(is_inside(p));

    return (*m_pixels)[p];
}

const Color& Bitmap::operator[](const Position& p) const
{
    assert(is_inside(p));

    return (*m_pixels)[p];
}

void Bitmap::clear(const Color& Color)
{
    for_each_position([this, &Color](const Position& p) {
        (*m_pixels)[p] = Color;
    });
}

void Bitmap::for_each_position(std::function<void(const Position&)> callback) const
{
    m_pixels->for_each_position(callback);
}

std::shared_ptr<Bitmap> Bitmap::slice(int x, int y, int width, int height) const
{
    auto sg = subgrid(m_pixels, Position(x, y), width, height);

    return std::shared_ptr<Bitmap>( new Bitmap(sg) );
}