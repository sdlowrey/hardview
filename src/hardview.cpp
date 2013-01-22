#include <iostream>
#include <string>

#include "SmBios.h"

using namespace std;

void log(string msg)
{
	cout << msg << endl;
}

int main(int argc, char * const argv[])
{
        SmBios smbios("/dev/mem");
	if (! smbios.decode()) {
		log("Unable to decode SMBios");
		return 1;  // TODO error codes?
	}
}
