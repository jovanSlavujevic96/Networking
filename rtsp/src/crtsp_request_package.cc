#include <sstream>

#include "crtsp_request_package.h"

static constexpr uint16_t myStrlen(const char* string)
{
	uint16_t i = 0;
	for (; string[i] != '\0'; i++);
	return i;
}

template <typename T>
static constexpr std::string to_string_with_precision(T& a_value, uint8_t n = 6)
{
	std::ostringstream out;
	out.precision(n);
	out << std::fixed << a_value;
	return out.str();
}

CRtspRequestPackage::CRtspRequestPackage(uint16_t size) :
	mSize{size},
	mRequestLen{0},
	mRtspMessage{std::make_unique<char[]>(size)}
{
	*mRtspMessage.get() = '\0';
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
	//return ::myStrlen(mRtspMessage.get());
	return mRequestLen;
}

uint16_t CRtspRequestPackage::getMaxSize() const
{
	return mSize;
}

void CRtspRequestPackage::clear()
{
	*mRtspMessage.get() = '\0';
	mRequestLen = 0;
}

CRtspRequestPackage& CRtspRequestPackage::operator<<(const char* string)
{
	mRequestLen += (uint16_t)std::snprintf(mRtspMessage.get() + mRequestLen, mSize, "%s", string);
	return *this;
}

CRtspRequestPackage& CRtspRequestPackage::operator<<(const std::string& string)
{
	(void)std::snprintf(mRtspMessage.get() + mRequestLen, mSize, "%s", string.c_str());
	mRequestLen += (uint16_t)string.size();
	return *this;
}

CRtspRequestPackage& CRtspRequestPackage::operator<<(char character)
{
	mRequestLen += (uint16_t)std::snprintf(mRtspMessage.get() + mRequestLen, mSize, "%c", character);
	return *this;
}

CRtspRequestPackage& CRtspRequestPackage::operator<<(int16_t value)
{
	mRequestLen += (uint16_t)std::snprintf(mRtspMessage.get() + mRequestLen, mSize, "%hd", value);
	return *this;
}

CRtspRequestPackage& CRtspRequestPackage::operator<<(uint16_t value)
{
	mRequestLen += (uint16_t)std::snprintf(mRtspMessage.get() + mRequestLen, mSize, "%hu", value);
	return *this;
}

CRtspRequestPackage& CRtspRequestPackage::operator<<(int32_t value)
{
	mRequestLen += (uint16_t)std::snprintf(mRtspMessage.get() + mRequestLen, mSize, "%d", value);
	return *this;
}

CRtspRequestPackage& CRtspRequestPackage::operator<<(uint32_t value)
{
	mRequestLen += (uint16_t)std::snprintf(mRtspMessage.get() + mRequestLen, mSize, "%u", value);
	return *this;
}

CRtspRequestPackage& CRtspRequestPackage::operator<<(int64_t value)
{
	mRequestLen += (uint16_t)std::snprintf(mRtspMessage.get() + mRequestLen, mSize, "%lld", value);
	return *this;
}

CRtspRequestPackage& CRtspRequestPackage::operator<<(uint64_t value)
{
	mRequestLen += (uint16_t)std::snprintf(mRtspMessage.get() + mRequestLen, mSize, "%llu", value);
	return *this;
}

CRtspRequestPackage& CRtspRequestPackage::operator<<(float value)
{
	mRequestLen += (uint16_t)std::snprintf(mRtspMessage.get() + mRequestLen, mSize, "%.1f", value);
	return *this;
}

CRtspRequestPackage& CRtspRequestPackage::operator>>(const char* string)
{
	uint16_t strlen_ = ::myStrlen(string);
	std::memmove(&mRtspMessage[strlen_], mRtspMessage.get(), (size_t)mRequestLen + 1);
	std::memcpy(mRtspMessage.get(), string, strlen_);
	mRequestLen += strlen_;
	return *this;
}

