#include <iostream>
#include <string>
#include <stdexcept>

#include "SmBios.h"

using namespace std;

int main(int argc, char * const argv[])
{
	string path = "/dev/mem";
	if (argc > 1) {
		path = argv[1];
	}
	SmBios smbios;
	try {
		smbios.decodePath(path);
	} catch (runtime_error e) {
		cout << e.what() << endl;
		return 1;
	}
}
