#pragma once
#include <string>
#include "Collector.hpp"

class OsCollector : Collector {
public:
	std::string getKernel();
private:
	std::string kernel;
};
