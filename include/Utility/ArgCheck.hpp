// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  ArgChecks.hpp

   File Description  :  Include file argument checking support logic.

   Revision History  :  1998-04-08 --- Creation of original ArgChecks()
                                       function templates.
                           Michael L. Brock
                        2023-01-06 --- Change names of core functions to
                                       ToString() and make ArgChecks()
                                       forward to those functions.
                           Michael L. Brock

      Copyright Michael L. Brock 1998 - 2023.
      Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
      http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__Utility__ArgChecks_hpp__HH

#define HH__MLB__Utility__ArgChecks_hpp__HH   1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
   \file ArgChecks.hpp

   \brief   The ArgChecks functions header file.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/ValueTraits.hpp>

#include <stdexcept>
#include <string>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
template <typename DatumType>
	inline API_UTILITY bool IsNull(const DatumType *test_datum)
{
	return(test_datum == NULL);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
inline API_UTILITY bool IsEmpty(const char *test_string)
{
	return((test_string == NULL) || (!(*test_string)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
inline API_UTILITY bool IsEmpty(const std::string &test_string)
{
	return(test_string.empty());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
inline API_UTILITY const char *NullToString(const char *test_string,
	const char *other_string = "")
{
	return((test_string != NULL) ? test_string :
		((other_string != NULL) ? other_string : ""));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
inline API_UTILITY const char *NullOrEmptyToString(const char *test_string,
	const char *other_string = "")
{
	return(((test_string != NULL) && *test_string) ? test_string :
		((other_string != NULL) ? other_string : ""));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
inline API_UTILITY std::string NullOrEmptyToString(
	const std::string &test_string, const std::string &other_string = "")
{
	return(NullOrEmptyToString(test_string.c_str(), other_string.c_str()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
inline API_UTILITY std::string NullOrEmptyToStringWithColon(
	const std::string &test_string, const std::string &other_string = "")
{
	const char *tmp_ptr = NullOrEmptyToString(test_string.c_str(),
		other_string.c_str());

	return((*tmp_ptr) ? (std::string(tmp_ptr) + ": ") : "");
}
// ////////////////////////////////////////////////////////////////////////////

namespace {

// ////////////////////////////////////////////////////////////////////////////
inline void ThrowIfSupport(const char *name_string,
	const char *error_string)
{
	if ((name_string == NULL) || (!(*name_string)))
		name_string = "Value";

	throw std::invalid_argument(std::string(name_string) +
		std::string(error_string));
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
template <typename DatumType>
	inline API_UTILITY const DatumType *ThrowIfNull(const DatumType *test_datum,
		const char *name_string = NULL)
{
	if (test_datum == NULL)
		ThrowIfSupport(name_string, " is a 'NULL' pointer.");

	return(test_datum);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
inline API_UTILITY const char *ThrowIfEmpty(const char *test_string,
	const char *name_string = NULL)
{
	if (!(*test_string))
		ThrowIfSupport(name_string, " is an empty string.");

	return(test_string);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
inline API_UTILITY const wchar_t *ThrowIfEmpty(const wchar_t *test_string,
	const char *name_string = NULL)
{
	if (!(*test_string))
		ThrowIfSupport(name_string, " is an empty string.");

	return(test_string);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
inline API_UTILITY const std::string &ThrowIfEmpty(
	const std::string &test_string, const char *name_string = NULL)
{
	ThrowIfEmpty(test_string.c_str(), name_string);

	return(test_string);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
inline API_UTILITY const char *ThrowIfNullOrEmpty(const char *test_string,
	const char *name_string = NULL)
{
	return(ThrowIfEmpty(ThrowIfNull(test_string, name_string), name_string));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename CountType>
	inline void CheckCountTypeAndValue(CountType in_count,
		const char *name_string = NULL)
{
	if (!std::numeric_limits<CountType>::is_integer)
		throw std::invalid_argument(
			NullOrEmptyToStringWithColon(name_string, "") + "The count type "
			"specified is not an integral type.");

	if (IsValueNegative(in_count))
		throw std::invalid_argument(
			NullOrEmptyToStringWithColon(name_string, "") + "The count value "
			"specified (" + std::to_string(in_count) + ") is negative.");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename CountType, typename ListType>
	inline void CheckCongruenceCountAndList(CountType in_count,
		ListType in_list, const char *name_string = NULL)
{
	CheckCountTypeAndValue(in_count, name_string);

	if ((in_count && (!in_list)) || ((!in_count) && in_list))
		throw std::invalid_argument(
			NullOrEmptyToStringWithColon(name_string, "") + "The count indicates "
			"the presence of " + std::to_string(in_count) + " elements in the "
			"list, but the list pointer is " +
			std::string((!in_list) ? "" : "not ") + "'NULL'.");
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__ArgChecks_hpp__HH

