// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB NatsWrapper Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  NatsInbox.hpp

   File Description  :  Include file for the NatsInbox class.

   Revision History  :  2024-09-16 --- Creation.
                           Michael L. Brock

      Copyright Michael L. Brock 2024.
      Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
      http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__NatsWrapper__NatsInbox_hpp__HH

#define HH__MLB__NatsWrapper__NatsInbox_hpp__HH 1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
   \file NatsInbox.hpp

   \brief   Main include file for the NatsInbox class.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <NatsWrapper/NatsSubscription.hpp>

//#include <string>
#include <string_view>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace NatsWrapper {

// ////////////////////////////////////////////////////////////////////////////
class NatsInbox
{
public:
	NatsInbox();

	virtual ~NatsInbox();

	      natsInbox        *GetPtr();
	const natsInbox        *GetPtr() const;
	      std::string       GetInboxAsString() const;
	      std::string_view  GetInboxAsStringView() const;

private:
	std::shared_ptr<natsInbox> nats_inbox_sptr_;
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace NatsWrapper

} // namespace MLB

#endif // #ifndef HH__MLB__NatsWrapper__NatsInbox_hpp__HH

