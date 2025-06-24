// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB Utility Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  LogLevel.cpp

   File Description  :  Implementation of the portable page size functions.

   Revision History  :  1998-04-08 --- Creation.
                           Michael L. Brock
                        2023-01-05 --- Migration to C++ MlbDev2/Utility.
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

#include <Logger/LogLevel.hpp>

#include <Utility/AnyToString.hpp>
#include <Utility/ToStringRadix.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

namespace {

// ////////////////////////////////////////////////////////////////////////////
const char         *LogTextList[] = {
	"LITERAL  ",
	"SPAM     ",
	"MINUTIAE ",
	"DEBUG    ",
	"DETAIL   ",
	"INFO     ",
	"NOTICE   ",
	"WARNING  ",
	"ERROR    ",
	"CRITICAL ",
	"ALERT    ",
	"EMERGENCY",
	"FATAL    "
};
const std::size_t   LogTextCount  =
	sizeof(LogTextList) / sizeof(LogTextList[0]);
// ----------------------------------------------------------------------------
const char         *LogTextListSimple[LogTextCount] = {
	"LITERAL",
	"SPAM",
	"MINUTIAE",
	"DEBUG",
	"DETAIL",
	"INFO",
	"NOTICE",
	"WARNING",
	"ERROR",
	"CRITICAL",
	"ALERT",
	"EMERGENCY",
	"FATAL"
};
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
const char *ConvertLogLevelToTextRaw(LogLevel log_level)
{
	return(((static_cast<int>(log_level) < 0) ||
		(static_cast<unsigned int>(log_level) >= LogTextCount)) ? "*********" :
		LogTextList[static_cast<int>(log_level)]);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &ConvertLogLevelToText(LogLevel log_level,
	std::string &out_string)
{
	return(out_string.assign(ConvertLogLevelToTextRaw(log_level)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string ConvertLogLevelToText(LogLevel log_level)
{
	return(ConvertLogLevelToTextRaw(log_level));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const char *ConvertLogLevelToTextSimpleRaw(LogLevel log_level)
{
	return(((static_cast<int>(log_level) < 0) ||
		(static_cast<unsigned int>(log_level) >= LogTextCount)) ? "*********" :
		LogTextListSimple[static_cast<int>(log_level)]);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &ConvertLogLevelToTextSimple(LogLevel log_level,
	std::string &out_string)
{
	return(out_string.assign(ConvertLogLevelToTextSimpleRaw(log_level)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string ConvertLogLevelToTextSimple(LogLevel log_level)
{
	return(ConvertLogLevelToTextSimpleRaw(log_level));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogLevel CheckLogLevel(LogLevel log_level)
{
	if ((log_level < LogLevel_Minimum) || (log_level > LogLevel_Maximum))
		throw std::invalid_argument("Invalid log level encountered (" +
			AnyToString(log_level) + ") --- permissible values are from ('" +
			ConvertLogLevelToTextSimple(LogLevel_Minimum) + "' = " +
			AnyToString(LogLevel_Minimum) + ") to ('" +
			ConvertLogLevelToTextSimple(LogLevel_Maximum) + "' = " +
			AnyToString(LogLevel_Maximum) + "), inclusive.");

	return(log_level);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogLevelPair LogLevelFlagsToLevels(LogLevelFlag log_level_flags)
{
	unsigned int tmp_flags =
		static_cast<unsigned int>(log_level_flags) & LogFlag_Mask;

	if (!tmp_flags)
		throw std::invalid_argument("Invalid log level flags parameter (0x" +
			ToStringHex(log_level_flags) + " = " +
			AnyToString(log_level_flags) + ").");

	bool         low_done_flag = false;
	LogLevelPair tmp_pair(LogLevel_Minimum, LogLevel_Minimum);

	while (tmp_flags) {
		if (!low_done_flag) {
			if (tmp_flags & 1)
				low_done_flag = true;
			else
   			tmp_pair.first =
					static_cast<LogLevel>(static_cast<int>(tmp_pair.first) + 1);
		}
		if (tmp_flags == 1)
			break;
   	tmp_pair.second =
			static_cast<LogLevel>(static_cast<int>(tmp_pair.second) + 1);
		tmp_flags >>= 1;
	}

	return(tmp_pair);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::vector<std::string> &GetLogLevelNameList(
	std::vector<std::string> &name_list, bool simple_flag)
{
	std::vector<std::string> tmp_name_list;

	for (std::size_t enum_idx = 0; enum_idx < LogTextCount; ++enum_idx)
		tmp_name_list.push_back((simple_flag) ? LogTextListSimple[enum_idx] :
			LogTextList[enum_idx]);

	name_list.swap(tmp_name_list);

	return(name_list);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::vector<std::string> GetLogLevelNameList(bool simple_flag)
{
	std::vector<std::string> name_list;

	return(GetLogLevelNameList(name_list, simple_flag));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::vector<std::string> &GetLogLevelNameListSimple(std::vector<std::string>&name_list)
{
	return(GetLogLevelNameList(name_list, true));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::vector<std::string> GetLogLevelNameListSimple()
{
	std::vector<std::string> name_list;

	return(GetLogLevelNameListSimple(name_list));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

