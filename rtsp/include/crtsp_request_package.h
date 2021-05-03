#pragma once

#include <memory>
#include <climits>

#include "ipackage.h"

class CRtspRequestPackage : public IPackage
{
public:
	CRtspRequestPackage(uint16_t size=USHRT_MAX);
	~CRtspRequestPackage() = default;

	char* data() override;
	const char* cData() const override;
	uint16_t getCurrentSize() const override;
	uint16_t getMaxSize() const override;
private:
	std::unique_ptr<char[]> mRtspMessage;
	uint16_t mSize;
	uint16_t mRequestLen;
};
