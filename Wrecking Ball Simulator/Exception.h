#pragma once
#include <string>

#ifndef Exception_h
#define Exception_h

class Exception
{
	std::string message;

public:
	Exception(const std::string& message) : message(message) {};

	virtual const std::string& GetMessage() const throw() { return message; }
};
#endif // !Exception_h