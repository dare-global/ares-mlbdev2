// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  VersionNumber.hpp

   File Description  :  Include file for the VersionNumber class.

   Revision History  :  1996-03-11 --- Creation in MlbDevOld/genfuncs library
                                       file genfuncs/source/version.c.
                           Michael L. Brock
                        1998-04-08 --- Implementation in MlbDev/Utility in
                                       file Utility/VersionNumber.cpp.
                           Michael L. Brock
                        2024-08-28 --- Migration to C++ MlbDev2/Utility.
                           Michael L. Brock
 
      Copyright Michael L. Brock 1996 - 2024.
      Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
      http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__Utility__VersionNumber_hpp__HH

#define HH__MLB__Utility__VersionNumber_hpp__HH 1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
   \file VersionNumber.hpp

   \brief   The MLB VersionNumber header file.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/Utility.hpp>

#include <compare>
#include <cstdint>
#include <iostream>
#include <string>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
struct API_UTILITY VersionNumber {
	enum class VersionNumberIndex {
		Major   = 0,
		Minor   = 1,
		Release = 2,
		Build   = 3,
		Count,
		Minimum = Major,
		Maximum = Build
	};

	static const std::size_t VersionElementCount =
		static_cast<std::size_t>(VersionNumberIndex::Count);

	explicit VersionNumber(uint32_t version_major = 0,
		uint32_t version_minor = 0, uint32_t version_release = 0,
		uint32_t version_build = 0);
	explicit VersionNumber(const std::string &version_number);

	void swap(VersionNumber &other);

	uint32_t GetMajor() const;
	uint32_t GetMinor() const;
	uint32_t GetRelease() const;
	uint32_t GetBuild() const;

	uint32_t SetMajor(uint32_t version_major);
	uint32_t SetMinor(uint32_t version_minor);
	uint32_t SetRelease(uint32_t version_release);
	uint32_t SetBuild(uint32_t version_build);

	VersionNumber &SetToMinimumValue();
	VersionNumber &SetToMaximumValue();

	constexpr auto operator<=>(const VersionNumber &other) const = default;
	constexpr bool operator==(const VersionNumber &other) const = default;

	constexpr int Compare(const VersionNumber &other) const
	{
		return(Compare(*this, other));
	}

	std::ostream &ToStream(std::ostream &o_str = std::cout) const;

	std::string   ToString() const;
	std::string  &ToString(std::string &out_string) const;

	static constexpr int Compare(const VersionNumber &lhs,
		const VersionNumber &rhs)
	{
		if (lhs.version_[0] < rhs.version_[0])
			return(-1);
		else if (lhs.version_[0] > rhs.version_[0])
			return(1);
		else if (lhs.version_[1] < rhs.version_[1])
			return(-1);
		else if (lhs.version_[1] > rhs.version_[1])
			return(1);
		else if (lhs.version_[2] < rhs.version_[2])
			return(-1);
		else if (lhs.version_[2] > rhs.version_[2])
			return(1);
		else if (lhs.version_[3] < rhs.version_[3])
			return(-1);
		else if (lhs.version_[3] > rhs.version_[3])
			return(1);

		return(0);
	}

	static uint32_t IdxToUInt(VersionNumberIndex element_index);

	static VersionNumber FromString(const std::string &in_datum,
		bool require_all_segments = true);
	static VersionNumber FromString(const char *in_datum,
		bool require_all_segments = true);

	static VersionNumber GetMinimumValue();
	static VersionNumber GetMaximumValue();

	static const char *GetElementName(VersionNumberIndex element_index);
	static const char *GetElementName(std::size_t element_index);

	uint32_t version_[VersionElementCount];
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
API_UTILITY std::ostream & operator << (std::ostream &o_str,
	const VersionNumber &datum);
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__VersionNumber_hpp__HH

