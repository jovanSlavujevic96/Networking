#include "crtsp_request_package.h"

static constexpr uint16_t myStrlen(const char* string)
{
	uint16_t i = 0;
	for (; i < string[i] != '\0'; i++);
	return i;
}

CRtspRequestPackage::CRtspRequestPackage(uint16_t size) :
	mSize{size},
	mRequestLen{0},
	mRtspMessage{std::make_unique<char[]>(size)}
{

}

char* CRtspRequestPackage::data()
{
	return mRtspMessage.get();
}

const char* CRtspRequestPackage::cData() const
{
	return mRtspMessage.get();
}

uint16_t CRtspRequestPackage::getCurrentSize() const
{
	return myStrlen(mRtspMessage.get());
}

uint16_t CRtspRequestPackage::getMaxSize() const
{
	return mSize;
}
