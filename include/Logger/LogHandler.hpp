// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  LogHandler.hpp

   File Description  :  Include file for the log handler class.

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

#ifndef HH__MLB__Utility__Utility__LogHandler_hpp__HH

#define HH__MLB__Utility__Utility__LogHandler_hpp__HH 1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Logger/LogEmitControl.hpp>

#include <memory>
#include <mutex>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
template <typename DatumType>
	using LogSPtr    = std::shared_ptr<DatumType>;
using LogLock       = std::mutex;
using LogLockScoped = std::lock_guard<LogLock>;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class API_UTILITY LogHandler {
public:
	LogHandler();
	virtual ~LogHandler();

	virtual void InstallHandler();
	virtual void RemoveHandler();

	virtual void EmitLine(const LogEmitControl &emit_control) = 0;
	virtual void EmitLiteral(unsigned int literal_length,
		const char *literal_string) = 0;
	virtual void EmitLiteral(const LogEmitControl &emit_control,
		unsigned int literal_length, const char *literal_ptr) = 0;

	virtual void EmitLineSpecific(const std::string &line_buffer,
		LogLevel log_level = LogLevel_Info);
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
using LogHandlerPtr = LogSPtr<LogHandler>;
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__Utility__LogHandler_hpp__HH

