#pragma once

#include <memory>
#include <climits>
#include <string>

#include "ipackage.h"

class CRtspRequestPackage : public IPackage
{
public:
	CRtspRequestPackage(uint16_t size=USHRT_MAX);
	~CRtspRequestPackage() = default;

	const char* cData() const override;
	uint16_t getCurrentSize() const override;
	uint16_t getMaxSize() const override;

	void clear();

	CRtspRequestPackage& operator<<(const char* string);
	CRtspRequestPackage& operator<<(const std::string& string);
	CRtspRequestPackage& operator<<(char character);
	CRtspRequestPackage& operator<<(int16_t value);
	CRtspRequestPackage& operator<<(uint16_t value);
	CRtspRequestPackage& operator<<(int32_t value);
	CRtspRequestPackage& operator<<(uint32_t value);
	CRtspRequestPackage& operator<<(int64_t value);
	CRtspRequestPackage& operator<<(uint64_t value);
	CRtspRequestPackage& operator<<(float value);

	CRtspRequestPackage& operator>>(const char* string);
	CRtspRequestPackage& operator>>(const std::string& string);
	CRtspRequestPackage& operator>>(char character);
	CRtspRequestPackage& operator>>(int16_t value);
	CRtspRequestPackage& operator>>(uint16_t value);
	CRtspRequestPackage& operator>>(int32_t value);
	CRtspRequestPackage& operator>>(uint32_t value);
	CRtspRequestPackage& operator>>(int64_t value);
	CRtspRequestPackage& operator>>(uint64_t value);
	CRtspRequestPackage& operator>>(float value);

	char& operator[](uint16_t i);

private:
	char* data() override;

	std::unique_ptr<char[]> mRtspMessage;
	uint16_t mSize;
	uint16_t mRequestLen;
};
