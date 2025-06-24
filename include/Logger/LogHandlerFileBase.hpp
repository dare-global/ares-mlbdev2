// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  LogHandlerFileBase.hpp

   File Description  :  Include file for the log handler file base class.

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

#ifndef HH__MLB__Utility__Utility__LogHandlerFileBase_hpp__HH

#define HH__MLB__Utility__Utility__LogHandlerFileBase_hpp__HH  1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Logger/LogHandler.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
class API_UTILITY LogHandlerFileBase : public LogHandler {
public:
	enum LogHandlerFileBaseFlag {
		None            = 0x0000,
		DoNotAppend     = 0x0001,
		NoConsoleOutput = 0x0002,
		Default         = None
	};

	explicit LogHandlerFileBase(LogHandlerFileBaseFlag flags = Default);

	virtual ~LogHandlerFileBase() override;

	virtual void InstallHandler() override;
	virtual void RemoveHandler() override;

	virtual void EmitLine(const LogEmitControl &emit_control) override;
	virtual void EmitLiteral(unsigned int literal_length,
		const char *literal_string) override;
	virtual void EmitLiteral(const LogEmitControl &emit_control,
		unsigned int literal_length, const char *literal_string) override;

	void OpenFile(const char *file_name);
	void OpenFile(const std::string &file_name);
/* 
	CODE NOTE: Decide whether to remove the elaborate instances of the OpenFile() overload.
	void OpenFile(const char *base_name, const char *dir_name);
	void OpenFile(const std::string &base_name, const std::string &dir_name);
	void OpenFile(const char *base_name, const char *dir_name,
		const MLB::Utility::TimeT &start_time);
	void OpenFile(const std::string &base_name, const std::string &dir_name,
		const MLB::Utility::TimeT &start_time);
*/

	void Flush();

	LogHandlerFileBaseFlag GetFlags() const;
	LogHandlerFileBaseFlag SetFlags(LogHandlerFileBaseFlag new_flags);

	std::string        GetFileName() const;

protected:
	virtual void InstallHandlerImpl() = 0;
	virtual void RemoveHandlerImpl() = 0;
	virtual void OpenFileImpl(const char *file_name) = 0;
	virtual void FlushImpl() = 0;
	virtual void EmitLineImpl(const LogEmitControl &emit_control) = 0;
	virtual void EmitLiteralImpl(unsigned int literal_length,
		const char *literal_string) = 0;

	std::string            out_file_name_;
	LogHandlerFileBaseFlag my_flags_;
	mutable LogLock        the_lock_;

private:
	LogHandlerFileBase(const LogHandlerFileBase &) = delete;
	LogHandlerFileBase & operator = (const LogHandlerFileBase &) = delete;
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__Utility__LogHandlerFileBase_hpp__HH

