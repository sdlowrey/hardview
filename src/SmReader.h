#pragma once
#include <string>

#include "SmElement.h"
#inlcude "types.h"

class SmReader
{
public:
	virtual SmElement *getElement(u8 type) = 0;
protected:
	SmReader();
private:
	u8 majorVer;
	u8 minorVer;
};

class SmBinaryReader : public SmReader
{
public:
	SmBinaryReader(std::string path);
	SmElement *getElement(u8 type);
private:
	u16 maxSize;
	u16 tableLen;
	u32 tablePtr;
	u16 nStructs;
};

class SmTextReader : public SmReader
{
public:
	SmTextReader(std::string path);
	SmElement *getElement(u8 type);
};
