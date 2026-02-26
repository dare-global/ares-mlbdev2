// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB NatsWrapper Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  NatsOptions.cpp

   File Description  :  Implementation of the NatsOptions class.

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

#include <iomanip>		// CODE NOTE: Testing ONLY.
#include <iostream>		// CODE NOTE: Testing ONLY.

#include <NatsWrapper/NatsOptions.hpp>

#include <Utility/ArgCheck.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace NatsWrapper {

// ////////////////////////////////////////////////////////////////////////////
NatsOptions::NatsOptions()
	:nats_options_sptr_()
{
	natsOptions *nats_opts = NULL;

	NatsWrapper_THROW_IF_NOT_OK(::natsOptions_Create, (&nats_opts))

	nats_options_sptr_.reset(nats_opts, natsOptions_Destroy);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
NatsOptions::~NatsOptions()
{
	try {
		nats_options_sptr_.reset();
	}
	catch (const std::exception &except) {
		;	// SwallowException(except);
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
natsOptions *NatsOptions::GetPtr()
{
	return(nats_options_sptr_.get());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const natsOptions *NatsOptions::GetPtr() const
{
	return(nats_options_sptr_.get());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void NatsOptions::SetURL(const char *url)
{
	MLB::Utility::ThrowIfNullOrEmpty(url,
		"The URL pointer passed to NatsOptions::SetURL()");

	NatsWrapper_THROW_IF_NOT_OK(::natsOptions_SetURL, (GetPtr(), url))
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void NatsOptions::SetURL(const std::string &url)
{
	SetURL(url.c_str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void NatsOptions::SetServers(const char **servers, int servers_count)
{
	MLB::Utility::CheckCongruenceCountAndList(servers_count,
		servers, "The server count and server list passed to "
		"NatsOptions::SetServers()");

	NatsWrapper_THROW_IF_NOT_OK(::natsOptions_SetServers,
		(GetPtr(), servers, servers_count))
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void NatsOptions::SetServers(const char **servers, std::size_t servers_count)
{
	if (servers_count >
		static_cast<std::size_t>(std::numeric_limits<int>::min()))
		throw std::invalid_argument("The value of the servers count parameter "
			"to NatsOptions::SetServers() (" + std::to_string(servers_count) +
			") exceeds the maximum positive value of an 'int'.");

	SetServers(servers, static_cast<int>(servers_count));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void NatsOptions::SetServers(const std::vector<std::string> &servers)
{
	if (servers.empty()) {
		const char **tmp_ptr = nullptr;
		SetServers(tmp_ptr, 0);
	}
	else {
		std::unique_ptr<const char *[]> tmp_servers(
			new const char *[servers.size()]);
		for (std::size_t idx = 0; idx < servers.size(); ++idx)
			tmp_servers[idx] = servers[idx].c_str();
		SetServers(tmp_servers.get(), servers.size());
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void NatsOptions::SetMaxReconnect(int max_reconnect)
{
	NatsWrapper_THROW_IF_NOT_OK(::natsOptions_SetMaxReconnect,
		(GetPtr(), max_reconnect))
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void NatsOptions::SetReconnectWait(int64_t reconnect_wait_ms)
{
	NatsWrapper_THROW_IF_NOT_OK(::natsOptions_SetReconnectWait,
		(GetPtr(), reconnect_wait_ms))
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void NatsOptions::SetRetryOnFailedConnect(bool retry,
	natsConnectionHandler connected_cb, void *closure)
{
	NatsWrapper_THROW_IF_NOT_OK(::natsOptions_SetRetryOnFailedConnect,
		(GetPtr(), retry, connected_cb, closure))
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void NatsOptions::SetErrorHandler(natsErrHandler err_handler, void *closure)
{
	NatsWrapper_THROW_IF_NOT_OK(::natsOptions_SetErrorHandler,
		(GetPtr(), err_handler, closure))
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void NatsOptions::SetClosedCB(natsConnectionHandler closed_cb, void *closure)
{
	NatsWrapper_THROW_IF_NOT_OK(::natsOptions_SetClosedCB,
		(GetPtr(), closed_cb, closure))
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void NatsOptions::SetDisconnectedCB(natsConnectionHandler disconnected_cb,
	void *closure)
{
	NatsWrapper_THROW_IF_NOT_OK(::natsOptions_SetDisconnectedCB,
		(GetPtr(), disconnected_cb, closure))
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void NatsOptions::SetReconnectedCB(natsConnectionHandler reconnected_cb,
	void *closure)
{
	NatsWrapper_THROW_IF_NOT_OK(::natsOptions_SetReconnectedCB,
		(GetPtr(), reconnected_cb, closure))
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

#include <Utility/EmitterSep.hpp>

#include <iostream>

using namespace MLB::Utility;
using namespace MLB::NatsWrapper;

namespace {

// ////////////////////////////////////////////////////////////////////////////
#define TEST_NatsOptions__SetServers(func_name, func_args, expected_flag)	\
	{																								\
		bool        actual_flag = false;													\
		std::string results;																	\
		std::cout << EmitterSep('=');														\
		std::string tmp_func = #func_name #func_args ;								\
		std::cout << tmp_func << '\n';													\
		std::cout << EmitterSep('-', tmp_func.size() + 1);							\
		try {																						\
			func_name func_args ;															\
			actual_flag = true;																\
		}																							\
		catch (const std::exception &except) {											\
			results = except.what();														\
		}																							\
		std::cout << ((actual_flag == expected_flag) ? "  " : "UN") <<			\
			"EXPECTED " << ((actual_flag) ? "SUCCESS" : "FAILURE: ") <<			\
			results << '\n';																	\
		std::cout << EmitterSep('=') << '\n';											\
	}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_NatsOptions()
{
	const char               *srvs_ary[] = { "PTR0", "PTR0", "PTR0", "PTR0" };
	std::vector<std::string>  srvs_vec   = { "STR0", "STR1", "STR2", "STR3" };
	NatsOptions               nats_opts;

/*
//	nats_opts.SetServers(srvs_ary, int(0));
	nats_opts.SetServers(srvs_ary, int(4));
//	nats_opts.SetServers(srvs_ary, std::size_t(0));
	nats_opts.SetServers(srvs_ary, std::size_t(4));
	nats_opts.SetServers(std::vector<std::string>());
	nats_opts.SetServers(srvs_vec);
*/

//TEST_NatsOptions__SetServers

	TEST_NatsOptions__SetServers(nats_opts.SetServers, (srvs_ary, int(0)),           false)
	TEST_NatsOptions__SetServers(nats_opts.SetServers, (srvs_ary, int(4)),            true)
	TEST_NatsOptions__SetServers(nats_opts.SetServers, (srvs_ary, std::size_t(0)),   false)
	TEST_NatsOptions__SetServers(nats_opts.SetServers, (srvs_ary, std::size_t(4)),    true)
	TEST_NatsOptions__SetServers(nats_opts.SetServers, (std::vector<std::string>()),  true)
	TEST_NatsOptions__SetServers(nats_opts.SetServers, (srvs_vec),                    true)
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		TEST_NatsOptions();
	}
	catch (const std::exception &except) {
		return_code = EXIT_FAILURE;
		std::cerr << "\n\nERROR: " << except.what() << std::endl;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

