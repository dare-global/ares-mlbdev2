// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB NatsWrapper Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  NatsOptions.hpp

   File Description  :  Include file for the NatsOptions class.

   Revision History  :  2024-08-17 --- Creation.
                           Michael L. Brock

      Copyright Michael L. Brock 2024.
      Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
      http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__NatsWrapper__NatsOptions_hpp__HH

#define HH__MLB__NatsWrapper__NatsOptions_hpp__HH 1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
   \file NatsOptions.hpp

   \brief   Main include file for the NatsOptions class.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <NatsWrapper/NatsWrapper.hpp>

#include <memory>
#include <string>
#include <vector>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace NatsWrapper {

// ////////////////////////////////////////////////////////////////////////////
class NatsOptions
{
public:
	NatsOptions();

	virtual ~NatsOptions();

	      natsOptions *GetPtr();
	const natsOptions *GetPtr() const;

	void SetURL(const char *url);
	void SetURL(const std::string &url);

	void SetServers(const char **servers, int servers_count);
	void SetServers(const char **servers, std::size_t servers_count);
	void SetServers(const std::vector<std::string> &servers);

	void SetMaxReconnect(int max_reconnect);
	void SetReconnectWait(int64_t reconnect_wait_ms);

	void SetRetryOnFailedConnect(bool retry,
		natsConnectionHandler connected_cb = nullptr,
		void *closure = nullptr);

	void SetErrorHandler(natsErrHandler err_handler, void *closure);
	void SetClosedCB(natsConnectionHandler closed_cb, void *closure);
	void SetDisconnectedCB(natsConnectionHandler disconnected_cb,
		void *closure);
	void SetReconnectedCB(natsConnectionHandler reconnected_cb,
		void *closure);

private:
	std::shared_ptr<natsOptions> nats_options_sptr_;
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace NatsWrapper

} // namespace MLB

#endif // #ifndef HH__MLB__NatsWrapper__NatsOptions_hpp__HH

