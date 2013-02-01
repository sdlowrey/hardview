#pragma once
#include <iostream>
#include <string>
#include <stdio.h>
#include "types.h"

class SmElement
{
public:
	SmElement();
	SmElement(u8 *p) : p(p), type(p[0]), len(p[1]), handle(WORD(p+0x02)) { 
		std::cout << "creating " << strType() << std::endl;
	};

	const std::string strType();

	// mixing the low-level stuff with this class is bad.
	// consider moving all low-level SMBios stuff into a Strategy.
	// store only high-level members in this class.
	u8 type;
	u8 len;
	u16 handle;

private:
	// void getHeader() {
	// 	type = p[0];
	// 	len = p[1];
	// 	handle = WORD(p + 0x02);		
	// };

protected:
	const u8 *p;
	bool isRequired;
	
};

class Bios : public SmElement 
{
public:
	Bios(u8 *p) : SmElement(p) {};

private:
	std::string vendor, version, releaseDate;
};

class System : public SmElement
{
public:
	System(u8 *p) : SmElement(p) {};

private:
	std::string manufacturer, product, version, serial, sku, family;
};

class Baseboard : public SmElement
{
public:
	Baseboard(u8 *p) : SmElement(p) {};

private:
	std::string manufacturer, product, version, serial, assetTag;
};


class SmElementFactory
{
public:
	SmElementFactory() = default;
	
	SmElement *create(u8 *p);
private:
	bool isValidType();
	
	u8 *p;
	u8 type;
	u8 len;
	u16 handle;		
	
};
