#include "SmBios.h"

using namespace std;

string SmBios::source()
{
	return infile;
}

StructMap SmBios::getNode(StructType t)
{
}

string SmBios::dumpText()
{
	//for key,val in structs, 
	return "dumpText: not implemented";
}

/*
void SmBios::get(BiosInfo& b)
{
	b.vendor = "quicktest";
}

void SmBios::get(SmBiosInfo& s)
{
	reader->get(s);
}
*/
