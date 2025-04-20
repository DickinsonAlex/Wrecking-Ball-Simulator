#pragma once
#include <string>

#ifndef EXCEPTION_H
#define EXCEPTION_H

class Exception
{
	std::string message;

public:
	Exception(const std::string& message) : message(message) {};

	virtual const std::string& GetMessage() const throw() { return message; }
};
#endif // EXCEPTION_H