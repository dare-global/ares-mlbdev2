// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB NatsWrapper Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  NatsConnection.hpp

   File Description  :  Include file for the NatsConnection class.

   Revision History  :  2024-08-17 --- Creation.
                           Michael L. Brock

      Copyright Michael L. Brock 2024.
      Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
      http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__NatsWrapper__NatsConnection_hpp__HH

#define HH__MLB__NatsWrapper__NatsConnection_hpp__HH 1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
   \file NatsConnection.hpp

   \brief   Main include file for the NatsConnection class.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <NatsWrapper/NatsOptions.hpp>

#include <NatsWrapper/NatsSubscription.hpp>

#include <expected>
#include <string>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace NatsWrapper {

// ////////////////////////////////////////////////////////////////////////////
class NatsConnection
{
public:
	NatsConnection(NatsOptions &nats_options);
	NatsConnection(const char *urls, std::size_t urls_length);
	NatsConnection(const std::string_view &urls);
	NatsConnection(const std::string &urls);
	NatsConnection(const char *urls);

	virtual ~NatsConnection();

	      natsConnection *GetPtr();
	const natsConnection *GetPtr() const;

	void Close();

	void Destroy();

	void FlushTimeout(int64_t time_out);

	void Publish(const char *subject_name, std::size_t subject_name_length,
		const void *data_ptr, std::size_t data_length);
	void Publish(const std::string_view &subject_name, const void *data_ptr,
		std::size_t data_length);
	void Publish(const std::string &subject_name, const void *data_ptr,
		std::size_t data_length);
	void Publish(const char *subject_name, const void *data_ptr,
		std::size_t data_length);

	void PublishString(const char *subject_name, const char *str);
	void PublishString(const std::string &subject_name, const std::string &str);

	void PublishMsg(NatsMsg &msg);

	std::expected<void, natsStatus> TryPublish(const char *subject_name,
		const void *data_ptr, std::size_t data_length);
	std::expected<void, natsStatus> TryPublish(const std::string &subject_name,
		const void *data_ptr, std::size_t data_length);
	std::expected<void, natsStatus> TryPublishMsg(NatsMsg &msg);

	void PublishRequest(const char *send_subject, const char *reply_subject,
		const void *data_ptr, std::size_t data_length);
	void PublishRequest(const std::string &send_subject,
		const std::string &reply_subject, const void *data_ptr,
		std::size_t data_length);

	void PublishRequestString(const char *send_subject,
		const char *reply_subject, const char *str);
	void PublishRequestString(const std::string &send_subject,
		const std::string &reply_subject, const std::string &str);

	NatsSubscription Subscribe(const char *subject_name,
		natsMsgHandler call_back, void *closure);
	NatsSubscription Subscribe(const std::string &subject_name,
		natsMsgHandler call_back, void *closure);

	NatsSubscription SubscribeTimeout(const char *subject_name,
		int64_t time_out, natsMsgHandler call_back, void *closure);
	NatsSubscription SubscribeTimeout(const std::string &subject_name,
		int64_t time_out, natsMsgHandler call_back, void *closure);

	NatsSubscription SubscribeSync(const char *subject_name);
	NatsSubscription SubscribeSync(const std::string &subject_name);

	natsConnStatus GetStatus() const;

	static std::expected<NatsConnection, natsStatus> TryConnect(
		NatsOptions &nats_options);
	static NatsConnection ConnectTo(const char *urls, std::size_t urls_length);
	static NatsConnection ConnectTo(const std::string &urls);
	static NatsConnection ConnectTo(const char *urls);

private:
	NatsConnection(natsConnection *nats_conn);

	std::shared_ptr<natsConnection> nats_connection_sptr_;
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace NatsWrapper

} // namespace MLB

#endif // #ifndef HH__MLB__NatsWrapper__NatsConnection_hpp__HH

