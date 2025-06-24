// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB Utility Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  LogEmitControl.cpp

   File Description  :  Implementation of the LogEmitControl struct.

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

#include <Logger/LogEmitControl.hpp>

#include <Utility/ThreadId.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
LogEmitControl::LogEmitControl(LogFlag log_flags, LogLevelFlag log_level_screen,
	LogLevelFlag log_level_persistent, const TimeSpec &line_start_time,
	LogLevel log_level, LogLevelFlag log_level_flag,
	const std::string &line_buffer)
	:log_flags_(log_flags)
	,log_level_screen_(log_level_screen)
	,log_level_persistent_(log_level_persistent)
	,line_start_time_(line_start_time)
	,log_level_(log_level)
	,log_level_flag_(log_level_flag)
	,line_buffer_empty_()
	,line_buffer_(line_buffer)
	,this_line_offset_(0)
{
	memcpy(line_leader_ + Length_TimeSpec + 1,
		 ConvertLogLevelToTextRaw(log_level), LogLevelTextMaxLength);
	line_leader_[Length_TimeSpec + 1 + LogLevelTextMaxLength] = ' ';
	//	Maximum length of a thread id is coerced to 10 characters...
	{
#if 0
		char tid_string[IntToString_MaxLen_Unsigned_10 + 1];
		IntToString(tid_string,
			static_cast<Native_UInt32>(CurrentThreadId() % 0xFFFFFFFF), 10, " ");
		memcpy(line_leader_ + Length_TimeSpec + 1 + LogLevelTextMaxLength + 1,
			tid_string + (IntToString_MaxLen_Unsigned_10 - 10), 10);
#else
		std::string tid(std::to_string(CurrentThreadId() % 0xFFFFFFFF));
		if (tid.size() > 10)
			tid = tid.substr(0, 9) + ">";
		else if (tid.size() < 10)
			tid = std::string(10 - tid.size(), ' ') + tid;
		memcpy(line_leader_ + Length_TimeSpec + 1 + LogLevelTextMaxLength + 1,
			tid.c_str(), 10);
#endif // #if 0
	}
	line_leader_[Length_TimeSpec + 1 + LogLevelTextMaxLength + 1 + 10] = ':';
	line_leader_[Length_TimeSpec + 1 + LogLevelTextMaxLength + 1 + 11] = ' ';
	line_leader_[Length_TimeSpec + 1 + LogLevelTextMaxLength + 1 + 12] = '\0';
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogEmitControl::LogEmitControl(LogFlag log_flags, LogLevelFlag log_level_screen,
	LogLevelFlag log_level_persistent, LogLevel log_level,
	LogLevelFlag log_level_flag)
	:log_flags_(log_flags)
	,log_level_screen_(log_level_screen)
	,log_level_persistent_(log_level_persistent)
	,line_start_time_(0, 0)
	,log_level_(log_level)
	,log_level_flag_(log_level_flag)
	,line_buffer_empty_()
	,line_buffer_(line_buffer_empty_)
	,this_line_offset_(0)
{
	line_leader_[0] = '\0';
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned int LogEmitControl::GetLeaderLength() const
{
	return(LogLineLeaderLength);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const char *LogEmitControl::GetLeaderPtr() const
{
	return(line_leader_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const char *LogEmitControl::GetNextLine(unsigned int &line_length) const
{
	if (this_line_offset_ > line_buffer_.size()) {
		line_length = 0;
		return(NULL);
	}
	else if ((!this_line_offset_) && line_buffer_.empty()) {
		this_line_offset_ = 1;
		line_length       = 0;
		return("");
	}

	const char             *ptr = line_buffer_.c_str() + this_line_offset_;
	std::string::size_type  next_end =
		line_buffer_.find('\n', this_line_offset_);

	if (next_end == std::string::npos) {
		line_length       = static_cast<unsigned int>(line_buffer_.size()) -
			this_line_offset_;
		this_line_offset_ = static_cast<unsigned int>(line_buffer_.size()) + 1;
	}
	else {
		line_length       = static_cast<unsigned int>(next_end) -
			this_line_offset_;
		this_line_offset_ = static_cast<unsigned int>(next_end) + 1;
	}

	return(ptr);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LogEmitControl::ResetLines() const
{
	this_line_offset_ = 0;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LogEmitControl::UpdateTime() const
{
	if (log_flags_ & LogZeroTime)
		::memcpy(line_leader_, "0000-00-00 00:00:00.000000000",
			Length_TimeSpec);
	else {
		if (log_flags_ & LogLocalTime)
			TimeSpec().ToStringLocal(line_leader_);
		else
			TimeSpec().ToString(line_leader_);
		line_leader_[Length_TimeSpec] = ' ';
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool LogEmitControl::ShouldLogScreen() const
{
	return((log_level_flag_ & log_level_screen_) != 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool LogEmitControl::ShouldLogPersistent() const
{
	return((log_level_flag_ & log_level_persistent_) != 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogFlag LogEmitControl::GetLogFlags() const
{
	return(log_flags_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const TimeSpec &LogEmitControl::GetLogStartTime() const
{
	return(line_start_time_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogLevel LogEmitControl::GetLogLevel() const
{
	return(log_level_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const std::string &LogEmitControl::GetLogMessage() const
{
	return(line_buffer_);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

