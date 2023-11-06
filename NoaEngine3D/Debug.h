#ifndef NOAENGINE_DEBUG_H
#define NOAENGINE_DEBUG_H

#include <string>
#include <iostream>

namespace noa {
	class Debug
	{
	public:
		static void Log(const std::string& msg) {
			std::cout << "[INFO " << "]:" << msg << std::endl;
		}

		static void Warring(const std::string& msg)
		{
			const std::string yellowColor = "\033[33m";
			const std::string resetColor = "\033[0m";
			std::cout << yellowColor << "[WARNING]: " << msg << resetColor << std::endl;
		}

		static void Error(const std::string& msg) {
			const std::string redColor = "\033[31m";
			const std::string resetColor = "\033[0m";
			std::cout << redColor << "[ERROR]: " << msg << resetColor << std::endl;
		}

	};

	template<typename T>
	inline std::string ToString(T value) {
		return std::to_string(value);
	}

}


#endif // !NOAENGINE_DEBUG_H

