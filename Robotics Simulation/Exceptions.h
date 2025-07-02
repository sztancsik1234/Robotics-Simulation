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