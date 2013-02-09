#include "SmElement.h"

using namespace std;


SmElement *SmElementFactory::create(u8 *ptr)
{
	u8 type = ptr[0];

	switch (type) {
	case 0: //bios
		return new Bios(ptr);
		break;
	case 1: //system
		return new System(ptr);
		break;
	case 2: //baseboard
		return new Baseboard(ptr);
		break;
	default:
		printf("factory: unknown type %d\n", type);
		return nullptr;
	}
	return nullptr;  // only for testing
};
