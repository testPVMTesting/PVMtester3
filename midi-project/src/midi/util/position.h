#ifndef POSITION_H
#define POSITION_H

#include <iostream>


struct Position final
{
    unsigned x, y;

    Position(unsigned x, unsigned y)
        : x(x), y(y) { }

    Position operator +(const Position& p) const
    {
        return Position(x + p.x, y + p.y);
    }
};

inline std::ostream& operator <<(std::ostream& out, const Position& p)
{
    return out << "(" << p.x << "," << p.y << ")";
}

#endif
