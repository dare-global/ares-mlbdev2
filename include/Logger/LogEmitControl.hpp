// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  LogEmitControl.hpp

   File Description  :  Definition of the LogEmitControl struct.

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

#ifndef HH__MLB__Utility__Utility__LogEmitControl_hpp__HH

#define HH__MLB__Utility__Utility__LogEmitControl_hpp__HH   1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Logger/LogLevel.hpp>

#include <Utility/TimeSpec.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
const std::size_t LogLineLeaderLength = Length_TimeSpec + 1 +
	LogLevelTextMaxLength + 1 + 10 + 2;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
enum LogFlag {
	None         = 0x0000,
	LogLocalTime = 0x0001,
	LogBothTimes = 0x0002,
	LogZeroTime  = 0x0004,
	LogZeroTid   = 0x0008,
	Default      = 0x0000
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct API_UTILITY LogEmitControl {
	//	Constructor for formatted log lines...
	LogEmitControl(LogFlag log_flags, LogLevelFlag log_level_screen,
		LogLevelFlag log_level_persistent, const TimeSpec &line_start_time,
		LogLevel log_level, LogLevelFlag log_level_flag,
		const std::string &line_buffer);
	//	Constructor for literal log lines...
	LogEmitControl(LogFlag log_flags, LogLevelFlag log_level_screen,
		LogLevelFlag log_level_persistent, LogLevel log_level,
		LogLevelFlag log_level_flag);

	unsigned int       GetLeaderLength() const;
	const char        *GetLeaderPtr() const;
	const char        *GetNextLine(unsigned int &line_length) const;
	void               ResetLines() const;
	void               UpdateTime() const;

	bool               ShouldLogScreen() const;
	bool               ShouldLogPersistent() const;
	LogFlag            GetLogFlags() const;
	const TimeSpec    &GetLogStartTime() const;
	LogLevel           GetLogLevel() const;
	const std::string &GetLogMessage() const;

	LogFlag               log_flags_;
	LogLevelFlag          log_level_screen_;
	LogLevelFlag          log_level_persistent_;
	TimeSpec              line_start_time_;
	LogLevel              log_level_;
	LogLevelFlag          log_level_flag_;
	std::string           line_buffer_empty_;
	const std::string    &line_buffer_;
	mutable char          line_leader_[LogLineLeaderLength + 1];
	mutable unsigned int  this_line_offset_;

private:
	LogEmitControl(const LogEmitControl &) = delete;
	LogEmitControl & operator = (const LogEmitControl &) = delete;
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__Utility__LogEmitControl_hpp__HH

