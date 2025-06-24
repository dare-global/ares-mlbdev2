// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  LogLevel.hpp

   File Description  :  Definitions of the logging level enumerations.

   Revision History  :  2005-01-02 --- Creation.
                           Michael L. Brock
                        2023-01-05 --- Migration to C++ MlbDev2/Utility.
                           Michael L. Brock

      Copyright Michael L. Brock 2005 - 2023.
      Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
      http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__Utility__Utility__LogLevel_hpp__HH

#define HH__MLB__Utility__Utility__LogLevel_hpp__HH   1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Logger/Logger.hpp>

#include <string>
#include <utility>
#include <vector>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
enum LogLevel {
	LogLevel_Literal   =  0,
	LogLevel_Spam      =  1,
	LogLevel_Minutiae  =  2,
	LogLevel_Debug     =  3,
	LogLevel_Detail    =  4,
	LogLevel_Info      =  5,
	LogLevel_Notice    =  6,
	LogLevel_Warning   =  7,
	LogLevel_Error     =  8,
	LogLevel_Critical  =  9,
	LogLevel_Alert     = 10,
	LogLevel_Emergency = 11,
	LogLevel_Fatal     = 12
};
//	----------------------------------------------------------------------------
enum LogLevelFlag {
	LogFlag_Literal   = (1 << LogLevel_Literal  ),
	LogFlag_Spam      = (1 << LogLevel_Spam     ),
	LogFlag_Minutiae  = (1 << LogLevel_Minutiae ),
	LogFlag_Debug     = (1 << LogLevel_Debug    ),
	LogFlag_Detail    = (1 << LogLevel_Detail   ),
	LogFlag_Info      = (1 << LogLevel_Info     ),
	LogFlag_Notice    = (1 << LogLevel_Notice   ),
	LogFlag_Warning   = (1 << LogLevel_Warning  ),
	LogFlag_Error     = (1 << LogLevel_Error    ),
	LogFlag_Critical  = (1 << LogLevel_Critical ),
	LogFlag_Alert     = (1 << LogLevel_Alert    ),
	LogFlag_Emergency = (1 << LogLevel_Emergency),
	LogFlag_Fatal     = (1 << LogLevel_Fatal    ),
	LogFlag_Mask      =	(1 << LogLevel_Literal  ) |
								(1 << LogLevel_Spam     ) |
								(1 << LogLevel_Minutiae ) |
								(1 << LogLevel_Debug    ) |
								(1 << LogLevel_Detail   ) |
								(1 << LogLevel_Info     ) |
								(1 << LogLevel_Notice   ) |
								(1 << LogLevel_Warning  ) |
								(1 << LogLevel_Error    ) |
								(1 << LogLevel_Critical ) |
								(1 << LogLevel_Alert    ) |
								(1 << LogLevel_Emergency) |
								(1 << LogLevel_Fatal    )
};
//	----------------------------------------------------------------------------
const LogLevel LogLevel_Minimum = LogLevel_Literal;
const LogLevel LogLevel_Maximum = LogLevel_Fatal;
const LogLevel LogLevel_Invalid = static_cast<LogLevel>(-1);
// ////////////////////////////////////////////////////////////////////////////

#if 0
// ////////////////////////////////////////////////////////////////////////////
enum class LogLevel {
	LogLevel_Invalid   = -1;
	LogLevel_Literal   =  0,
	LogLevel_Spam      =  1,
	LogLevel_Minutiae  =  2,
	LogLevel_Debug     =  3,
	LogLevel_Detail    =  4,
	LogLevel_Info      =  5,
	LogLevel_Notice    =  6,
	LogLevel_Warning   =  7,
	LogLevel_Error     =  8,
	LogLevel_Critical  =  9,
	LogLevel_Alert     = 10,
	LogLevel_Emergency = 11,
	LogLevel_Fatal     = 12,
	LogLevel_Minimum   = LogLevel_Literal,
	LogLevel_Min       = LogLevel_Minimum,
	LogLevel_Maximum   = LogLevel_Fatal,
	LogLevel_Max       = LogLevel_Maximum
};
//	----------------------------------------------------------------------------
enum class LogLevelFlag {
	LogFlag_Literal   = (1 << LogLevel::LogLevel_Literal  ),
	LogFlag_Spam      = (1 << LogLevel::LogLevel_Spam     ),
	LogFlag_Minutiae  = (1 << LogLevel::LogLevel_Minutiae ),
	LogFlag_Debug     = (1 << LogLevel::LogLevel_Debug    ),
	LogFlag_Detail    = (1 << LogLevel::LogLevel_Detail   ),
	LogFlag_Info      = (1 << LogLevel::LogLevel_Info     ),
	LogFlag_Notice    = (1 << LogLevel::LogLevel_Notice   ),
	LogFlag_Warning   = (1 << LogLevel::LogLevel_Warning  ),
	LogFlag_Error     = (1 << LogLevel::LogLevel_Error    ),
	LogFlag_Critical  = (1 << LogLevel::LogLevel_Critical ),
	LogFlag_Alert     = (1 << LogLevel::LogLevel_Alert    ),
	LogFlag_Emergency = (1 << LogLevel::LogLevel_Emergency),
	LogFlag_Fatal     = (1 << LogLevel::LogLevel_Fatal    ),
	LogFlag_Mask      = (1 << LogLevel::LogLevel_Literal  ) |
							  (1 << LogLevel::LogLevel_Spam     ) |
							  (1 << LogLevel::LogLevel_Minutiae ) |
							  (1 << LogLevel::LogLevel_Debug    ) |
							  (1 << LogLevel::LogLevel_Detail   ) |
							  (1 << LogLevel::LogLevel_Info     ) |
							  (1 << LogLevel::LogLevel_Notice   ) |
							  (1 << LogLevel::LogLevel_Warning  ) |
							  (1 << LogLevel::LogLevel_Error    ) |
							  (1 << LogLevel::LogLevel_Critical ) |
							  (1 << LogLevel::LogLevel_Alert    ) |
							  (1 << LogLevel::LogLevel_Emergency) |
							  (1 << LogLevel::LogLevel_Fatal    )
};
// ////////////////////////////////////////////////////////////////////////////
#endif // #if 0

// ////////////////////////////////////////////////////////////////////////////
typedef std::pair<LogLevel, LogLevel> LogLevelPair;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const std::size_t LogLevelTextMaxLength = 9;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
API_UTILITY const char   *ConvertLogLevelToTextRaw(LogLevel log_level);
API_UTILITY std::string  &ConvertLogLevelToText(LogLevel log_level,
	std::string &out_string);
API_UTILITY std::string   ConvertLogLevelToText(LogLevel log_level);

API_UTILITY const char   *ConvertLogLevelToTextSimpleRaw(LogLevel log_level);
API_UTILITY std::string  &ConvertLogLevelToTextSimple(LogLevel log_level,
	std::string &out_string);
API_UTILITY std::string   ConvertLogLevelToTextSimple(LogLevel log_level);

API_UTILITY LogLevel      CheckLogLevel(LogLevel log_level);

API_UTILITY LogLevelPair  LogLevelFlagsToLevels(LogLevelFlag log_level_flags);

API_UTILITY std::vector<std::string> &GetLogLevelNameList(
	std::vector<std::string> &name_list, bool simple_flag = false);
API_UTILITY std::vector<std::string>  GetLogLevelNameList(
	bool simple_flag = false);
API_UTILITY std::vector<std::string> &GetLogLevelNameListSimple(
	std::vector<std::string> &name_list);
API_UTILITY std::vector<std::string>  GetLogLevelNameListSimple();
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__Utility__LogLevel_hpp__HH

