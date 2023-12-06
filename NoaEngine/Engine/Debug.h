#ifndef NOAENGINE_DEBUG_H
#define NOAENGINE_DEBUG_H

#include <string>
#include <iostream>

#include "Core.h"
#include "NoaMath.h"

namespace noa {
	class NOA_API Debug
	{
	public:
		static void Log(const std::string& msg) {
			std::cout << "[INFO " << "]:" << msg << '\n';
		}

		static void Warring(const std::string & msg) 
		{
			const std::string yellowColor = "\033[33m";
			const std::string resetColor = "\033[0m";
			std::cout << yellowColor << "[WARNING]: " << msg << resetColor << '\n';
		}

		static void Error(const std::string& msg) {
			const std::string redColor = "\033[31m";
			const std::string resetColor = "\033[0m";
			std::cout << redColor << "[ERROR]: " << msg << resetColor << '\n';
		}

	};

	template<typename T>
	inline NOA_API std::string ToString(T value) {
		return std::to_string(value);
	}

	template<typename T>
	inline NOA_API std::string ToString(const noa::Vector<T> & value)
	{
		return "(" + std::to_string(value.x) + "," + std::to_string(value.y) + ")";
	}

}


#endif // !NOAENGINE_DEBUG_H

