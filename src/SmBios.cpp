#include "SmBios.h"

using namespace std;

SmBios::SmBios(string f = "/dev/mem") : infile(f) 
{ 
	getTable(); 
}

string SmBios::source()
{
	return infile;
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
