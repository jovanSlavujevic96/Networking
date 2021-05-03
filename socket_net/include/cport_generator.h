#pragma once

#include <string>
#include <vector>

class CPortGenerator
{
public:
	static CPortGenerator& instance();

	CPortGenerator(const CPortGenerator&) = delete;

	uint16_t getPort();
	void addPort(uint16_t port);
	void releasePort(uint16_t port);
private:
	std::vector<uint16_t> mPorts;

	CPortGenerator() = default;
	~CPortGenerator() = default;
};
