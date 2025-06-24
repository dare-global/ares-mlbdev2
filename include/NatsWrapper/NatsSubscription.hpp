// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB NatsWrapper Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  NatsSubscription.hpp

   File Description  :  Include file for the NatsSubscription class.

   Revision History  :  2024-08-17 --- Creation.
                           Michael L. Brock

      Copyright Michael L. Brock 2024.
      Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
      http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__NatsWrapper__NatsSubscription_hpp__HH

#define HH__MLB__NatsWrapper__NatsSubscription_hpp__HH 1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
   \file NatsSubscription.hpp

   \brief   Main include file for the NatsSubscription class.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <NatsWrapper/NatsWrapper.hpp>

#include <memory>
#include <string>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace NatsWrapper {

// ////////////////////////////////////////////////////////////////////////////
class NatsConnection;
class NatsMsg;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class NatsSubscription
{
public:
	// Performs a natsConnection_Subscribe()
	NatsSubscription(NatsConnection &nats_conn, const char *subject_name,
		natsMsgHandler call_back, void *closure = nullptr);
	NatsSubscription(NatsConnection &nats_conn, const std::string &subject_name,
		natsMsgHandler call_back, void *closure = nullptr);

	// Performs a natsConnection_SubscribeTimeout()
	NatsSubscription(NatsConnection &nats_conn, const char *subject_name,
		int64_t time_out, natsMsgHandler call_back, void *closure = nullptr);
	NatsSubscription(NatsConnection &nats_conn, const std::string &subject_name,
		int64_t time_out, natsMsgHandler call_back, void *closure = nullptr);

	// Performs a natsConnection_SubscribeSync()
	NatsSubscription(NatsConnection &nats_conn, const char *subject_name);
	NatsSubscription(NatsConnection &nats_conn, const std::string &subject_name);

	virtual ~NatsSubscription();

	      natsSubscription *GetPtr();
	const natsSubscription *GetPtr() const;

	/// Returns a hollow NatMsg upon timeout.
	NatsMsg NextMsg(int64_t time_out);

	void Unsubscribe();

protected:
	virtual void NatsMsgHandler(natsConnection *nats_conn_ptr,
		natsSubscription *nats_subs_ptr, natsMsg *nats_msg_ptr);

	static void NatsMsgHandler(natsConnection *nats_conn_ptr,
		natsSubscription *nats_subs_ptr,
		natsMsg *nats_msg_ptr, void *closure_ptr);

private:
	std::shared_ptr<natsSubscription> nats_subscription_sptr_;
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace NatsWrapper

} // namespace MLB

#endif // #ifndef HH__MLB__NatsWrapper__NatsSubscription_hpp__HH

