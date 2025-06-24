// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  UniqueId.hpp

   File Description  :  Include file for the UniqueId class.

   Revision History  :  1998-04-08 --- Creation.
                           Michael L. Brock
                        2024-09-12 --- Added to MlbDev2 Utility library.
                           Michael L. Brock

      Copyright Michael L. Brock 1998 - 2024.
      Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
      http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__Utility__UniqueId_hpp__HH

#define HH__MLB__Utility__UniqueId_hpp__HH   1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
   \file UniqueId.hpp

   \brief   The MLB UniqueId header file.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/Utility.hpp>

#if 0
#include <Utility/IntToString.hpp>
#endif // #if 0

#include <iostream>
#include <string>
#include <vector>

#include <boost/version.hpp>

#if BOOST_VERSION < 104200
# error "Boost version is less that 1.42!"
#endif // # if BOOST_VERSION < 104200

#include <boost/uuid/uuid.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
typedef boost::uuids::uuid NativeUuidType;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// The length of a unique identifier as a human-readable string (terminating
// ASCII NUL character not included).
// ////////////////////////////////////////////////////////////////////////////
const unsigned int UniqueIdStringLength = 36;
// ////////////////////////////////////////////////////////////////////////////

#if 0
// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// The length of a unique identifier as a human-readable string in radix 64
// (terminating ASCII NUL character not included).
// ////////////////////////////////////////////////////////////////////////////
const unsigned int UniqueIdStringLengthRadix64 =
	(sizeof(NativeUuidType) / sizeof(long)) * IntToString_MaxLen_Unsigned_64;
// ////////////////////////////////////////////////////////////////////////////
#endif // #if 0

// ////////////////////////////////////////////////////////////////////////////
/**
	\brief A wrapper class for Universally Unique IDentifier (UUID).

	This class wraps the UUID specified in RFC 4122.
*/
struct API_UTILITY UniqueId {
	         UniqueId();
	explicit UniqueId(bool generate_uuid_flag);
	explicit UniqueId(const char *unique_id);
	explicit UniqueId(const std::string &unique_id);

#if defined(BOOST_CXX_VERSION) && (BOOST_CXX_VERSION >= 201703L)
	constexpr auto operator <=> (const UniqueId &other) const = default;
	constexpr bool operator ==  (const UniqueId &other) const = default;
#else
	bool operator  < (const UniqueId &other) const
	{
		return(this->id_ < other.id_);
	}
	bool operator == (const UniqueId &other) const
	{
		return(this->id_ == other.id_);
	}
	bool operator != (const UniqueId &other) const
	{
		return(this->id_ != other.id_);
	}
#endif // #if defined(BOOST_CXX_VERSION) && (BOOST_CXX_VERSION >= 201703L)

	std::ostream    &ToStream(std::ostream &o_str = std::cout) const;
	std::istream    &FromStream(std::istream &i_str = std::cin);

	std::string      ToString() const;
	std::string     &ToString(std::string &out_string) const;
	char            *ToString(char *out_string) const;

	std::string      ToStringByteOrder() const;
	std::string     &ToStringByteOrder(std::string &out_string) const;
	char            *ToStringByteOrder(char *out_string) const;

#if 0
	std::string      ToStringRadix64() const;
	std::string     &ToStringRadix64(std::string &out_string) const;
	char            *ToStringRadix64(char *out_string) const;
#endif // #if 0
	
	static UniqueId  FromString(const std::string &uuid_string);
	static UniqueId  FromString(const char *uuid_string);

	/**
		Returns true if all 16 bytes of the UUID are zero.
	*/
	bool IsZero() const;

	void swap(UniqueId &other);

	static NativeUuidType GetSystemUUID();

	NativeUuidType id_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
API_UTILITY std::ostream & operator << (std::ostream &o_str,
	const UniqueId &datum);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ContainerType>
	API_UTILITY ContainerType &CreateUniqueIdContainer(ContainerType &dst,
		std::size_t element_count)
{
	auto inserter(std::inserter(dst, dst.end()));

	while (element_count--)
		*inserter++ = UniqueId();

	return(dst);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ContainerType>
	API_UTILITY ContainerType CreateUniqueIdContainer(std::size_t element_count)
{
	ContainerType dst;

	return(CreateUniqueIdContainer(dst, element_count));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__UniqueId_hpp__HH

