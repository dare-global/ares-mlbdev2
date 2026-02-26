// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB NatsWrapper Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  NatsConnection.cpp

   File Description  :  Implementation of the NatsConnection class.

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

#include <NatsWrapper/NatsConnection.hpp>

#include <NatsWrapper/NatsMsg.hpp>
#include <NatsWrapper/NatsSubscription.hpp>

#include <Utility/ArgCheck.hpp>

#include <cstring>
#include <limits>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace NatsWrapper {

// ////////////////////////////////////////////////////////////////////////////
class NatsSubscription;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// PRIVATE: Used by TryConnect()
NatsConnection::NatsConnection(natsConnection *nats_conn)
	:nats_connection_sptr_()
{
	if (nats_conn)
		nats_connection_sptr_.reset(nats_conn, ::natsConnection_Destroy);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
NatsConnection::NatsConnection(NatsOptions &nats_options)
	:nats_connection_sptr_()
{
	natsConnection *nats_conn = NULL;

	NatsWrapper_THROW_IF_NOT_OK(::natsConnection_Connect,
		(&nats_conn, nats_options.GetPtr()))

	nats_connection_sptr_.reset(nats_conn, ::natsConnection_Destroy);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
NatsConnection::NatsConnection(const char *urls, std::size_t urls_length)
	:nats_connection_sptr_()
{
	MLB::Utility::ThrowIfNull(urls, "The pointer to the URLs string");

	if (!urls_length)
		throw std::invalid_argument("The length of the URLs string is 0.");

	natsConnection *nats_conn = NULL;

	NatsWrapper_THROW_IF_NOT_OK(::natsConnection_ConnectTo, (&nats_conn, urls))

	nats_connection_sptr_.reset(nats_conn, ::natsConnection_Destroy);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
NatsConnection::NatsConnection(const std::string_view &urls)
	:NatsConnection(urls.data(), urls.size())
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
NatsConnection::NatsConnection(const std::string &urls)
	:NatsConnection(urls.c_str(), urls.size())
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
NatsConnection::NatsConnection(const char *urls)
	:NatsConnection(MLB::Utility::ThrowIfNull(urls, "The URLs string"),
		::strlen(urls))
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
NatsConnection::~NatsConnection()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
natsConnection *NatsConnection::GetPtr()
{
	return(nats_connection_sptr_.get());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const natsConnection *NatsConnection::GetPtr() const
{
	return(nats_connection_sptr_.get());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void NatsConnection::Close()
{
	if (nats_connection_sptr_)
		natsConnection_Close(nats_connection_sptr_.get());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void NatsConnection::FlushTimeout(int64_t time_out)
{
	NatsWrapper_THROW_IF_NOT_OK(::natsConnection_FlushTimeout,
		(GetPtr(), time_out))
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void NatsConnection::Publish(const char *subject_name,
	std::size_t subject_name_length, const void *data_ptr,
	std::size_t data_length)
{
	MLB::Utility::ThrowIfNullOrEmpty(subject_name,
		"The send subject name on which data is to be published");

	if (!subject_name_length)
		throw std::invalid_argument("The length of the subject name on which "
			"data to be published is 0.");

	MLB::Utility::ThrowIfNull(data_ptr, "The pointer to data to be published");

/*
	if (!data_length)
		throw std::invalid_argument("The length of the data to be published "
			"is 0.");
*/

	if (data_length >
		 static_cast<std::size_t>(std::numeric_limits<int>::max()))
		throw std::invalid_argument("Length of the data to be published is "
			"greater than the maximum permissible by the data type NATS uses to "
			"specify the length of published data (" +
			std::to_string(std::numeric_limits<int>::max()) + ").");

	NatsWrapper_THROW_IF_NOT_OK(::natsConnection_Publish,
		(GetPtr(), subject_name, data_ptr, data_length))
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void NatsConnection::Publish(const std::string_view &subject_name,
	const void *data_ptr, std::size_t data_length)
{
	Publish(subject_name.data(), subject_name.size(), data_ptr, data_length);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void NatsConnection::Publish(const std::string &subject_name,
	const void *data_ptr, std::size_t data_length)
{
	Publish(subject_name.c_str(), subject_name.size(), data_ptr, data_length);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void NatsConnection::Publish(const char *subject_name, const void *data_ptr,
	std::size_t data_length)
{
	MLB::Utility::ThrowIfNull(subject_name,
		"The subject name on which to publish");

	Publish(subject_name, ::strlen(subject_name), data_ptr, data_length);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void NatsConnection::PublishString(const char *subject_name, const char *str)
{
	MLB::Utility::ThrowIfNullOrEmpty(subject_name,
		"The subject name on which data is to be published");

	MLB::Utility::ThrowIfNull(str, "The string to be published");

	NatsWrapper_THROW_IF_NOT_OK(::natsConnection_PublishString,
		(GetPtr(), subject_name, str))
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void NatsConnection::PublishString(const std::string &subject_name,
	const std::string &str)
{
	PublishString(subject_name.c_str(), str.c_str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void NatsConnection::PublishMsg(NatsMsg &msg)
{
	NatsWrapper_THROW_IF_NOT_OK(::natsConnection_PublishMsg,
		(GetPtr(), msg.GetPtrChecked()))
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::expected<void, natsStatus> NatsConnection::TryPublish(
	const char *subject_name, const void *data_ptr, std::size_t data_length)
{
	MLB::Utility::ThrowIfNullOrEmpty(subject_name,
		"The subject name on which data is to be published");

	MLB::Utility::ThrowIfNull(data_ptr, "The pointer to data to be published");

	if (data_length >
		 static_cast<std::size_t>(std::numeric_limits<int>::max()))
		throw std::invalid_argument("Length of the data to be published is "
			"greater than the maximum permissible by the data type NATS uses to "
			"specify the length of published data (" +
			std::to_string(std::numeric_limits<int>::max()) + ").");

	natsStatus s = ::natsConnection_Publish(GetPtr(), subject_name,
		data_ptr, data_length);

	if (s != NATS_OK)
		return std::unexpected(s);

	return {};
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::expected<void, natsStatus> NatsConnection::TryPublish(
	const std::string &subject_name, const void *data_ptr,
	std::size_t data_length)
{
	return TryPublish(subject_name.c_str(), data_ptr, data_length);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::expected<void, natsStatus> NatsConnection::TryPublishMsg(NatsMsg &msg)
{
	natsStatus s = ::natsConnection_PublishMsg(GetPtr(), msg.GetPtrChecked());

	if (s != NATS_OK)
		return std::unexpected(s);

	return {};
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void NatsConnection::PublishRequest(const char *send_subject,
	const char *reply_subject, const void *data_ptr, std::size_t data_length)
{
	MLB::Utility::ThrowIfNullOrEmpty(send_subject,
		"The send subject name on which data is to be published");

	MLB::Utility::ThrowIfNullOrEmpty(reply_subject,
		"The reply subject name on which data is to be received");

	MLB::Utility::ThrowIfNull(data_ptr, "The pointer to data to be published");

/*
	if (!data_length)
		throw std::invalid_argument("The length of the data to be published "
			"is 0.");
*/

	if (data_length >
		 static_cast<std::size_t>(std::numeric_limits<int>::max()))
		throw std::invalid_argument("Length of the data to be published is "
			"greater than the maximum permissible by the data type NATS uses to "
			"specify the length of published data (" +
			std::to_string(std::numeric_limits<int>::max()) + ").");

	NatsWrapper_THROW_IF_NOT_OK(::natsConnection_PublishRequest,
		(GetPtr(), send_subject, reply_subject, data_ptr, data_length))
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void NatsConnection::PublishRequest(const std::string &send_subject,
	const std::string &reply_subject, const void *data_ptr,
	std::size_t data_length)
{
	PublishRequest(send_subject.c_str(), reply_subject.c_str(),
		data_ptr, data_length);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void NatsConnection::PublishRequestString(const char *send_subject,
	const char *reply_subject, const char *str)
{
	MLB::Utility::ThrowIfNullOrEmpty(send_subject,
		"The send subject name on which data is to be published");

	MLB::Utility::ThrowIfNullOrEmpty(reply_subject,
		"The reply subject name on which data is to be received");

	MLB::Utility::ThrowIfNull(str, "The string to be published");

	NatsWrapper_THROW_IF_NOT_OK(::natsConnection_PublishRequestString,
		(GetPtr(), send_subject, reply_subject, str))
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void NatsConnection::PublishRequestString(const std::string &send_subject,
	const std::string &reply_subject, const std::string &str)
{
	PublishRequestString(send_subject.c_str(), reply_subject.c_str(),
		str.c_str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
NatsSubscription NatsConnection::Subscribe(const char *subject_name,
	natsMsgHandler call_back, void *closure)
{
	return(NatsSubscription(*this, subject_name, call_back, closure));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
NatsSubscription NatsConnection::Subscribe(const std::string &subject_name,
	natsMsgHandler call_back, void *closure)
{
	return(Subscribe(subject_name.c_str(), call_back, closure));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
NatsSubscription NatsConnection::SubscribeTimeout(const char *subject_name,
	int64_t time_out, natsMsgHandler call_back, void *closure)
{
	return(NatsSubscription(*this, subject_name, time_out, call_back, closure));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
NatsSubscription NatsConnection::SubscribeTimeout(
	const std::string &subject_name, int64_t time_out, natsMsgHandler call_back,
	void *closure)
{
	return(SubscribeTimeout(subject_name.c_str(), time_out,
		call_back, closure));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
NatsSubscription NatsConnection::SubscribeSync(const char *subject_name)
{
	return(NatsSubscription(*this, subject_name));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
NatsSubscription NatsConnection::SubscribeSync(const std::string &subject_name)
{
	return(SubscribeSync(subject_name.c_str()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
NatsConnection NatsConnection::ConnectTo(const char *urls,
	std::size_t urls_length)
{
	return(NatsConnection(urls, urls_length));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
NatsConnection NatsConnection::ConnectTo(const std::string &urls)
{
	return(NatsConnection(urls));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
NatsConnection NatsConnection::ConnectTo(const char *urls)
{
	return(NatsConnection(urls));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
natsConnStatus NatsConnection::GetStatus() const
{
	return(::natsConnection_Status(nats_connection_sptr_.get()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::expected<NatsConnection, natsStatus> NatsConnection::TryConnect(
	NatsOptions &nats_options)
{
	natsConnection *nats_conn = NULL;

	natsStatus s = ::natsConnection_Connect(&nats_conn, nats_options.GetPtr());

	if (s == NATS_OK || s == NATS_NOT_YET_CONNECTED)
		return NatsConnection(nats_conn);

	return std::unexpected(s);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace NatsWrapper

} // namespace MLB

