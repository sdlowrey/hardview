#pragma once
#include <string>

#include "SmElement.h"
#include "types.h"

class SmReader
{
public:
	virtual ~SmReader();
	virtual std::vector<SmElement> getAllElements() = 0;
	virtual SmElement *getElement(u8 type) = 0;
	virtual std::string getVersion() = 0;
	virtual std::string getTableInfo() = 0;
protected:
	SmReader() = default;
private:
	u8 majorVer;
	u8 minorVer;
};

class SmBinaryReader : public SmReader
{
public:
	SmBinaryReader(std::string p);
	~SmBinaryReader();

	std::vector<SmElement> getAllElements();
	SmElement *getElement(u8 type);
	std::string getVersion();
	std::string getTableInfo();
private:
	void processEntryPoint();
	bool isValidType(u8 t);
	std::string path;
	u16 maxSize;
	u16 tableLen;
	u32 tablePtr;
	u16 nStructs;
	u8 *table;
};

class SmTextReader : public SmReader
{
public:
	SmTextReader(std::string path);
	~SmTextReader();

	SmElement *getElement(u8 type);
	std::string getVersion();
	std::string getTableInfo();
};
