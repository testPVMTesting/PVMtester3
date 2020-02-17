#ifndef READ_H
#define READ_H

#include <istream>
#include <iostream>
#include "logging.h"

namespace io {
	template<typename T>
	void read_to(std::istream& in, T* buffer, size_t n)
	{
		in.read(reinterpret_cast<char*>(buffer), sizeof(T) * n);
		CHECK(!in.fail()) << __FUNCTION__ << " failed";
	}
	template<typename T>
	void read_to(std::istream& in, T* buffer) {
		read_to(in, buffer, 1);
	};

	template<typename T, typename std::enable_if<std::is_fundamental<T>::value, T>::type* = nullptr>
	T read(std::istream& in)
	{
		T object;
		read_to(in, &object);
		return object;
	}

	template<typename T>
	std::unique_ptr<T[]> read_array(std::istream& in, size_t n) {
		std::unique_ptr<T[]> object = std::make_unique<T[]>(n);
		read_to(in, object.get(), n);
		return object;
	};
}

#endif
