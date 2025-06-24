// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  LogHandlerFile.hpp

   File Description  :  Include file for the log handler file class.

   Revision History  :  1993-10-02 --- Creation of predecessor 'mlog' facility.
                           Michael L. Brock
                        2005-01-02 --- New ostream-based log model.
                           Michael L. Brock
                        2023-01-05 --- Migration to C++ MlbDev2/Utility.
                           Michael L. Brock

      Copyright Michael L. Brock 2005 - 2023.
      Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
      http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__Utility__Utility__LogHandlerFile_hpp__HH

#define HH__MLB__Utility__Utility__LogHandlerFile_hpp__HH   1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Logger/LogHandlerFileBase.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
class API_UTILITY LogHandlerFile : public LogHandler {
public:
	enum LogHandlerFileFlag {
		None            = 0x0000,
		DoNotAppend     = 0x0001,
		NoConsoleOutput = 0x0002,
		Default         = None
	};
	LogHandlerFile();
	explicit LogHandlerFile(const char *file_name,
		LogHandlerFileFlag flags = Default);
	explicit LogHandlerFile(const std::string &file_name,
		LogHandlerFileFlag flags = Default);
/* 
	CODE NOTE: Decide whether to remove the elaborate instances of the OpenFile() overload.
	LogHandlerFile(const char *base_name, const char *dir_name,
		LogHandlerFileFlag flags = Default);
	LogHandlerFile(const std::string &base_name, const std::string &dir_name,
		LogHandlerFileFlag flags = Default);
	LogHandlerFile(const char *base_name, const char *dir_name,
		const MLB::Utility::TimeT &start_time,
		LogHandlerFileFlag flags = Default);
	LogHandlerFile(const std::string &base_name, const std::string &dir_name,
		const MLB::Utility::TimeT &start_time,
   	LogHandlerFileFlag flags = Default);
*/

	virtual ~LogHandlerFile();

	void InstallHandler();
	void RemoveHandler();

	virtual void EmitLine(const LogEmitControl &emit_control);
	virtual void EmitLiteral(unsigned int literal_length,
		const char *literal_string);
	virtual void EmitLiteral(const LogEmitControl &emit_control,
		unsigned int literal_length, const char *literal_string);

	virtual void OpenFile(const char *file_name);
	virtual void OpenFile(const std::string &file_name);
/* 
	CODE NOTE: Decide whether to remove the elaborate instances of the OpenFile() overload.
	virtual void OpenFile(const char *base_name, const char *dir_name);
	virtual void OpenFile(const std::string &base_name,
		const std::string &dir_name);
	virtual void OpenFile(const char *base_name, const char *dir_name,
		const MLB::Utility::TimeT &start_time);
	virtual void OpenFile(const std::string &base_name,
   	const std::string &dir_name, const MLB::Utility::TimeT &start_time);
*/

	LogHandlerFileFlag GetFlags() const;
	LogHandlerFileFlag SetFlags(LogHandlerFileFlag new_flags);
	std::string        GetFileName() const;

protected:
	std::string            out_file_name_;
	LogSPtr<std::ofstream> out_file_ptr_;
	LogHandlerFileFlag     my_flags_;
	mutable LogLock        the_lock_;

private:

	LogHandlerFile(const LogHandlerFile &) = delete;
	LogHandlerFile & operator = (const LogHandlerFile &) = delete;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class API_UTILITY LogHandlerXFile : public LogHandlerFileBase {
public:
	LogHandlerXFile();
	explicit LogHandlerXFile(const char *file_name,
		LogHandlerFileBaseFlag flags = Default);
	explicit LogHandlerXFile(const std::string &file_name,
		LogHandlerFileBaseFlag flags = Default);
/* 
	CODE NOTE: Decide whether to remove the elaborate instances of the OpenFile() overload.
	LogHandlerXFile(const char *base_name, const char *dir_name,
		LogHandlerFileBaseFlag flags = Default);
	LogHandlerXFile(const std::string &base_name, const std::string &dir_name,
		LogHandlerFileBaseFlag flags = Default);
	LogHandlerXFile(const char *base_name, const char *dir_name,
		const MLB::Utility::TimeT &start_time,
		LogHandlerFileBaseFlag flags = Default);
	LogHandlerXFile(const std::string &base_name, const std::string &dir_name,
		const MLB::Utility::TimeT &start_time,
		LogHandlerFileBaseFlag flags = Default);
*/

	virtual ~LogHandlerXFile();

protected:
	virtual void InstallHandlerImpl();
	virtual void RemoveHandlerImpl();
	virtual void OpenFileImpl(const char *file_name);
	virtual void FlushImpl();
	virtual void EmitLineImpl(const LogEmitControl &emit_control);
	virtual void EmitLiteralImpl(unsigned int literal_length,
		const char *literal_string);

	LogSPtr<std::ofstream> out_file_ptr_;

private:

	LogHandlerXFile(const LogHandlerXFile &) = delete;
	LogHandlerXFile & operator = (const LogHandlerXFile &) = delete;
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__Utility__LogHandlerFile_hpp__HH