CRtspRequestPackage& CRtspRequestPackage::operator>>(const std::string& string)
{
	uint16_t strlen_ = (uint16_t)string.size();
	std::memmove(&mRtspMessage[strlen_], mRtspMessage.get(), (size_t)mRequestLen + 1);
	std::memcpy(mRtspMessage.get(), string.c_str(), strlen_);
	mRequestLen += strlen_;
	return *this;
}

CRtspRequestPackage& CRtspRequestPackage::operator>>(char character)
{
	std::memmove(mRtspMessage.get() + 1, mRtspMessage.get(), (size_t)mRequestLen + 1);
	std::memcpy(mRtspMessage.get(), &character, 1);
	mRequestLen += 1;
	return *this;
}

CRtspRequestPackage& CRtspRequestPackage::operator>>(int16_t value)
{
	std::string val_str = std::to_string(value);
	uint16_t strlen_ = (uint16_t)val_str.size();
	std::memmove(&mRtspMessage[strlen_], mRtspMessage.get(), (size_t)mRequestLen + 1);
	std::memcpy(mRtspMessage.get(), val_str.c_str(), strlen_);
	mRequestLen += strlen_;
	return *this;
}

CRtspRequestPackage& CRtspRequestPackage::operator>>(uint16_t value)
{
	std::string val_str = std::to_string(value);
	uint16_t strlen_ = (uint16_t)val_str.size();
	std::memmove(&mRtspMessage[strlen_], mRtspMessage.get(), (size_t)mRequestLen + 1);
	std::memcpy(mRtspMessage.get(), val_str.c_str(), strlen_);
	mRequestLen += strlen_;
	return *this;
}

CRtspRequestPackage& CRtspRequestPackage::operator>>(int32_t value)
{
	std::string val_str = std::to_string(value);
	uint16_t strlen_ = (uint16_t)val_str.size();
	std::memmove(&mRtspMessage[strlen_], mRtspMessage.get(), (size_t)mRequestLen + 1);
	std::memcpy(mRtspMessage.get(), val_str.c_str(), strlen_);
	mRequestLen += strlen_;
	return *this;
}

CRtspRequestPackage& CRtspRequestPackage::operator>>(uint32_t value)
{
	std::string val_str = std::to_string(value);
	uint16_t strlen_ = (uint16_t)val_str.size();
	std::memmove(&mRtspMessage[strlen_], mRtspMessage.get(), (size_t)mRequestLen + 1);
	std::memcpy(mRtspMessage.get(), val_str.c_str(), strlen_);
	mRequestLen += strlen_;
	return *this;
}

CRtspRequestPackage& CRtspRequestPackage::operator>>(int64_t value)
{
	std::string val_str = std::to_string(value);
	uint16_t strlen_ = (uint16_t)val_str.size();
	std::memmove(&mRtspMessage[strlen_], mRtspMessage.get(), (size_t)mRequestLen + 1);
	std::memcpy(mRtspMessage.get(), val_str.c_str(), strlen_);
	mRequestLen += strlen_;
	return *this;
}

CRtspRequestPackage& CRtspRequestPackage::operator>>(uint64_t value)
{
	std::string val_str = std::to_string(value);
	uint16_t strlen_ = (uint16_t)val_str.size();
	std::memmove(&mRtspMessage[strlen_], mRtspMessage.get(), (size_t)mRequestLen + 1);
	std::memcpy(mRtspMessage.get(), val_str.c_str(), strlen_);
	mRequestLen += strlen_;
	return *this;
}

CRtspRequestPackage& CRtspRequestPackage::operator>>(float value)
{
	std::string val_str = ::to_string_with_precision<float>(value, 1);
	uint16_t strlen_ = (uint16_t)val_str.size();
	std::memmove(&mRtspMessage[strlen_], mRtspMessage.get(), (size_t)mRequestLen + 1);
	std::memcpy(mRtspMessage.get(), val_str.c_str(), strlen_);
	mRequestLen += strlen_;
	return *this;
}

char& CRtspRequestPackage::operator[](uint16_t i)
{
	return mRtspMessage[i];
}
