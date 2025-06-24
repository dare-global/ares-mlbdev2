// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB Utility Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  TimeTM.cpp

   File Description  :  Implementation of the TimeTM class.

   Revision History  :  1998-04-08 --- Creation.
                           Michael L. Brock
                        2023-01-04 --- Migration to C++ MlbDev2/Utility.
                           Michael L. Brock

      Copyright Michael L. Brock 1998 - 2023.
      Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
      http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/TimeTM.hpp>

#include <Utility/AnyToString.hpp>
#include <Utility/nstrcpy.hpp>
#include <Utility/ToStringRadix.hpp>

#include <cstring>
#include <sstream>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

namespace {

// ////////////////////////////////////////////////////////////////////////////
void TimeT_To_TimeTM_ConvertError(const char *this_func_name,
	const char *lib_func_name, const time_t in_time, const struct tm *source_ptr,
	const struct tm *result_ptr)
{
	std::ostringstream o_str;

	o_str << "Invocation of method 'TimeTM::" << this_func_name <<
		"()' in an attempt to convert a 'time_t' with a value of " <<
		static_cast<long>(in_time) << ") to a 'TimeTM' failed because the "
		"Standard Library function '" << lib_func_name << "()' ";

	if (result_ptr == NULL)
		o_str << " returned a 'NULL' pointer.";
	else
		o_str << " returned a pointer value (0x" <<
			ToStringHex(result_ptr) << ") which was not equal to the passed "
			"parameter pointer value (0x" << ToStringHex(source_ptr) << ").";

	throw std::runtime_error(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
TimeTM::TimeTM()
{
	*this = Now();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
TimeTM::TimeTM(const tm &in_time)
	:tm(in_time)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
TimeTM::~TimeTM()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
char *TimeTM::AscTime(char *buffer) const
{
	return(::strcpy(buffer, asctime(this)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string TimeTM::AscTime() const
{
	char buffer[Length_TimeTM_AscTime + 1];

	return(ToString(buffer));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
char *TimeTM::AscTimeSane(char *buffer) const
{
	//	Necessary because asctime() places a line-feed at the string end.
	char tmp_buffer[Length_TimeTM_AscTime + 1];

	AscTime(tmp_buffer);

	tmp_buffer[Length_TimeTM_AscTimeSane] = '\0';

	return(::strcpy(buffer, tmp_buffer));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string TimeTM::AscTimeSane() const
{
	char buffer[Length_TimeTM_AscTimeSane + 1];

	return(AscTimeSane(buffer));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
TimeTM TimeTM::TimeUTC(const time_t in_time)
{
	if (in_time < static_cast<time_t>(0))
		throw std::invalid_argument("Invalid 'time_t' encountered in invocation "
			"of method TimeTM::TimeUTC() (value = " + AnyToString(in_time) +
			").");

	struct tm  out_time;
	struct tm *result_ptr;

#ifdef _Windows
	if ((result_ptr = gmtime(&in_time)) == NULL)
		TimeT_To_TimeTM_ConvertError("TimeUTC", "gmtime", in_time, NULL,
			result_ptr);
	out_time = *result_ptr;
#else
# if defined(__EXTENSIONS__) || defined(_REENTRANT) || defined(_POSIX_C_SOURCE)
	if (((result_ptr = gmtime_r(&in_time, &out_time)) == NULL) ||
		(result_ptr != &out_time))
		TimeT_To_TimeTM_ConvertError("TimeUTC", "gmtime_r", in_time, &out_time,
			result_ptr);
# else
	if ((result_ptr = gmtime(&in_time)) == NULL)
		TimeT_To_TimeTM_ConvertError("TimeUTC", "gmtime", in_time, NULL,
			result_ptr);
	out_time = *result_ptr;
# endif /* #if defined(__EXTENSIONS__ || _REENTRANT || _POSIX_C_SOURCE) */
#endif /* #ifdef _Windows */

	return(TimeTM(out_time));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
TimeTM TimeTM::TimeLocal(const time_t in_time)
{
	if (in_time < static_cast<time_t>(0))
		throw std::invalid_argument("Invalid 'time_t' encountered in invocation "
			"of method TimeTM::TimeLocal() (value = " + AnyToString(in_time) +
			").");

	struct tm  out_time;
	struct tm *result_ptr;

#ifdef _Windows
	if ((result_ptr = localtime(&in_time)) == NULL)
		TimeT_To_TimeTM_ConvertError("TimeLocal", "localtime", in_time, NULL,
			result_ptr);
	out_time = *result_ptr;
#else
# if defined(__EXTENSIONS__) || defined(_REENTRANT) || defined(_POSIX_C_SOURCE)
	if (((result_ptr = localtime_r(&in_time, &out_time)) == NULL) ||
		(result_ptr != &out_time))
		TimeT_To_TimeTM_ConvertError("TimeLocal", "localtime_r", in_time,
			&out_time, result_ptr);
# else
	if ((result_ptr = localtime(&in_time)) == NULL)
		TimeT_To_TimeTM_ConvertError("TimeLocal", "localtime", in_time, NULL,
			result_ptr);
	out_time = *result_ptr;
# endif /* #if defined(__EXTENSIONS__ || _REENTRANT || _POSIX_C_SOURCE) */
#endif /* #ifdef _Windows */

	return(TimeTM(out_time));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
char *TimeTM::ToString(char *buffer, std::size_t max_length) const
{
	if (max_length < Length_TimeTM) {
		char tmp_buffer[Length_TimeTM + 1];
		return(nstrcpy(buffer, ToString(tmp_buffer), max_length));
	}

	tm tm_data = *this;

	tm_data.tm_year  = (tm_data.tm_year + 1900) % 10000;
	tm_data.tm_mon   = (tm_data.tm_mon + 1) % 100;
	tm_data.tm_mday %= 100;
	tm_data.tm_hour %= 100;
	tm_data.tm_min  %= 100;
	tm_data.tm_sec  %= 100;

	buffer[0]        = ((char) ('0' + (tm_data.tm_year / 1000)));
	buffer[1]        = ((char) ('0' + ((tm_data.tm_year % 1000) / 100)));
	buffer[2]        = ((char) ('0' + ((tm_data.tm_year % 100) / 10)));
	buffer[3]        = ((char) ('0' + (tm_data.tm_year % 10)));
	buffer[4]        = '-';
	buffer[5]        = ((char) ('0' + (tm_data.tm_mon / 10)));
	buffer[6]        = ((char) ('0' + (tm_data.tm_mon % 10)));
	buffer[7]        = '-';
	buffer[8]        = ((char) ('0' + (tm_data.tm_mday / 10)));
	buffer[9]        = ((char) ('0' + (tm_data.tm_mday % 10)));
	buffer[10]       = ' ';
	buffer[11]       = ((char) ('0' + (tm_data.tm_hour / 10)));
	buffer[12]       = ((char) ('0' + (tm_data.tm_hour % 10)));
	buffer[13]       = ':';
	buffer[14]       = ((char) ('0' + (tm_data.tm_min / 10)));
	buffer[15]       = ((char) ('0' + (tm_data.tm_min % 10)));
	buffer[16]       = ':';
	buffer[17]       = ((char) ('0' + (tm_data.tm_sec / 10)));
	buffer[18]       = ((char) ('0' + (tm_data.tm_sec % 10)));
	buffer[19]       = '\0';

	return(buffer);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string TimeTM::ToString(std::size_t max_length) const
{
	char buffer[Length_TimeTM + 1];

	return(ToString(buffer, max_length));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &TimeTM::ToString(std::string &out_string,
	std::size_t max_length) const
{
	char buffer[Length_TimeTM + 1];

	return(out_string.assign(ToString(buffer, max_length)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
TimeTM TimeTM::Now()
{
	return(TimeUTC());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const TimeTM &datum)
{
	char tmp_buffer[Length_TimeTM + 1];

	o_str <<
		datum.ToString(tmp_buffer, sizeof(tmp_buffer) - 1);

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

