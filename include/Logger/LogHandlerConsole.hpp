// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  LogHandlerConsole.hpp

   File Description  :  Include file for the log handler console class.

   Revision History  :  1993-10-02 --- Creation of predecessor 'mlog' facility.
                           Michael L. Brock
                        2005-01-02 --- New ostream-based log model.
                           Michael L. Brock
                        2023-02-27 --- Migration to C++ MlbDev2/Utility.
                           Michael L. Brock

      Copyright Michael L. Brock 2005 - 2023.
      Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
      http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__Utility__Utility__LogHandlerConsole_hpp__HH

#define HH__MLB__Utility__Utility__LogHandlerConsole_hpp__HH   1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Logger/LogEmitControl.hpp>
#include <Logger/LogHandler.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
class API_UTILITY LogHandlerConsole : public LogHandler {
public:
	LogHandlerConsole();

	virtual ~LogHandlerConsole();

	virtual void InstallHandler();
	virtual void RemoveHandler();

	virtual void EmitLine(const LogEmitControl &emit_control);
	virtual void EmitLiteral(unsigned int literal_length,
		const char *literal_string);
	virtual void EmitLiteral(const LogEmitControl &emit_control,
		unsigned int literal_length, const char *literal_string);

protected:
	mutable LogLock the_lock_;

private:
	std::ios_base::Init iostreams_init_;

	LogHandlerConsole(const LogHandlerConsole &) = delete;
	LogHandlerConsole & operator = (const LogHandlerConsole &) = delete;
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__Utility__LogHandlerConsole_hpp__HH

