// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB NatsWrapper Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  NatsMsg.hpp

   File Description  :  Include file for the NatsMsg class.

   Revision History  :  2024-08-17 --- Creation.
                           Michael L. Brock

      Copyright Michael L. Brock 2024.
      Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
      http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__NatsWrapper__NatsMsg_hpp__HH

#define HH__MLB__NatsWrapper__NatsMsg_hpp__HH 1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
   \file NatsMsg.hpp

   \brief   Main include file for the NatsMsg class.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <NatsWrapper/NatsSubscription.hpp>

#include <string>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace NatsWrapper {

// ////////////////////////////////////////////////////////////////////////////
class NatsMsg
{
	NatsMsg(natsMsg *nats_msg);

	friend NatsMsg NatsSubscription::NextMsg(int64_t time_out);

public:
	NatsMsg(NatsSubscription &nats_subs, int64_t time_out);

	virtual ~NatsMsg();

	static NatsMsg Create(const char *subject,
		const void *data_ptr, std::size_t data_length,
		const char *reply = nullptr);
	static NatsMsg Create(const std::string &subject,
		const std::string &data,
		const std::string &reply = "");

	/// Wrap a raw natsMsg* received in a natsMsgHandler callback.
	/// Takes ownership: the message will be destroyed when NatsMsg
	/// goes out of scope. The caller must NOT call natsMsg_Destroy().
	static NatsMsg FromRaw(natsMsg *msg);

	const char *GetSubject() const;
	const char *GetReply() const;
	const char *GetData() const;
	int         GetDataLength() const;

	bool        IsNoResponders() const;

	void        SetHeader(const char *key, const char *value);
	void        SetHeader(const std::string &key, const std::string &value);

	void        AddHeader(const char *key, const char *value);
	void        AddHeader(const std::string &key, const std::string &value);

	std::string GetHeader(const char *key) const;
	std::string GetHeader(const std::string &key) const;

	bool        HasHeader(const char *key) const;
	bool        HasHeader(const std::string &key) const;

	void        DeleteHeader(const char *key);
	void        DeleteHeader(const std::string &key);

	      natsMsg    *GetPtr();
	const natsMsg    *GetPtr() const;
	      natsMsg    *GetPtrChecked();
	const natsMsg    *GetPtrChecked() const;

private:
	std::shared_ptr<natsMsg> nats_msg_sptr_;
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace NatsWrapper

} // namespace MLB

#endif // #ifndef HH__MLB__NatsWrapper__NatsMsg_hpp__HH

