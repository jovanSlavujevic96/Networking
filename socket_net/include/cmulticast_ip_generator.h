#pragma once

#include <string>
#include <vector>
#include <mutex>

class CMutlicastIpGenerator
{
public:
	static CMutlicastIpGenerator& instance();

	CMutlicastIpGenerator(const CMutlicastIpGenerator&) = delete;

	const std::string& getAddr();
	void releaseAddr(const char* addr);
	void releaseAddr(const std::string& addr);
private:
	std::vector<std::string> mMulticastIp;
	std::mutex mMutex;
	
	CMutlicastIpGenerator() = default;
	~CMutlicastIpGenerator() = default;
};
