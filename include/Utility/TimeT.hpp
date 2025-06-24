// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  TimeT.hpp

   File Description  :  Definition of the TimeT class.

   Revision History  :  1998-04-08 --- Creation
                           Michael L. Brock
                        2023-01-04 --- Migration to C++ MlbDev2/Utility.
                           Michael L. Brock

      Copyright Michael L. Brock 1998 - 2023.
      Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
      http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__Utility__TimeT_hpp__HH

#define HH__MLB__Utility__TimeT_hpp__HH   1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
   \file TimeT.hpp

   \brief   The TimeT class definition include file.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/TimeSupport.hpp>

#include <ostream>
#include <string>

#include <boost/config.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
struct TimeTM;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/**
	\brief A wrapper class around \c time_t.
*/
struct API_UTILITY TimeT {
	         TimeT();
	explicit TimeT(const time_t &in_time);
	explicit TimeT(const std::string &in_date);

	~TimeT();

#if defined(BOOST_CXX_VERSION) && (BOOST_CXX_VERSION >= 201703L)
	constexpr auto operator <=> (const TimeT &other) const = default;
	constexpr bool operator ==  (const TimeT &other) const = default;
#else
	constexpr bool operator <  (const TimeT &other) const
	{
		return(Compare(*this, other) <  0);
	}
	constexpr bool operator >  (const TimeT &other) const
	{
		return(Compare(*this, other) >  0);
	}
	constexpr bool operator <= (const TimeT &other) const
	{
		return(Compare(*this, other) <= 0);
	}
	constexpr bool operator >= (const TimeT &other) const
	{
		return(Compare(*this, other) >= 0);
	}
	constexpr bool operator == (const TimeT &other) const
	{
		return(Compare(*this, other) == 0);
	}
	constexpr bool operator != (const TimeT &other) const
	{
		return(Compare(*this, other) != 0);
	}
#endif // #if defined(BOOST_CXX_VERSION) && (BOOST_CXX_VERSION >= 201703L)

	constexpr int Compare(const TimeT &other) const
	{
		return(Compare(*this, other));
	}

	TimeT &SetToNow();
	TimeT &SetToMinimumValue();
	TimeT &SetToMaximumValue();

	bool IsZero() const;

	TimeT &AddSeconds(int secs_to_add);
	TimeT &AddMilliseconds(int msecs_to_add);
	TimeT &AddMicroseconds(int usecs_to_add);
	TimeT &AddNanoseconds(int nsecs_to_add);

	char        *ToString(char *buffer,
		std::size_t max_length = Length_TimeT) const;
	std::string  ToString(std::size_t max_length = Length_TimeT) const;
	std::string &ToString(std::string &out_string,
		std::size_t max_length = Length_TimeT) const;

	char        *ToStringLocal(char *buffer,
		std::size_t max_length = Length_TimeT) const;
	std::string  ToStringLocal(std::size_t max_length = Length_TimeT) const;
	std::string &ToStringLocal(std::string &out_string,
		std::size_t max_length = Length_TimeT) const;

	char        *ToStringInterval(char *buffer,
		std::size_t max_length = LengthInterval_TimeT) const;
	std::string  ToStringInterval(
		std::size_t max_length = LengthInterval_TimeT) const;
	std::string &ToStringInterval(std::string &out_string,
		std::size_t max_length = LengthInterval_TimeT) const;

	const time_t &GetUnderlyingRef() const
	{
		return(datum_);
	}
	time_t &GetUnderlyingRef()
	{
		return(datum_);
	}
	time_t GetUnderlying() const
	{
		return(datum_);
	}

	static TimeT FromString(const std::string &in_date);
	static TimeT FromString(const char *in_date);

	static TimeT Now();

	static constexpr int Compare(const TimeT &lhs, const TimeT &rhs)
	{
		return(
			((int) (lhs.datum_  > rhs.datum_)  ?  1 :
					((lhs.datum_  < rhs.datum_)  ? -1 : 0)));
	}
	//	Used to support a C-style interface...
	static constexpr int Compare(const TimeT *lhs, const TimeT *rhs)
	{
		return(Compare(*lhs, *rhs));
	}

	static TimeT GetMinimumValue();
	static TimeT GetMaximumValue();

	time_t datum_;

private:
	char *FormatString(const TimeTM &in_tm, char *buffer,
		std::size_t max_length = Length_TimeT) const;
	char *FormatStringInterval(char *buffer,
		std::size_t max_length = LengthInterval_TimeT) const;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
API_UTILITY std::ostream & operator << (std::ostream &o_str,
	const TimeT &datum);
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__TimeT_hpp__HH

