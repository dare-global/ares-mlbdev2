// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB Utility Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  LogHandlerFile.cpp

   File Description  :  Implementation of the log handler file class.

   Revision History  :  1993-10-02 --- Creation of predecessor 'mlog' facility.
                           Michael L. Brock
                        2005-01-02 --- New ostream-based log model.
                           Michael L. Brock
                        2023-01-05 --- Migration to C++ MlbDev2/Utility.
                           Michael L. Brock

      Copyright Michael L. Brock 1993 - 2023
      Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
      http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Logger/LogHandlerFile.hpp>

#include <Utility/AnyToString.hpp>
#include <Utility/ExceptionRethrow.hpp>

#include <fstream>
#include <iostream>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
LogHandlerFile::LogHandlerFile()
	:LogHandler()
	,out_file_name_()
	,out_file_ptr_()
	,my_flags_(Default)
	,the_lock_()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogHandlerFile::LogHandlerFile(const char *file_name, LogHandlerFileFlag flags)
	:LogHandler()
	,out_file_name_()
	,out_file_ptr_()
	,my_flags_(flags)
	,the_lock_()
{
	OpenFile(file_name);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogHandlerFile::LogHandlerFile(const std::string &file_name,
	LogHandlerFileFlag flags)
	:LogHandler()
	,out_file_name_()
	,out_file_ptr_()
	,my_flags_(flags)
	,the_lock_()
{
	OpenFile(file_name);
}
// ////////////////////////////////////////////////////////////////////////////

/* 
	CODE NOTE: Decide whether to remove the elaborate instances of the OpenFile() overload.
// ////////////////////////////////////////////////////////////////////////////
LogHandlerFile::LogHandlerFile(const char *base_name, const char *dir_name,
	LogHandlerFileFlag flags)
	:LogHandler()
	,out_file_name_()
	,out_file_ptr_()
	,my_flags_(flags)
	,the_lock_()
{
	OpenFile(base_name, dir_name);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogHandlerFile::LogHandlerFile(const std::string &base_name,
	const std::string &dir_name, LogHandlerFileFlag flags)
	:LogHandler()
	,out_file_name_()
	,out_file_ptr_()
	,my_flags_(flags)
	,the_lock_()
{
	OpenFile(base_name, dir_name);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogHandlerFile::LogHandlerFile(const char *base_name, const char *dir_name,
	const MLB::Utility::TimeT &start_time, LogHandlerFileFlag flags)
	:LogHandler()
	,out_file_name_()
	,out_file_ptr_()
	,my_flags_(flags)
	,the_lock_()
{
	OpenFile(base_name, dir_name, start_time);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogHandlerFile::LogHandlerFile(const std::string &base_name,
	const std::string &dir_name, const MLB::Utility::TimeT &start_time,
	LogHandlerFileFlag flags)
	:LogHandler()
	,out_file_name_()
	,out_file_ptr_()
	,my_flags_(flags)
	,the_lock_()
{
	OpenFile(base_name, dir_name, start_time);
}
// ////////////////////////////////////////////////////////////////////////////
*/

// ////////////////////////////////////////////////////////////////////////////
LogHandlerFile::~LogHandlerFile()
{
	LogLockScoped my_lock(the_lock_);

	if (out_file_ptr_->is_open()) {
		out_file_ptr_->flush();
		out_file_ptr_->close();
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LogHandlerFile::InstallHandler()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LogHandlerFile::RemoveHandler()
{
	LogLockScoped my_lock(the_lock_);

	if (out_file_ptr_->is_open())
		out_file_ptr_->flush();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LogHandlerFile::EmitLine(const LogEmitControl &emit_control)
{
	if (emit_control.ShouldLogPersistent() || emit_control.ShouldLogScreen()) {
		LogLockScoped my_lock(the_lock_);
		emit_control.UpdateTime();
		if (emit_control.ShouldLogPersistent() && (out_file_ptr_ != NULL)) {
			out_file_ptr_->write(emit_control.GetLeaderPtr(),
				static_cast<std::streamsize>(emit_control.GetLeaderLength()));
			out_file_ptr_->write(emit_control.line_buffer_.c_str(),
				static_cast<std::streamsize>(emit_control.line_buffer_.size()));
			*out_file_ptr_ << std::endl;
		}
		if ((!(my_flags_ & NoConsoleOutput)) && emit_control.ShouldLogScreen()) {
			std::cout.write(emit_control.GetLeaderPtr(),
				static_cast<std::streamsize>(emit_control.GetLeaderLength()));
			std::cout.write(emit_control.line_buffer_.c_str(),
				static_cast<std::streamsize>(emit_control.line_buffer_.size()));
			std::cout << std::endl;
		}
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LogHandlerFile::EmitLiteral(unsigned int literal_length,
	const char *literal_string)
{
	LogLockScoped my_lock(the_lock_);

	if (out_file_ptr_ != NULL) {
		out_file_ptr_->write(literal_string,
			static_cast<std::streamsize>(literal_length));
		*out_file_ptr_ << std::endl;
	}

	if (!(my_flags_ & NoConsoleOutput)) {
		std::cout.write(literal_string,
			static_cast<std::streamsize>(literal_length));
		std::cout << std::endl;
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LogHandlerFile::EmitLiteral(const LogEmitControl &emit_control,
	unsigned int literal_length, const char *literal_string)
{
	if (emit_control.ShouldLogPersistent() || emit_control.ShouldLogScreen()) {
		LogLockScoped my_lock(the_lock_);
		if (emit_control.ShouldLogPersistent() && (out_file_ptr_ != NULL)) {
			out_file_ptr_->write(literal_string,
				static_cast<std::streamsize>(literal_length));
			*out_file_ptr_ << std::endl;
		}
		if ((!(my_flags_ & NoConsoleOutput)) && emit_control.ShouldLogScreen()) {
			std::cout.write(literal_string,
				static_cast<std::streamsize>(literal_length));
			std::cout << std::endl;
		}
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LogHandlerFile::OpenFile(const char *file_name)
{
	if ((file_name == NULL) || (!(*file_name)))
		throw std::invalid_argument("Specified log file name is NULL or an empty string.");

	try {
		LogSPtr<std::ofstream> tmp_file_ptr(
			new std::ofstream(file_name,
				(!(my_flags_ & DoNotAppend)) ?
				(std::ios_base::app | std::ios_base::ate) :
				(std::ios_base::app | std::ios_base::trunc)));
		if (tmp_file_ptr->fail())
			throw std::runtime_error("Open attempt failed.");
		{
			LogLockScoped my_lock(the_lock_);
			if ((out_file_ptr_ != NULL) && out_file_ptr_->is_open()) {
				out_file_ptr_->flush();
				out_file_ptr_->close();
				out_file_ptr_.reset();
			}
			out_file_ptr_  = tmp_file_ptr;
			out_file_name_ = file_name;
		}
	}
	catch (const std::exception &except) {
		Rethrow(except, "Unable to open log file '" + std::string(file_name) +
			std::string("': ") + except.what());
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LogHandlerFile::OpenFile(const std::string &file_name)
{
	OpenFile(file_name.c_str());
}
// ////////////////////////////////////////////////////////////////////////////

/* 
	CODE NOTE: Decide whether to remove the elaborate instances of the OpenFile() overload.
// ////////////////////////////////////////////////////////////////////////////
void LogHandlerFile::OpenFile(const char *base_name, const char *dir_name)
{
	OpenFile(base_name, dir_name, MLB::Utility::TimeT());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LogHandlerFile::OpenFile(const std::string &base_name,
	const std::string &dir_name)
{
	OpenFile(base_name.c_str(), dir_name.c_str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LogHandlerFile::OpenFile(const char *base_name, const char *dir_name,
	const MLB::Utility::TimeT &start_time)
{
	std::string file_name;

	if ((base_name == NULL) || (!(*base_name)))
		file_name = "LogFile.";
	else {
		file_name = base_name;
		if (base_name[strlen(base_name) - 1] != '.')
			file_name += '.';
	}

	std::string tmp_date_time(start_time.ToString());

	tmp_date_time[10]  = '.';
	tmp_date_time[13]  = '.';
	tmp_date_time[16]  = '.';
	file_name         += tmp_date_time + "." + GetHostNameCanonical() + "." +
		AnyToString(CurrentProcessId()) + ".log";

	boost::filesystem::path tmp_file(BoostFs_ConstructNativePath(file_name));

	if ((dir_name != NULL) && *dir_name) {
		std::string tmp_dir_name;
		ResolveFilePathGeneral(dir_name, tmp_dir_name, "", true, true, false);
		boost::filesystem::path tmp_path(
			BoostFs_ConstructNativePath(tmp_dir_name));
		boost::filesystem::path this_file;
		this_file        = tmp_path / tmp_file;
		file_name        = BoostFs_GetNativeFileString(this_file);
	}
	else {
		if (!tmp_file.has_root_path())
			tmp_file = boost::filesystem::system_complete(tmp_file);
		file_name = BoostFs_GetNativeFileString(tmp_file);
	}

	OpenFile(file_name);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LogHandlerFile::OpenFile(const std::string &base_name,
	const std::string &dir_name, const MLB::Utility::TimeT &start_time)
{
	OpenFile(base_name.c_str(), dir_name.c_str(), start_time);
}
// ////////////////////////////////////////////////////////////////////////////
*/

// ////////////////////////////////////////////////////////////////////////////
LogHandlerFile::LogHandlerFileFlag LogHandlerFile::GetFlags() const
{
	LogLockScoped my_lock(the_lock_);

	return(my_flags_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogHandlerFile::LogHandlerFileFlag LogHandlerFile::SetFlags(
	LogHandlerFileFlag new_flags)
{
	LogLockScoped my_lock(the_lock_);

	LogHandlerFileFlag old_flags = my_flags_;

	my_flags_ = new_flags;

	return(old_flags);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string LogHandlerFile::GetFileName() const
{
	return(out_file_name_);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

// ////////////////////////////////////////////////////////////////////////////
// ****************************************************************************
// ****************************************************************************
// ****************************************************************************
// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
LogHandlerXFile::LogHandlerXFile()
	:LogHandlerFileBase()
	,out_file_ptr_()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogHandlerXFile::LogHandlerXFile(const char *file_name,
	LogHandlerFileBaseFlag flags)
	:LogHandlerFileBase(flags)
	,out_file_ptr_()
{
	OpenFile(file_name);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogHandlerXFile::LogHandlerXFile(const std::string &file_name,
	LogHandlerFileBaseFlag flags)
	:LogHandlerFileBase(flags)
	,out_file_ptr_()
{
	OpenFile(file_name);
}
// ////////////////////////////////////////////////////////////////////////////

/* 
	CODE NOTE: Decide whether to remove the elaborate instances of the OpenFile() overload.
// ////////////////////////////////////////////////////////////////////////////
LogHandlerXFile::LogHandlerXFile(const char *base_name, const char *dir_name,
	LogHandlerFileBaseFlag flags)
	:LogHandlerFileBase(flags)
	,out_file_ptr_()
{
	OpenFile(base_name, dir_name);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogHandlerXFile::LogHandlerXFile(const std::string &base_name,
	const std::string &dir_name, LogHandlerFileBaseFlag flags)
	:LogHandlerFileBase(flags)
	,out_file_ptr_()
{
	OpenFile(base_name, dir_name);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogHandlerXFile::LogHandlerXFile(const char *base_name, const char *dir_name,
	const MLB::Utility::TimeT &start_time, LogHandlerFileBaseFlag flags)
	:LogHandlerFileBase(flags)
	,out_file_ptr_()
{
	OpenFile(base_name, dir_name, start_time);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogHandlerXFile::LogHandlerXFile(const std::string &base_name,
	const std::string &dir_name, const MLB::Utility::TimeT &start_time,
	LogHandlerFileBaseFlag flags)
	:LogHandlerFileBase(flags)
	,out_file_ptr_()
{
	OpenFile(base_name, dir_name, start_time);
}
// ////////////////////////////////////////////////////////////////////////////
*/

// ////////////////////////////////////////////////////////////////////////////
LogHandlerXFile::~LogHandlerXFile()
{
	LogLockScoped my_lock(the_lock_);

	if ((out_file_ptr_ != NULL) && out_file_ptr_->is_open()) {
		out_file_ptr_->flush();
		out_file_ptr_->close();
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LogHandlerXFile::InstallHandlerImpl()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LogHandlerXFile::RemoveHandlerImpl()
{
	if (out_file_ptr_->is_open())
		out_file_ptr_->flush();
}
// ////////////////////////////////////////////////////////////////////////////

namespace {

// ////////////////////////////////////////////////////////////////////////////
/*
	CODE NOTE: LogFileHandler buffering test code. Restricts LogHandlerXFile
	to single instance use. To be replaced.
*/
static char TestFileBuffer[2000000];
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
void LogHandlerXFile::OpenFileImpl(const char *file_name)
{
	LogSPtr<std::ofstream> tmp_file_ptr(
		new std::ofstream(file_name,
			(!(my_flags_ & DoNotAppend)) ?
			(std::ios_base::app | std::ios_base::ate) :
			(std::ios_base::app | std::ios_base::trunc)));

	if (tmp_file_ptr->fail())
		throw std::runtime_error("Open attempt failed.");

	/*
		CODE NOTE: LogFileHandler buffering test code. Restricts LogHandlerXFile
		to single instance use. To be replaced.
	*/
	std::streambuf *new_buffer_ptr =
		tmp_file_ptr->rdbuf()->pubsetbuf(TestFileBuffer, sizeof(TestFileBuffer));
	if (new_buffer_ptr == NULL) {
		throw std::runtime_error("Attempt to set the log file buffer size to " +
			AnyToString(sizeof(TestFileBuffer)) + " bytes failed.");
	}

	{
		std::string   tmp_file_name(file_name);
		LogLockScoped my_lock(the_lock_);
		if ((out_file_ptr_ != NULL) && out_file_ptr_->is_open()) {
			out_file_ptr_->flush();
			out_file_ptr_->close();
			out_file_ptr_.reset();
		}
		out_file_ptr_.swap(tmp_file_ptr);
		out_file_name_.swap(tmp_file_name);
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LogHandlerXFile::FlushImpl()
{
	if ((out_file_ptr_ != NULL) && out_file_ptr_->is_open())
		out_file_ptr_->flush();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LogHandlerXFile::EmitLineImpl(const LogEmitControl &emit_control)
{
	if (out_file_ptr_ != NULL) {
		out_file_ptr_->write(emit_control.GetLeaderPtr(),
			static_cast<std::streamsize>(emit_control.GetLeaderLength()));
		out_file_ptr_->write(emit_control.line_buffer_.c_str(),
			static_cast<std::streamsize>(emit_control.line_buffer_.size()));
		*out_file_ptr_ << std::endl;
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LogHandlerXFile::EmitLiteralImpl(unsigned int literal_length,
	const char *literal_string)
{
	if (out_file_ptr_ != NULL) {
		out_file_ptr_->write(literal_string,
			static_cast<std::streamsize>(literal_length));
		*out_file_ptr_ << std::endl;
	}
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
			new LogHandlerXFile("TestLogXFile.VERSION_OLD_001.log"));
*/
		LogHandlerPtr my_log_handler(
			new LogHandlerXFile(TEST_GetLogFileName("LogHandlerXFile")));
		TEST_TestControl(my_log_handler, 10000, 200, 1, 2000000);
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

