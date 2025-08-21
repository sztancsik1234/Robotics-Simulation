#pragma once
#include <stdexcept>

class DivisionByZeroException : public std::runtime_error {
	public:
	DivisionByZeroException() : std::runtime_error("Division by zero") {}
};

class NotImplementedException : public std::runtime_error {
	public:
	NotImplementedException() : std::runtime_error("Call to function not implemented yet") {}
};

class WrongKeyCodeException : public std::invalid_argument {
	public:
	WrongKeyCodeException() : std::invalid_argument("Unsupported KeyCode") {}
	explicit WrongKeyCodeException(std::string const& key) 
		: std::invalid_argument("Unsupported KeyCode: " + key) {}
};