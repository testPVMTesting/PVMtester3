#ifndef GRID_H
#define GRID_H

#include "util/position.h"
#include <memory>
#include <functional>
#include <assert.h>


template<typename T>
class Grid
{
public:
    virtual ~Grid() { }

    virtual T& operator[](const Position&) = 0;
    virtual const T& operator[](const Position&) const = 0;

    virtual unsigned width() const = 0;
    virtual unsigned height() const = 0;

    bool is_inside(const Position& p) const
    {
        return p.x < width() && p.y < height();
    }

    void for_each_position(std::function<void(const Position&)> function) const
    {
        for (unsigned y = 0; y != height(); ++y)
        {
            for (unsigned x = 0; x != width(); ++x)
            {
                function(Position(x, y));
            }
        }
    }
};

template<typename T>
class ConcreteGrid : public Grid<T>
{
public:
    ConcreteGrid(unsigned width, unsigned height, std::function<T(const Position&)> initializer)
        : ConcreteGrid(width, height)
    {
        for (unsigned y = 0; y != height; ++y)
        {
            for (unsigned x = 0; x != width; ++x)
            {
                Position p(x, y);

                (*this)[p] = initializer(p);
            }
        }
    }

    ConcreteGrid(unsigned width, unsigned height, T initial_value)
        : ConcreteGrid(width, height, [&initial_value](const Position&) { return initial_value; })
    {
        // NOP
    }

    ConcreteGrid(unsigned width, unsigned height)
        : m_elts(std::make_unique<T[]>(width * height)), m_width(width), m_height(height)
    {
        // NOP
    }

    ConcreteGrid(const Grid<T>& grid)
        : ConcreteGrid(grid.width(), grid.height(), [&grid](const Position& p) { return grid[p]; })
    {
        // NOP
    }

    T& operator [](const Position& p) override
    {
        assert(this->is_inside(p));

        return m_elts[p.x + p.y * m_width];
    }

    const T& operator [](const Position& p) const override
    {
        assert(this->is_inside(p));

        return m_elts[p.x + p.y * m_width];
    }

    unsigned width() const override
    {
        return m_width;
    }

    unsigned height() const override
    {
        return m_height;
    }

private:
    std::unique_ptr<T[]> m_elts;
    unsigned m_width;
    unsigned m_height;
};

template<typename T>
class SubGrid : public Grid<T>
{
public:
    SubGrid(std::shared_ptr<Grid<T>> parent, const Position& position, unsigned width, unsigned height)
        : m_parent(parent), m_position(position), m_width(width), m_height(height) { }

    T& operator[](const Position& p) override
    {
        return (*m_parent)[m_position + p];
    }

    const T& operator[](const Position& p) const override
    {
        return (*m_parent)[m_position + p];
    }

    unsigned width() const override
    {
        return m_width;
    }

    unsigned height() const override
    {
        return m_height;
    }

private:
    std::shared_ptr<Grid<T>> m_parent;
    const Position m_position;
    const unsigned m_width;
    const unsigned m_height;
};

template<typename T>
std::shared_ptr<Grid<T>> subgrid(std::shared_ptr<Grid<T>> grid, const Position& p, unsigned width, unsigned height)
{
    return std::make_shared<SubGrid<T>>(grid, p, width, height);
}

#endif
