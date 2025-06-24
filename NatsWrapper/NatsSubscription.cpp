// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB NatsWrapper Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  NatsSubscription.cpp

   File Description  :  Implementation of the NatsSubscription class.

   Revision History  :  2024-08-17 --- Creation.
                           Michael L. Brock

      Copyright Michael L. Brock 2024.
      Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
      http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <NatsWrapper/NatsSubscription.hpp>

#include <NatsWrapper/NatsConnection.hpp>
#include <NatsWrapper/NatsMsg.hpp>

#include <Utility/ArgCheck.hpp>

#include <cstring>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace NatsWrapper {

// ////////////////////////////////////////////////////////////////////////////
NatsSubscription::NatsSubscription(NatsConnection &nats_conn,
	const char *subject_name, natsMsgHandler call_back, void *closure)
	:nats_subscription_sptr_()
{
	MLB::Utility::ThrowIfNull(subject_name, "The subscription subject name");

	natsSubscription *nats_sub = NULL;

	NatsWrapper_THROW_IF_NOT_OK(::natsConnection_Subscribe,
		(&nats_sub, nats_conn.GetPtr(), subject_name, call_back, closure))

	nats_subscription_sptr_.reset(nats_sub, ::natsSubscription_Destroy);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
NatsSubscription::NatsSubscription(NatsConnection &nats_conn,
	const std::string &subject_name, natsMsgHandler call_back, void *closure)
	:NatsSubscription(nats_conn, subject_name.c_str(), call_back, closure)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
NatsSubscription::NatsSubscription(NatsConnection &nats_conn,
	const char *subject_name, int64_t time_out, natsMsgHandler call_back,
	void *closure)
	:nats_subscription_sptr_()
{
	MLB::Utility::ThrowIfNull(subject_name, "The subscription subject name");

	natsSubscription *nats_sub = NULL;

	NatsWrapper_THROW_IF_NOT_OK(::natsConnection_SubscribeTimeout,
		(&nats_sub, nats_conn.GetPtr(), subject_name, time_out,
		 call_back, closure))

	nats_subscription_sptr_.reset(nats_sub, ::natsSubscription_Destroy);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
NatsSubscription::NatsSubscription(NatsConnection &nats_conn,
	const std::string &subject_name, int64_t time_out, natsMsgHandler call_back,
	void *closure)
	:NatsSubscription(nats_conn, subject_name.c_str(), time_out,
		call_back, closure)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
NatsSubscription::NatsSubscription(NatsConnection &nats_conn,
	const char *subject_name)
	:nats_subscription_sptr_()
{
	MLB::Utility::ThrowIfNull(subject_name, "The subscription subject name");

	natsSubscription *nats_sub = NULL;

	NatsWrapper_THROW_IF_NOT_OK(::natsConnection_SubscribeSync,
		(&nats_sub, nats_conn.GetPtr(), subject_name))

	nats_subscription_sptr_.reset(nats_sub, ::natsSubscription_Destroy);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
NatsSubscription::NatsSubscription(NatsConnection &nats_conn,
	const std::string &subject_name)
	:NatsSubscription(nats_conn, subject_name.c_str())
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
NatsSubscription::~NatsSubscription()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
natsSubscription *NatsSubscription::GetPtr()
{
	return(nats_subscription_sptr_.get());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const natsSubscription *NatsSubscription::GetPtr() const
{
	return(nats_subscription_sptr_.get());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
NatsMsg NatsSubscription::NextMsg(int64_t time_out)
{
	natsMsg *nats_msg = NULL;

	NatsWrapper_THROW_IF_NOT_OK_OR_TIMEOUT(::natsSubscription_NextMsg,
		(&nats_msg, GetPtr(), time_out))

	return(NatsMsg(nats_msg));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void NatsSubscription::Unsubscribe()
{
	if (nats_subscription_sptr_)
		::natsSubscription_Unsubscribe(GetPtr());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void NatsSubscription::NatsMsgHandler(natsConnection * /* nats_conn_ptr */,
	natsSubscription * /* nats_subs_ptr */, natsMsg * /* nats_msg_ptr */)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void NatsSubscription::NatsMsgHandler(natsConnection *nats_conn_ptr,
	natsSubscription *nats_subs_ptr, natsMsg *nats_msg_ptr, void *closure_ptr)
{
	if (!closure_ptr)
		MLB::Utility::ThrowIfNull("NatsSubscription::NatsMsgHandler() called "
			"with a NULL closure pointer.");

/*
	NatsSubscription *my_nats_sub;

	if ((my_nats_sub = dynamic_cast<NatsSubscription *>(closure_ptr)) == NULL)
		MLB::Utility::ThrowIfNull("NatsSubscription::NatsMsgHandler() called "
			"with a closure pointer which dynamic_cast<> reports can ot be "
			"converted to a NatsSubscription pointer.");
*/

	NatsSubscription *my_nats_sub = ((NatsSubscription *) closure_ptr);

	my_nats_sub->NatsMsgHandler(nats_conn_ptr, nats_subs_ptr, nats_msg_ptr);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace NatsWrapper

} // namespace MLB

// ////////////////////////////////////////////////////////////////////////////
// ****************************************************************************
// ****************************************************************************
// ****************************************************************************
// ////////////////////////////////////////////////////////////////////////////

#ifdef TEST_MAIN

#include <NatsWrapper/NatsContext.hpp>

#include <Utility/Sleep.hpp>

#include <iostream>

namespace {

// ////////////////////////////////////////////////////////////////////////////
void TEST_NatsSubscription(int argc, char **argv)
{
	using namespace MLB::NatsWrapper;

	NatsContext                   nats_context;
	NatsOptions                   nats_options;
	NatsConnection                nats_connection(nats_options);
	std::vector<NatsSubscription> subscription_list;

	for (int arg_idx = 1; arg_idx < argc; ++arg_idx) {
		NatsSubscription nats_subs(nats_connection, argv[arg_idx]);
		subscription_list.push_back(nats_subs);
	}
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
		TEST_NatsSubscription(argc, argv);
	}
	catch (const std::exception &except) {
		return_code = EXIT_FAILURE;
		std::cerr << "\n\nERROR: " << except.what() << std::endl;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

