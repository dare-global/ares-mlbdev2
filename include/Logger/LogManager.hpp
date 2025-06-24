// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  LogManager.hpp

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

#ifndef HH__MLB__Utility__LogManager_hpp__HH

#define HH__MLB__Utility__LogManager_hpp__HH 1

// ////////////////////////////////////////////////////////////////////////////
/**
   \file    LogManager.hpp

   \brief   The header file for a core LogManager logic.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Logger/LogHandlerConsole.hpp>

#include <Utility/ThreadId.hpp>        // CODE NOTE: Needed by LogStream.hpp ONLY.

#include <fstream>
#include <iomanip>
#include <map>
#include <set>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// /////////////////////////////////////////////////////////////////////////////
class API_UTILITY LogManager {
public:
	explicit LogManager(LogFlag log_flags = Default,
		LogLevel min_log_level_screen = LogLevel_Info,
		LogLevel max_log_level_screen = LogLevel_Fatal,
		LogLevel min_log_level_persistent = LogLevel_Spam,
		LogLevel max_log_level_persistent = LogLevel_Fatal);

	explicit LogManager(LogHandlerPtr log_handler_ptr,
		LogFlag log_flags = Default,
		LogLevel min_log_level_screen = LogLevel_Info,
		LogLevel max_log_level_screen = LogLevel_Fatal,
		LogLevel min_log_level_persistent = LogLevel_Spam,
		LogLevel max_log_level_persistent = LogLevel_Fatal);

	~LogManager();

	LogHandlerPtr HandlerInstall(LogHandlerPtr log_handler_ptr);
	LogHandlerPtr HandlerRemove();
	LogHandlerPtr GetHandlerPtr();

	LogLevelPair GetLogLevelConsole() const;
	LogLevelPair GetLogLevelFile() const;

	LogLevelPair SetLogLevelConsole(LogLevel min_log_level,
		LogLevel max_log_level = LogLevel_Maximum);
	LogLevelPair SetLogLevelFile(LogLevel min_log_level,
		LogLevel max_log_level = LogLevel_Maximum);

	void SetLogLevelConsoleAll();
	void SetLogLevelFileAll();

	void EmitLine(const TimeSpec &line_start_time, LogLevel log_level,
		const std::string &line_buffer);
	void EmitLine(const std::string &line_buffer,
		LogLevel log_level = LogLevel_Info);
	void EmitLiteral(const std::string &literal_string);
	void EmitLiteral(unsigned int literal_length, const char *literal_ptr);
	void EmitLiteral(LogLevel log_level, const std::string &literal_string);
	void EmitLiteral(LogLevel log_level, unsigned int literal_length,
		const char *literal_ptr);

	//	Public to provide speed by permitting access to a pointer to will be
	//	const memory on most systems. Don't ever try to write through the
	//	returned pointer!
 	static const char *LogLevelToTextRaw(LogLevel log_level) {
		return(ConvertLogLevelToTextRaw(log_level));
	}

	static std::string LogLevelToText(LogLevel log_level) {
		return(LogLevelToTextRaw(log_level));
	}

private:
	LogHandlerPtr log_handler_ptr_;
	LogFlag       log_flags_;
	LogLevelFlag  log_level_screen_;
	LogLevelFlag  log_level_persistent_;
	LogLock       the_lock_;

	static LogLevelFlag GetLogLevelMask(LogLevel min_level, LogLevel max_level);

	LogManager(const LogManager &) = delete;
	LogManager & operator = (const LogManager &) = delete;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class API_UTILITY ThreadStreamBuffer : public std::streambuf {
public:
	ThreadStreamBuffer(LogManager &manager_ref, LogLevel log_level)
		:std::streambuf()
		,last_used_time_()
		,manager_ref_(manager_ref)
		,log_level_(log_level)
		,line_start_time_()
		,line_buffer_()
		,sep_buffer_()
	 {
		setp(0, 0);
      setg(0, 0, 0);
	}

	~ThreadStreamBuffer() {
		Synchronize();
	}

	void Synchronize(bool force_line_flag = false) {
		sync(force_line_flag);
	}
	void PutChar(int datum) {
		put_char(datum);
	}
	void PutString(const std::string &datum) {
		std::string::size_type string_length = datum.size();
		std::string::size_type string_index  = 0;
		while (string_index < string_length)
         put_char(datum[string_index++]);
	}
	void PutLiteral(unsigned int literal_length, const char *literal_string) {
		Synchronize();
		manager_ref_.EmitLiteral(log_level_, literal_length, literal_string);
	}

	void LogSeparator(char sep_char = '*', unsigned int sep_length = 80)
	{
		unsigned int total_sep_length = LogLineLeaderLength + sep_length;

		if (sep_buffer_.empty() || (total_sep_length != sep_buffer_.size()) ||
			(sep_buffer_[0] != sep_char)) {
			sep_buffer_.assign(total_sep_length, sep_char);
			sep_buffer_[Length_TimeSpec]                                          = ' ';
			sep_buffer_[Length_TimeSpec + 1 + LogLevelTextMaxLength]              = ' ';
			sep_buffer_[Length_TimeSpec + 1 + LogLevelTextMaxLength + 1 + 10 + 1] = ' ';
		}

		PutLiteral(total_sep_length, sep_buffer_.c_str());
	}

protected:
	std::streambuf::int_type overflow(int c) {
      if (c != EOF)
  			put_char(c);
		return(0);
	}

	int sync() {
		return(sync(false));
	}
	int sync(bool force_line_flag) {
		put_buffer(force_line_flag);
		return(0);
	}

private:

	TimeVal      last_used_time_;
	LogManager  &manager_ref_;
	LogLevel     log_level_;
	TimeSpec     line_start_time_;
	std::string  line_buffer_;
	std::string  sep_buffer_;

	void put_char(int chr) {
		if (line_buffer_.empty())
			line_start_time_ = TimeSpec();
		if (chr == '\n')
			put_buffer(true);
		else {
			line_buffer_    += static_cast<char>(chr);
			last_used_time_  = TimeVal();
		}
	}

	void put_buffer(bool force_flag) {
		if (force_flag || (!line_buffer_.empty())) {
			if (log_level_ == LogLevel_Literal)
				manager_ref_.EmitLiteral(log_level_, line_buffer_);
			else
				manager_ref_.EmitLine(line_start_time_, log_level_, line_buffer_);
			line_buffer_.clear();
		}
		last_used_time_ = TimeVal();
	}

	ThreadStreamBuffer(const ThreadStreamBuffer &) = delete;
	ThreadStreamBuffer & operator = (const ThreadStreamBuffer &) = delete;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
typedef LogSPtr<ThreadStreamBuffer> ThreadStreamBufferPtr;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class ThreadStream;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class ThreadStreamBufferPtrHolder
{
	friend class ThreadStream;

	ThreadStreamBufferPtrHolder(LogManager &manager_ref, LogLevel log_level)
		:buffer_ptr_(
			std::make_shared<ThreadStreamBuffer>(manager_ref, log_level))
	{
	}

	ThreadStreamBufferPtrHolder(ThreadStreamBufferPtr buffer_ptr)
		:buffer_ptr_(buffer_ptr)
	{
	}

	ThreadStreamBufferPtr buffer_ptr_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class API_UTILITY ThreadStream :
	private ThreadStreamBufferPtrHolder,
	public std::ostream
{
public:
	ThreadStream(LogManager &manager_ref, LogLevel log_level,
		ThreadStreamBufferPtr buffer_ptr)
		:ThreadStreamBufferPtrHolder(buffer_ptr)
		,std::ostream(buffer_ptr_.get())
		,manager_ref_(manager_ref)
		,log_level_(log_level)
//		,buffer_ptr_(buffer_ptr)
	{
	}

	ThreadStream(LogManager &manager_ref, LogLevel log_level)
		:ThreadStreamBufferPtrHolder(manager_ref, log_level)
		,std::ostream(buffer_ptr_.get())
		,manager_ref_(manager_ref)
		,log_level_(log_level)
//		,buffer_ptr_(buffer_ptr)
	{
	}

	~ThreadStream() {
		buffer_ptr_->Synchronize();
	}

	ThreadStreamBufferPtr  GetBufferPtrRef() {
		return(buffer_ptr_);
	}

	//	Not truly necessary to keep a local copy of of the manager reference and
	//	the log level, but they were useful during debugging...
	LogManager            &manager_ref_;
	LogLevel               log_level_;
//	ThreadStreamBufferPtr  buffer_ptr_;

private:
	ThreadStream(const ThreadStream &) = delete;
	ThreadStream & operator = (const ThreadStream &) = delete;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class API_UTILITY LogStream : public std::ostream {
public:
	LogStream(LogManager &manager_ref, LogLevel log_level)
		:std::ostream(new ThreadStreamBuffer(manager_ref, log_level))
		,manager_ref_(manager_ref)
		,log_level_(log_level)
		,thread_stream_map_() {
	}
	~LogStream();

	LogStream & operator << (std::ostream & (*pfn)(std::ostream &)) {
		*GetThreadStream() << pfn;
		return(*this);
	}
	LogStream & operator << (std::ios_base & (*pfn)(std::ios_base &)) {
		*GetThreadStream() << pfn;
		return(*this);
	}
	LogStream & operator << (std::ios & (*pfn)(std::ios &)) {
		*GetThreadStream() << pfn;
		return(*this);
	}
	template <typename DataType> LogStream & operator << (
		const DataType &datum) {
		*GetThreadStream() << datum;
		return(*this);
	}
	LogStream & operator << (const std::string &datum) {
		*GetThreadStream() << datum.c_str();
		return(*this);
	}
/*
	LogStream & operator << (const char *datum) {
		*GetThreadStream() << datum;
		return(*this);
	}
*/
/*
	LogStream & operator << (unsigned int datum) {
		*GetThreadStream() << datum;
		return(*this);
	}
*/

	void LogLiteral(const std::string &literal_string) {
		LogLiteral(static_cast<unsigned int>(literal_string.size()),
			literal_string.c_str());
	}
	void LogLiteral(const char *literal_string) {
		LogLiteral((literal_string == NULL) ? 0 :
			static_cast<unsigned int>(strlen(literal_string)), literal_string);
	}
	void LogLiteral(unsigned int literal_length, const char *literal_string) {
		GetThreadStream()->GetBufferPtrRef()->PutLiteral(literal_length,
			literal_string);
	}
	void LogSeparator(char sep_char = '*', unsigned int text_length = 80);

	void LogToLevel(LogLevel log_level, const std::string &log_text) {
		ThreadStreamBufferPtr buffer_ptr(
									new ThreadStreamBuffer(manager_ref_, log_level));
		ThreadStream tmp_stream(manager_ref_, log_level, buffer_ptr);
		tmp_stream << log_text;
	}

