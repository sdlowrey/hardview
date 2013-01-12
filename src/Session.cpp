#include "Session.hpp"
using namespace boost::posix_time;

Session::Session()
{
	// not implemented or nothing to do?
};

void Session::begin()
{
	startTime = second_clock::local_time(); 
};

void Session::end()
{
	endTime = second_clock::local_time();
};

bool Session::isInProgress()
{
	return ! startTime.is_not_a_date_time(); 
};

