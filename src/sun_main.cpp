// sun.cpp : Defines the entry point for the application.
//
#include <iostream>
#include "sun_server.h"


int main()
{
	std::cout << "Hello CMake SUN ." << std::endl;
	
	sun_server sun;
	sun.start();

	std::cout << "Goodbye CMake SUN ." << std::endl;
	return 0;
}
