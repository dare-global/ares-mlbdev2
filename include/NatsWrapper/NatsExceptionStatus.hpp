// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB NatsWrapper Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  NatsExceptionStatus.hpp

   File Description  :  Include file for the NatsExceptionStatus class.

   Revision History  :  2024-09-10 --- Creation.
                           Michael L. Brock

      Copyright Michael L. Brock 2024.
      Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
      http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__NatsWrapper__NatsExceptionStatus_hpp__HH

#define HH__MLB__NatsWrapper__NatsExceptionStatus_hpp__HH 1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
   \file NatsExceptionStatus.hpp

   \brief   Main include file for the NatsExceptionStatus class.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/Utility.hpp>

#include <nats/nats.h>

#include <stdexcept>
#include <string>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace NatsWrapper {

// ////////////////////////////////////////////////////////////////////////////
class MB_LIB_EXCEPTION_CLASS(API_NATSWRAPPER) NatsExceptionStatus :
	public std::runtime_error
{
public:
	NatsExceptionStatus();
	NatsExceptionStatus(const std::string &error_text);
	NatsExceptionStatus(natsStatus nats_code, const char *func_name = nullptr,
		const char *params = nullptr);
	NatsExceptionStatus(const std::string &src_text, natsStatus nats_code);

	virtual ~NatsExceptionStatus();

/*
	virtual const char *what() noexcept;
*/

	natsStatus GetNatsStatus() const;

private:
	natsStatus nats_code_;

	static std::string GetStatusString(natsStatus nats_code,
		const char *func_name, const char *params);
	static std::string GetStatusString(const std::string &src_text,
   	natsStatus nats_code);
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace NatsWrapper

} // namespace MLB

#endif // #ifndef HH__MLB__NatsWrapper__NatsExceptionStatus_hpp__HH

