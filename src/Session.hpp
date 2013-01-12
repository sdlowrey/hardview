#pragma once
#include <boost/date_time.hpp>

class Session
{
public:
	Session();
	void begin();
	void end();
	bool isInProgress();
private:
	boost::posix_time::ptime startTime, endTime;
};
