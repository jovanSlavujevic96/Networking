#include <iostream>

#include "crtsp_master.h"
#include "cudp_mcast_server.h"
#include "cmulticast_ip_generator.h"

int main()
{
	CRtspMaster master("0.0.0.0", 9090);
	const char* StreamingSuffix = "live";
	uint64_t hash = master.addSession(std::make_unique<CRtspMediaSession>(StreamingSuffix));
	if (hash == 0)
	{
		std::cerr << "session has not been added. hash key is zero.\n";
		return -1;
	}
	try
	{
		master.initServer();
		master.start();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return -1;
	}

	std::cout << "Multicast streamer\n"
		<< "\tsuffix  = " << StreamingSuffix << '\n'
		<< "\tip addr = " << master.getSessionIp(hash) << '\n'
		<< "\tport    = " << master.getSessionPort(hash) << "\n\n";

	std::cout << "rtsp://127.0.0.1:" << master.getPort() << '/' << StreamingSuffix << "\n\n";

	master.join();
	return 0;
}
