// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB Utility Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  LogManager.cpp

   File Description  :  Implementation of logging management.

   Revision History  :  1993-10-02 --- Creation of predecessor 'mlog' facility.
                           Michael L. Brock
                        2005-01-02 --- New ostream-based log model.
                           Michael L. Brock
                        2023-01-05 --- Migration to C++ MlbDev2/Utility.
                           Michael L. Brock

      Copyright Michael L. Brock 1993 - 2023.
      Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
      http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Logger/LogManager.hpp>

#include <Utility/ExceptionRethrow.hpp>

#include <fstream>
#include <iostream>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
LogManager::LogManager(LogFlag log_flags,
	LogLevel min_log_level_screen, LogLevel max_log_level_screen,
	LogLevel min_log_level_persistent, LogLevel max_log_level_persistent)
	:log_handler_ptr_()
	,log_flags_(log_flags)
	,log_level_screen_(GetLogLevelMask(min_log_level_screen,
		max_log_level_screen))
	,log_level_persistent_(GetLogLevelMask(min_log_level_persistent,
		max_log_level_persistent))
	,the_lock_()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogManager::LogManager(LogHandlerPtr log_handler_ptr,
	LogFlag log_flags, LogLevel min_log_level_screen,
	LogLevel max_log_level_screen, LogLevel min_log_level_persistent,
	LogLevel max_log_level_persistent)
	:log_handler_ptr_()
	,log_flags_(log_flags)
	,log_level_screen_(GetLogLevelMask(min_log_level_screen,
		max_log_level_screen))
	,log_level_persistent_(GetLogLevelMask(min_log_level_persistent,
		max_log_level_persistent))
	,the_lock_()
{
	HandlerInstall(log_handler_ptr);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogManager::~LogManager()
{
	HandlerRemove();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogHandlerPtr LogManager::HandlerInstall(LogHandlerPtr log_handler_ptr)
{
	LogLockScoped my_lock(the_lock_);
	LogHandlerPtr old_log_handler_ptr = log_handler_ptr_;

	if (log_handler_ptr_ != NULL)
		log_handler_ptr_->RemoveHandler();

	log_handler_ptr_ = log_handler_ptr;

	if (log_handler_ptr_ != NULL)
		log_handler_ptr_->InstallHandler();

	return(old_log_handler_ptr);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogHandlerPtr LogManager::HandlerRemove()
{
	return(HandlerInstall(LogHandlerPtr()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogHandlerPtr LogManager::GetHandlerPtr()
{
	LogLockScoped my_lock(the_lock_);

	return(log_handler_ptr_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogLevelPair LogManager::GetLogLevelConsole() const
{
	return(LogLevelFlagsToLevels(log_level_screen_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogLevelPair LogManager::GetLogLevelFile() const
{
	return(LogLevelFlagsToLevels(log_level_persistent_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogLevelPair LogManager::SetLogLevelConsole(LogLevel min_log_level,
	LogLevel max_log_level)
{
	LogLockScoped my_lock(the_lock_);
	LogLevelPair  old_levels(GetLogLevelConsole());

	log_level_screen_ = GetLogLevelMask(min_log_level, max_log_level);

	return(old_levels);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogLevelPair LogManager::SetLogLevelFile(LogLevel min_log_level,
	LogLevel max_log_level)
{
	LogLockScoped my_lock(the_lock_);
	LogLevelPair  old_levels(GetLogLevelFile());

	log_level_persistent_ = GetLogLevelMask(min_log_level, max_log_level);

	return(old_levels);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LogManager::SetLogLevelConsoleAll()
{
	SetLogLevelConsole(LogLevel_Minimum, LogLevel_Maximum);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LogManager::SetLogLevelFileAll()
{
	SetLogLevelFile(LogLevel_Minimum, LogLevel_Maximum);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LogManager::EmitLine(const TimeSpec &line_start_time, LogLevel log_level,
	const std::string &line_buffer)
{
	LogLevelFlag log_level_flag = static_cast<LogLevelFlag>
		((1 << log_level) & LogFlag_Mask);
	LogLockScoped my_lock(the_lock_);

	if ((log_handler_ptr_ != NULL) && ((log_level_flag & log_level_screen_) ||
		(log_level_flag & log_level_persistent_))) {
		LogEmitControl emit_ctl(log_flags_, log_level_screen_,
			log_level_persistent_, line_start_time, log_level, log_level_flag,
			line_buffer);
/*
		Can't early-release std::lock_guard.

		But should have taken a copy of log_handler_ptr_ before the early
		release and used that anyway.

		To be re-factored into something like:
		LogHandlerPtr tmp_log_handler_ptr = log_handler_ptr_;
		my_lock.unlock();
		log_handler_ptr_->EmitLine(emit_ctl);
*/
		log_handler_ptr_->EmitLine(emit_ctl);
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LogManager::EmitLine(const std::string &line_buffer,
	LogLevel log_level)
{
	EmitLine(TimeSpec(), log_level, line_buffer);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LogManager::EmitLiteral(const std::string &literal_string)
{
	EmitLiteral(static_cast<unsigned int>(literal_string.size()),
		literal_string.c_str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LogManager::EmitLiteral(unsigned int literal_length,
	const char *literal_ptr)
{
	literal_ptr = (literal_ptr == NULL) ? "" : literal_ptr;

	LogLockScoped my_lock(the_lock_);

	if (log_handler_ptr_ != NULL)
		log_handler_ptr_->EmitLiteral(literal_length, literal_ptr);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LogManager::EmitLiteral(LogLevel log_level,
	const std::string &literal_string)
{
	EmitLiteral(log_level, static_cast<unsigned int>(literal_string.size()),
		literal_string.c_str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LogManager::EmitLiteral(LogLevel log_level, unsigned int literal_length,
	const char *literal_ptr)
{
	literal_ptr = (literal_ptr == NULL) ? "" : literal_ptr;

	LogLevelFlag log_level_flag = static_cast<LogLevelFlag>
		((1 << log_level) & LogFlag_Mask);
	LogLockScoped my_lock(the_lock_);

	if ((log_handler_ptr_ != NULL) && ((log_level_flag & log_level_screen_) ||
		(log_level_flag & log_level_persistent_))) {
		LogEmitControl emit_ctl(log_flags_, log_level_screen_,
			log_level_persistent_, log_level, log_level_flag);
		log_handler_ptr_->EmitLiteral(emit_ctl, literal_length, literal_ptr);
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogLevelFlag LogManager::GetLogLevelMask(LogLevel min_level, LogLevel max_level)
{
	min_level = std::max(min_level, LogLevel_Minimum);
	max_level = std::min(max_level, LogLevel_Maximum);

	if (min_level > max_level)
		std::swap(min_level, max_level);

	unsigned int level_flags = 0;
	while (min_level <= max_level) {
		level_flags |= 1 << static_cast<unsigned int>(min_level);
		min_level    =
			static_cast<LogLevel>(static_cast<unsigned int>(min_level) + 1);
	}

	return(static_cast<LogLevelFlag>(level_flags));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ****************************************************************************
// ****************************************************************************
// ****************************************************************************
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogStream::~LogStream()
{
	try {
		LogLockScoped my_lock(the_lock_);
		thread_stream_map_.clear();
	}
	catch (const std::exception &) {
		;	// TLILB
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LogStream::LogSeparator(char sep_char, unsigned int text_length)
{
	GetThreadStream()->GetBufferPtrRef()->LogSeparator(sep_char, text_length);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogStream::ThreadStreamPtr LogStream::GetThreadStream()
{
	LogLockScoped       my_lock(the_lock_);
	ThreadId            thread_id(CurrentThreadId());
	ThreadStreamMapIter iter_f(thread_stream_map_.find(thread_id));

	if (iter_f != thread_stream_map_.end())
		return(iter_f->second);

	ThreadStreamBufferPtr buffer_ptr(
									new ThreadStreamBuffer(manager_ref_, log_level_));
	ThreadStreamPtr       ostream_ptr(
									new ThreadStream(manager_ref_, log_level_,
									buffer_ptr));

	thread_stream_map_[thread_id] = ostream_ptr;

	return(ostream_ptr);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

// ////////////////////////////////////////////////////////////////////////////
// ****************************************************************************
// ****************************************************************************
// ****************************************************************************
// ////////////////////////////////////////////////////////////////////////////

#ifdef TEST_MAIN

#include <Logger/LogHandlerFile.hpp>
#include <Logger/LogManager.hpp>
#include <Logger/LogTestSupport.hpp>

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	using namespace MLB::Utility;

	int return_code = EXIT_SUCCESS;

	try {
		//	Create a LogHandlerFile...
/*
		LogHandlerPtr my_log_handler(
			new LogHandlerFile("TestLogFile.VERSION_OLD_001.log"));
*/
		LogHandlerPtr my_log_handler(
			new LogHandlerFile(TEST_GetLogFileName("LogManager")));
		TEST_TestControl(my_log_handler, 0, 0, 0, 0);
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << std::endl << "ERROR: " << except.what() <<
			std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

