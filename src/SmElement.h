#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <string.h>
#include "types.h"

class SmElement
{
public:
	SmElement();
	SmElement(u8 *p) : p(p), type(p[0]), len(p[1]), handle(WORD(p+0x02)) { 
		p += len;
		while (*p != '\0') {
			strs.push_back(std::string((const char *)p));
			p = p + strlen((char *)p) + 1;
		};
		/* for (auto itr = strs.begin(); itr != strs.end(); ++itr) { */
		/* 	std::cout << *itr << std::endl; */
		/* } */
	};

	// mixing the low-level stuff with this class is bad.
	// consider moving all low-level SMBios stuff into a Strategy.
	// store only high-level members in this class.

private:

protected:
	const u8 *p;
	u8 type;
	u8 len;
	u16 handle;
	bool isRequired;
	std::vector<std::string> strs;
};

class Bios : public SmElement 
{
public:
	Bios(u8 *p) : SmElement(p) {
	  //tis stupid to assign these strings when they already exist in a vector.
		if (p[0x04] != 0) {
			vendor = std::string(strs[p[0x04]-1]);
		} else {
			vendor = "Not Specified";
		}
		if (p[0x05] != 0) {
			version = std::string(strs[p[0x05]-1]);
		} else {
			version = "Not Specified";
		}
		if (p[0x08] != 0) {
			releaseDate = std::string(strs[p[0x08]-1]);
		} else {
			releaseDate = "Not Specified";
		}
		std::cout << "vendor: " << vendor << std::endl;
	};

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
	
	u8 *p;
	u8 type;
	u8 len;
	u16 handle;		
	
};
