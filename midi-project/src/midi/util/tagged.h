#ifndef TAGGED_H
#define TAGGED_H

#include <iostream>


template<typename T, typename TAG>
class tagged
{
private:
    T _value;

public:
    tagged() : _value() { }

    explicit tagged(const T& value) : _value(value) { }    

    // https://isocpp.org/wiki/faq/templates#template-friends
    friend T& value(tagged<T, TAG>& st)
    {
        return st._value;
    }

    friend const T& value(const tagged<T, TAG>& st)
    {
        return st._value;
    }
};

template<typename T>
struct equality
{
    friend bool operator ==(const T& x, const T& y)
    {
        return value(x) == value(y);
    }

    friend bool operator !=(const T& x, const T& y)
    {
        return value(x) != value(y);
    }
};

template<typename T>
struct ordered : equality<T>
{
    friend bool operator <(const T& x, const T& y)
    {
        return value(x) < value(y);
    }

    friend bool operator <=(const T& x, const T& y)
    {
        return value(x) <= value(y);
    }

    friend bool operator >(const T& x, const T& y)
    {
        return value(x) > value(y);
    }

    friend bool operator >=(const T& x, const T& y)
    {
        return value(x) >= value(y);
    }
};

template<typename T, typename CONVERT_TO>
struct show_value_helper
{
    std::ostream& print(std::ostream& out, const T& x)
    {
        return out << CONVERT_TO(value(x));
    }
};

template<typename T>
struct show_value_helper<T, void>
{
    std::ostream& print(std::ostream& out, const T& x)
    {
        using CONVERT_TO = typename T::inner_type;
        return out << CONVERT_TO(value(x));
    }
};

template<typename T, typename CONVERT_TO = void>
struct show_value
{
    friend std::ostream& operator <<(std::ostream& out, const T& x)
    {
        return show_value_helper<T, CONVERT_TO>().print(out, x);
    }
};

#endif