private:
	typedef LogSPtr<ThreadStream>               ThreadStreamPtr;
	typedef std::map<ThreadId, ThreadStreamPtr> ThreadStreamMap;
	typedef ThreadStreamMap::iterator           ThreadStreamMapIter;

	LogManager      &manager_ref_;
	LogLevel         log_level_;
	ThreadStreamMap  thread_stream_map_;
	LogLock          the_lock_;

	ThreadStreamPtr GetThreadStream();

	LogStream(const LogStream &) = delete;
	LogStream & operator = (const LogStream &) = delete;
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

// ////////////////////////////////////////////////////////////////////////////
	/**
		Use once in any collective executable unit... That is, only one \e .cpp
		should use this macro in any program. A good place to do so is in the
		translation unit which contains the \c main function.

		If these names need to be visibile in other translation units besides the
		one which contains \c main, the #LogManagerMacroDeclaration macro should
		be used instead.
	*/
#define LogManagerMacroDefinition(export_spec)																			\
	export_spec MLB::Utility::LogManager MyLogManager;																	\
	export_spec MLB::Utility::LogStream  LogLiteral(MyLogManager,   MLB::Utility::LogLevel_Literal);	\
	export_spec MLB::Utility::LogStream  LogSpam(MyLogManager,      MLB::Utility::LogLevel_Spam);		\
	export_spec MLB::Utility::LogStream  LogMinutiae(MyLogManager,  MLB::Utility::LogLevel_Minutiae);	\
	export_spec MLB::Utility::LogStream  LogDebug(MyLogManager,     MLB::Utility::LogLevel_Debug);		\
	export_spec MLB::Utility::LogStream  LogDetail(MyLogManager,    MLB::Utility::LogLevel_Detail);		\
	export_spec MLB::Utility::LogStream  LogInfo(MyLogManager,      MLB::Utility::LogLevel_Info);		\
	export_spec MLB::Utility::LogStream  LogNotice(MyLogManager,    MLB::Utility::LogLevel_Notice);		\
	export_spec MLB::Utility::LogStream  LogWarning(MyLogManager,   MLB::Utility::LogLevel_Warning);	\
	export_spec MLB::Utility::LogStream  LogError(MyLogManager,     MLB::Utility::LogLevel_Error);		\
	export_spec MLB::Utility::LogStream  LogCritical(MyLogManager,  MLB::Utility::LogLevel_Critical);	\
	export_spec MLB::Utility::LogStream  LogAlert(MyLogManager,     MLB::Utility::LogLevel_Alert);		\
	export_spec MLB::Utility::LogStream  LogEmergency(MyLogManager, MLB::Utility::LogLevel_Emergency);	\
	export_spec MLB::Utility::LogStream  LogFatal(MyLogManager,     MLB::Utility::LogLevel_Fatal);

	/**
		Use wherever logging is needed...
	*/
#define LogManagerMacroDeclaration(import_spec)						\
	extern import_spec MLB::Utility::LogManager MyLogManager;	\
	extern import_spec MLB::Utility::LogStream  LogLiteral;		\
	extern import_spec MLB::Utility::LogStream  LogSpam;			\
	extern import_spec MLB::Utility::LogStream  LogMinutiae;		\
	extern import_spec MLB::Utility::LogStream  LogDebug;			\
	extern import_spec MLB::Utility::LogStream  LogDetail;		\
	extern import_spec MLB::Utility::LogStream  LogInfo;			\
	extern import_spec MLB::Utility::LogStream  LogNotice;		\
	extern import_spec MLB::Utility::LogStream  LogWarning;		\
	extern import_spec MLB::Utility::LogStream  LogError;			\
	extern import_spec MLB::Utility::LogStream  LogCritical;		\
	extern import_spec MLB::Utility::LogStream  LogAlert;			\
	extern import_spec MLB::Utility::LogStream  LogEmergency;	\
	extern import_spec MLB::Utility::LogStream  LogFatal;
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifndef HH__MLB__Utility__LogManager_hpp__HH

