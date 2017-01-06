#pragma once

#include <typeinfo>

// Functions from Print.hpp (https://github.com/illescasDaniel/print.hpp)

namespace std {
	// Stub function (don't use externally) (useful when trying to pass a string to the 'to_string' function, to avoid problems)
	std::string to_string(const std::string& str) {
		return str;
	}

	// Convert char to string
	std::string to_string(const char& chr) {
		return std::string(1,chr);
	}
}

// Return a single or double quoted string IF the data is a string or a char
template <typename Type>
std::string quotedString(const Type& data) {
	
	if (typeid(data) == typeid(std::string)) {
		return ("\"" + std::to_string(data) + "\"");
	}
	else if (typeid(data) == typeid(char)) {
		return ("\'" + std::to_string(data) + "\'");
	}
	else {
		return std::to_string(data);
	}
}
