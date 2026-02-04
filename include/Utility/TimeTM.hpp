// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  TimeTM.hpp

   File Description  :  Definition of the TimeTM class.

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

#ifndef HH__MLB__Utility__TimeTM_hpp__HH

#define HH__MLB__Utility__TimeTM_hpp__HH  1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
   \file TimeTM.hpp

   \brief   The TimeTM class definition include file.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/TimeSupport.hpp>

#include <compare>
#include <string>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
/**
	\brief A wrapper class around \c struct \c tm.

	The wrapper is implemented by public inheritance from \c struct \c tm ,
	so substitutability is supported.
*/
struct API_UTILITY TimeTM : public tm {
	TimeTM();
	explicit TimeTM(const tm &in_time);
	~TimeTM();

	constexpr std::strong_ordering operator<=>(const TimeTM &other) const
	{
		const int cmp = Compare(*this, other);
		return (cmp < 0) ? std::strong_ordering::less :
		       (cmp > 0) ? std::strong_ordering::greater :
		                   std::strong_ordering::equal;
	}
	constexpr bool operator==(const TimeTM &other) const
	{
		return Compare(*this, other) == 0;
	}

	constexpr int Compare(const TimeTM &other) const
	{
		return(Compare(*this, other));
	}

	char        *AscTime(char *buffer) const;
	std::string  AscTime() const;
	char        *AscTimeSane(char *buffer) const;
	std::string  AscTimeSane() const;

	char        *ToString(char *buffer,
		std::size_t max_length = Length_TimeTM) const;
	std::string  ToString(std::size_t max_length = Length_TimeTM) const;
	std::string &ToString(std::string &out_string,
		std::size_t max_length = Length_TimeTM) const;

	const struct tm &GetUnderlyingRef() const
	{
		return(*this);
	}
	struct tm &GetUnderlyingRef()
	{
		return(*this);
	}
	struct tm GetUnderlying() const
	{
		return(*this);
	}

	static TimeTM Now();

	static TimeTM TimeUTC(const time_t in_time = time(NULL));
	static TimeTM TimeLocal(const time_t in_time = time(NULL));

	static constexpr int Compare(const TimeTM &lhs, const TimeTM &rhs)
	{
		return(
			((int) (lhs.tm_year > rhs.tm_year) ?  1 :
					((lhs.tm_year < rhs.tm_year) ? -1 :
					((lhs.tm_mon  > rhs.tm_mon)  ?  1 :
					((lhs.tm_mon  < rhs.tm_mon)  ? -1 :
					((lhs.tm_mday > rhs.tm_mday) ?  1 :
					((lhs.tm_mday < rhs.tm_mday) ? -1 :
					((lhs.tm_hour > rhs.tm_hour) ?  1 :
					((lhs.tm_hour < rhs.tm_hour) ? -1 :
					((lhs.tm_min  > rhs.tm_min)  ?  1 :
					((lhs.tm_min  < rhs.tm_min)  ? -1 :
					((lhs.tm_sec  > rhs.tm_sec)  ?  1 :
					((lhs.tm_sec  < rhs.tm_sec)  ? -1 : 0)))))))))))));
	}
	//	Used to support a C-style interface...
	static constexpr int Compare(const TimeTM *lhs, const TimeTM *rhs)
	{
		return(Compare(*lhs, *rhs));
	}
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
API_UTILITY std::ostream & operator << (std::ostream &o_str,
	const TimeTM &datum);
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__TimeTM_hpp__HH

