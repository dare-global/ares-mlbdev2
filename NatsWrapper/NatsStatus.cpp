// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB NatsWrapper Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  NatsStatus.cpp

   File Description  :  Implementation of the NatsStatus support.

   Revision History  :  2024-09-18 --- Creation.
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

#include <NatsWrapper/NatsStatus.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace NatsWrapper {

// ////////////////////////////////////////////////////////////////////////////
const std::set<natsStatus> &GetNatsStatusSet()
{
	static std::set<natsStatus> NatsStatusSet =
	{
    NATS_OK         , //= 0,                ///< Success

    NATS_ERR,                           ///< Generic error
    NATS_PROTOCOL_ERROR,                ///< Error when parsing a protocol message,
                                        ///  or not getting the expected message.
    NATS_IO_ERROR,                      ///< IO Error (network communication).
    NATS_LINE_TOO_LONG,                 ///< The protocol message read from the socket
                                        ///  does not fit in the read buffer.

    NATS_CONNECTION_CLOSED,             ///< Operation on this connection failed because
                                        ///  the connection is closed.
    NATS_NO_SERVER,                     ///< Unable to connect, the server could not be
                                        ///  reached or is not running.
    NATS_STALE_CONNECTION,              ///< The server closed our connection because it
                                        ///  did not receive PINGs at the expected interval.
    NATS_SECURE_CONNECTION_WANTED,      ///< The client is configured to use TLS, but the
                                        ///  server is not.
    NATS_SECURE_CONNECTION_REQUIRED,    ///< The server expects a TLS connection.
    NATS_CONNECTION_DISCONNECTED,       ///< The connection was disconnected. Depending on
                                        ///  the configuration, the connection may reconnect.

    NATS_CONNECTION_AUTH_FAILED,        ///< The connection failed due to authentication error.
    NATS_NOT_PERMITTED,                 ///< The action is not permitted.
    NATS_NOT_FOUND,                     ///< An action could not complete because something
                                        ///  was not found. So far, this is an internal error.

    NATS_ADDRESS_MISSING,               ///< Incorrect URL. For instance no host specified in
                                        ///  the URL.

    NATS_INVALID_SUBJECT,               ///< Invalid subject, for instance NULL or empty string.
    NATS_INVALID_ARG,                   ///< An invalid argument is passed to a function. For
                                        ///  instance passing NULL to an API that does not
                                        ///  accept this value.
    NATS_INVALID_SUBSCRIPTION,          ///< The call to a subscription function fails because
                                        ///  the subscription has previously been closed.
    NATS_INVALID_TIMEOUT,               ///< Timeout must be positive numbers.

    NATS_ILLEGAL_STATE,                 ///< An unexpected state, for instance calling
                                        ///  #natsSubscription_NextMsg() on an asynchronous
                                        ///  subscriber.

    NATS_SLOW_CONSUMER,                 ///< The maximum number of messages waiting to be
                                        ///  delivered has been reached. Messages are dropped.

    NATS_MAX_PAYLOAD,                   ///< Attempt to send a payload larger than the maximum
                                        ///  allowed by the NATS Server.
    NATS_MAX_DELIVERED_MSGS,            ///< Attempt to receive more messages than allowed, for
                                        ///  instance because of #natsSubscription_AutoUnsubscribe().

    NATS_INSUFFICIENT_BUFFER,           ///< A buffer is not large enough to accommodate the data.

    NATS_NO_MEMORY,                     ///< An operation could not complete because of insufficient
                                        ///  memory.

    NATS_SYS_ERROR,                     ///< Some system function returned an error.

    NATS_TIMEOUT,                       ///< An operation timed-out. For instance
                                        ///  #natsSubscription_NextMsg().

    NATS_FAILED_TO_INITIALIZE,          ///< The library failed to initialize.
    NATS_NOT_INITIALIZED,               ///< The library is not yet initialized.

    NATS_SSL_ERROR,                     ///< An SSL error occurred when trying to establish a
                                        ///  connection.

    NATS_NO_SERVER_SUPPORT,             ///< The server does not support this action.

    NATS_NOT_YET_CONNECTED,             ///< A connection could not be immediately established and
                                        ///  #natsOptions_SetRetryOnFailedConnect() specified
                                        ///  a connected callback. The connect is retried asynchronously.

    NATS_DRAINING,                      ///< A connection and/or subscription entered the draining mode.
                                        ///  Some operations will fail when in that mode.

    NATS_INVALID_QUEUE_NAME,            ///< An invalid queue name was passed when creating a queue subscription.

    NATS_NO_RESPONDERS,                 ///< No responders were running when the server received the request.

    NATS_MISMATCH,                      ///< For JetStream subscriptions, it means that a consumer sequence mismatch was discovered.

    NATS_MISSED_HEARTBEAT,              ///< For JetStream subscriptions, it means that the library detected that server heartbeats have been missed.
	};

	return(NatsStatusSet);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::pair<natsStatus, natsStatus> GetNatsStatusRange()
{
	static const std::pair<natsStatus, natsStatus> NatsStatusRange(
		std::pair<natsStatus, natsStatus>(
			*GetNatsStatusSet().begin(), *GetNatsStatusSet().rbegin()));

	return(NatsStatusRange);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
natsStatus GetNatsStatusMin()
{
	static natsStatus NatsStatusMin = GetNatsStatusRange().first;

	return(NatsStatusMin);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
natsStatus GetNatsStatusMax()
{
	static natsStatus NatsStatusMax = GetNatsStatusRange().second;

	return(NatsStatusMax);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool IsValidNatsStatus(natsStatus nats_code)
{
	return(GetNatsStatusSet().find(nats_code) != GetNatsStatusSet().end());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
natsStatus CheckIsValidNatsStatus(natsStatus nats_code)
{
	if (IsValidNatsStatus(nats_code))
		return(nats_code);

	throw std::invalid_argument("The value " + std::to_string(nats_code) +
		" is not a valid natsStatus.");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const char *GetNatsStatusTextPtr(natsStatus nats_code)
{
	return((IsValidNatsStatus(nats_code)) ?
		::natsStatus_GetText(nats_code) : nullptr);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const char *GetNatsStatusText(natsStatus nats_code)
{
	const char *nats_text = GetNatsStatusTextPtr(nats_code);

	return((nats_text && *nats_text) ? nats_text : "*UNKNOWN-NATS-TEXT*");
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

#include <iomanip>
#include <iostream>

namespace {

// ////////////////////////////////////////////////////////////////////////////
void TEST_NatsStatus()
{
	using namespace MLB::NatsWrapper;

	for (natsStatus nats_code = GetNatsStatusMin();
		nats_code <= GetNatsStatusMax(); ) {
		std::cout << "NatsStatus " << std::setw(5) << nats_code << ": ";
		if (IsValidNatsStatus(nats_code))
			std::cout << GetNatsStatusTextPtr(nats_code) << '\n';
		else
			std::cout << "*NOT-VALID*\n";
		nats_code = static_cast<natsStatus>(static_cast<int>(nats_code) + 1);
	}
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		TEST_NatsStatus();
	}
	catch (const std::exception &except) {
		return_code = EXIT_FAILURE;
		std::cerr << "\n\nERROR: " << except.what() << std::endl;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

