// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB Utility Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  VersionNumber.cpp

   File Description  :  Implementation of the VersionNumber class.

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

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/VersionNumber.hpp>

#include <Utility/ArgCheck.hpp>
#include <Utility/StringSplit.hpp>

#include <cmath>
#include <sstream>

#ifdef __GNUC__
# include <string.h>
# include <stdlib.h>
#endif // #ifdef __GNUC__

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
VersionNumber::VersionNumber(uint32_t version_major,
	uint32_t version_minor, uint32_t version_release,
	uint32_t version_build)
{
	version_[IdxToUInt(VersionNumberIndex::Major)]   = version_major;
	version_[IdxToUInt(VersionNumberIndex::Minor)]   = version_minor;
	version_[IdxToUInt(VersionNumberIndex::Release)] = version_release;
	version_[IdxToUInt(VersionNumberIndex::Build)]   = version_build;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
VersionNumber::VersionNumber(const std::string &version_number)
{
	*this = FromString(version_number);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void VersionNumber::swap(VersionNumber &other)
{
	uint32_t tmp_version[sizeof(version_) / sizeof(version_[0])];

	::memcpy(tmp_version, version_, sizeof(version_));
	::memcpy(version_, other.version_, sizeof(version_));
	::memcpy(other.version_, tmp_version, sizeof(version_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
uint32_t VersionNumber::GetMajor() const
{
	return(version_[IdxToUInt(VersionNumberIndex::Major)]);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
uint32_t VersionNumber::GetMinor() const
{
	return(version_[IdxToUInt(VersionNumberIndex::Minor)]);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
uint32_t VersionNumber::GetRelease() const
{
	return(version_[IdxToUInt(VersionNumberIndex::Release)]);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
uint32_t VersionNumber::GetBuild() const
{
	return(version_[IdxToUInt(VersionNumberIndex::Build)]);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
uint32_t VersionNumber::SetMajor(uint32_t version_major)
{
	unsigned old_value = version_[IdxToUInt(VersionNumberIndex::Major)];

	version_[IdxToUInt(VersionNumberIndex::Major)]   = version_major;

	return(old_value);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
uint32_t VersionNumber::SetMinor(uint32_t version_minor)
{
	unsigned old_value = version_[IdxToUInt(VersionNumberIndex::Minor)];

	version_[IdxToUInt(VersionNumberIndex::Minor)]   = version_minor;

	return(old_value);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
uint32_t VersionNumber::SetRelease(uint32_t version_release)
{
	unsigned old_value = version_[IdxToUInt(VersionNumberIndex::Release)];

	version_[IdxToUInt(VersionNumberIndex::Release)] = version_release;

	return(old_value);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
uint32_t VersionNumber::SetBuild(uint32_t version_build)
{
	unsigned old_value = version_[IdxToUInt(VersionNumberIndex::Build)];

	version_[IdxToUInt(VersionNumberIndex::Build)]   = version_build;

	return(old_value);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
VersionNumber &VersionNumber::SetToMinimumValue()
{
	version_[IdxToUInt(VersionNumberIndex::Major)]   = 0;
	version_[IdxToUInt(VersionNumberIndex::Minor)]   = 0;
	version_[IdxToUInt(VersionNumberIndex::Release)] = 0;
	version_[IdxToUInt(VersionNumberIndex::Build)]   = 0;

	return(*this);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
VersionNumber &VersionNumber::SetToMaximumValue()
{
	version_[IdxToUInt(VersionNumberIndex::Major)]   =
		std::numeric_limits<uint32_t>::max();
	version_[IdxToUInt(VersionNumberIndex::Minor)]   =
		std::numeric_limits<uint32_t>::max();
	version_[IdxToUInt(VersionNumberIndex::Release)] =
		std::numeric_limits<uint32_t>::max();
	version_[IdxToUInt(VersionNumberIndex::Build)]   =
		std::numeric_limits<uint32_t>::max();

	return(*this);
}
// ////////////////////////////////////////////////////////////////////////////

#if 0
// ////////////////////////////////////////////////////////////////////////////
constexpr bool VersionNumber::operator < (const VersionNumber &other) const
{
// return(::memcmp(version_, other.version_, sizeof(version_)) < 0);

	return(Compare(other) < 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
constexpr bool VersionNumber::operator > (const VersionNumber &other) const
{
// return(::memcmp(version_, other.version_, sizeof(version_)) > 0);

	return(Compare(other) > 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
constexpr bool VersionNumber::operator <= (const VersionNumber &other) const
{
// return(::memcmp(version_, other.version_, sizeof(version_)) <= 0);

	return(Compare(other) <= 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
constexpr bool VersionNumber::operator >= (const VersionNumber &other) const
{
// return(::memcmp(version_, other.version_, sizeof(version_)) >= 0);

	return(Compare(other) >= 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
constexpr bool VersionNumber::operator == (const VersionNumber &other) const
{
// return(::memcmp(version_, other.version_, sizeof(version_)) == 0);

	return(Compare(other) == 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
constexpr bool VersionNumber::operator != (const VersionNumber &other) const
{
// return(::memcmp(version_, other.version_, sizeof(version_)) != 0);

	return(Compare(other) != 0);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
constexpr int VersionNumber::Compare(const VersionNumber &other) const
{
	return(Compare(*this, other));
}
// ////////////////////////////////////////////////////////////////////////////
#endif // #if 0

// ////////////////////////////////////////////////////////////////////////////
std::ostream &VersionNumber::ToStream(std::ostream &o_str) const
{
	o_str
		<< version_[IdxToUInt(VersionNumberIndex::Major)]   << '.'
		<< version_[IdxToUInt(VersionNumberIndex::Minor)]   << '.'
		<< version_[IdxToUInt(VersionNumberIndex::Release)] << '.'
		<< version_[IdxToUInt(VersionNumberIndex::Build)]
		;

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string VersionNumber::ToString() const
{
	std::string out_string;

	return(ToString(out_string));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &VersionNumber::ToString(std::string &out_string) const
{
	std::ostringstream o_str;

	ToStream(o_str);

	out_string = o_str.str();

	return(out_string);
}
// ////////////////////////////////////////////////////////////////////////////

#if 0
// ////////////////////////////////////////////////////////////////////////////
constexpr int VersionNumber::Compare(const VersionNumber &lhs,
	const VersionNumber &rhs)
{
/*
	int64_t cmp;

	for (std::size_t element_idx = 0; element_idx < VersionElementCount;
		++element_idx) {
		cmp = static_cast<int64_t>(lhs.version_[element_idx]) -
				static_cast<int64_t>(rhs.version_[element_idx]);
		if (cmp)
			return((cmp < 0LL) ? -1 : 1);
	}

	return(0);
*/
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
// ////////////////////////////////////////////////////////////////////////////
#endif // #if 0

// ////////////////////////////////////////////////////////////////////////////
uint32_t VersionNumber::IdxToUInt(VersionNumberIndex element_index)
{
	return(static_cast<std::size_t>(element_index));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
VersionNumber VersionNumber::FromString(const std::string &in_datum,
	bool require_all_segments)
{
	return(FromString(in_datum.c_str(), require_all_segments));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
VersionNumber VersionNumber::FromString(const char *in_datum,
	bool require_all_segments)
{
/*
	if (in_datum == NULL)
		ThrowInvalidArgument("Version string pointer is 'NULL'.");

	if (!(*in_datum))
		ThrowInvalidArgument("Version string is empty.");
*/
	ThrowIfNullOrEmpty(in_datum, "Version string");

	std::vector<std::string> out_list;

	SplitString(in_datum, ".", out_list);

	if (out_list.size() > 4)
		throw std::invalid_argument("Version string '" + std::string(in_datum) +
			"' number of elements (" + std::to_string(out_list.size()) +
			") exceeds the maximum (4).");

	if (require_all_segments && (out_list.size() < 4))
		throw std::invalid_argument("Version string '" + std::string(in_datum) +
			"' number of elements (" + std::to_string(out_list.size()) +
			") is not equal to 4.");

	VersionNumber                       out_datum;
	std::vector<std::string>::size_type count_1;

	for (count_1 = 0; count_1 < out_list.size(); ++count_1) {
		char       *end_ptr;
		const char *start_ptr  = out_list[count_1].c_str();
		double      tmp_double = ::strtod(start_ptr, &end_ptr);
		if (end_ptr != (start_ptr + ::strlen(start_ptr)))
			throw std::invalid_argument("Version string '" +
				std::string(in_datum) + "' " +
				GetElementName(static_cast<uint32_t>(count_1)) +
				" element contains at least one invalid character (ASCII " +
				std::to_string(static_cast<uint32_t>(*end_ptr)) + ").");
		else if ((tmp_double <
			static_cast<double>(std::numeric_limits<uint32_t>::min())) ||
			(tmp_double >
			static_cast<double>(std::numeric_limits<uint32_t>::max())))
			throw std::invalid_argument("Version string '" +
				std::string(in_datum) + "' " +
				GetElementName(static_cast<uint32_t>(count_1)) +
				" element is outside of the permissible range (from " +
				std::to_string(std::numeric_limits<uint32_t>::min()) + " to " +
				std::to_string(std::numeric_limits<uint32_t>::max()) + " to " +
				", inclusive).");
		else if (tmp_double != ::floor(tmp_double))
			throw std::invalid_argument("Version string '" +
				std::string(in_datum) + "' " +
				GetElementName(static_cast<uint32_t>(count_1)) +
				" element is not an integral value.");
		out_datum.version_[count_1] = static_cast<uint32_t>(tmp_double);
	}

	return(out_datum);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
VersionNumber VersionNumber::GetMinimumValue()
{
	return(VersionNumber().SetToMinimumValue());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
VersionNumber VersionNumber::GetMaximumValue()
{
	return(VersionNumber().SetToMaximumValue());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const char *VersionNumber::GetElementName(VersionNumberIndex element_index)
{
	return(GetElementName(IdxToUInt(element_index)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const char *VersionNumber::GetElementName(std::size_t element_index)
{
	return((element_index == 0) ? "major" :
		((element_index == 1) ? "minor" :
		((element_index == 2) ? "release" :
		((element_index == 3) ? "build" : "*** INVALID ***"))));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const VersionNumber &datum)
{
	return(datum.ToStream(o_str));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

