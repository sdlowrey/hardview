#include <iostream>
#include <string>
#include <stdexcept>

#include "SmBios.h"

using namespace std;

int main(int argc, char * const argv[])
{
	string path = "/dev/mem";
	try {
		SmBios smbios(path);
		cout << &smbios << endl;
	} catch (runtime_error e) {
		cout << e.what() << endl;
	        return 1;
	}
}
