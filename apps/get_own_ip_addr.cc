#include <iostream>
#include <vector>

#include "socket_utils.h"
#include "csocket_exception.h"

int  main()
{
	std::string ip_addr;
	try
	{
		ip_addr = ::getOwnIpV4Address();
	}
	catch (const CSocketException& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch (const std::bad_alloc& e)
	{
		std::cerr << "Heap alloc error: " << e.what() << std::endl;
	}
	std::cout << "IP Address of this machine is: " << ip_addr << std::endl;
	return 0;
}