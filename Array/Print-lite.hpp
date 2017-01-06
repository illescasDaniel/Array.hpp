#pragma once

#include <typeinfo>

using namespace std;

// Functions from Print.hpp (https://github.com/illescasDaniel/print.hpp)

// Stub function (don't use externally) (useful when trying to pass a string to the 'to_string' function, to avoid problems)
string to_string(const string& str) {
	return str;
}

// Convert char to string
string to_string(const char& chr) {
	return string(1,chr);
}

// Return a single or double quoted string IF the data is a string or a char
template <typename Type>
string quotedString(const Type& data) {
	
	if (typeid(data) == typeid(string)) {
		return ("\"" + to_string(data) + "\"");
	}
	else if (typeid(data) == typeid(char)) {
		return ("\'" + to_string(data) + "\'");
	}
	else {
		return to_string(data);
	}
}
