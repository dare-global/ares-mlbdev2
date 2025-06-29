// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  ValueTraits.hpp

   File Description  :  Include file for miscellaneous value manipulation.

   Revision History  :  2009-09-13 --- Creation.
                           Michael L. Brock
                        2024-09-10 --- Move into MlbDev2.

      Copyright Michael L. Brock 2009 - 2024.
      Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
      http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__Utility__ValueTraits_hpp__HH

#define HH__MLB__Utility__ValueTraits_hpp__HH   1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
   \file    ValueTraits.hpp

   \brief   Miscellaneous logic for value manipulation.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/Utility.hpp>

#include <limits>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

namespace {

// ////////////////////////////////////////////////////////////////////////////
template <bool IsSignedFlag>
	struct IsSignedType {
};
// ----------------------------------------------------------------------------
template <> struct IsSignedType<false> {
	template <typename SomeType>
	static bool IsNegative(const SomeType &)
	{
		return(false);
	}

	template <typename SomeType>
		static SomeType GetValueNegated(const SomeType &in_value)
	{
		return(in_value);
	}

	template <typename SomeType>
		static SomeType GetValueAbsolute(const SomeType &in_value)
	{
		return(in_value);
	}
};
// ----------------------------------------------------------------------------
template <> struct IsSignedType<true> {
	template <typename SomeType>
	static bool IsNegative(const SomeType &in_value)
	{
		return(in_value < static_cast<SomeType>(0));
	}

	template <typename SomeType>
		static SomeType GetValueNegated(const SomeType &in_value)
	{
		return(-in_value);
	}

	template <typename SomeType>
		static SomeType GetValueAbsolute(const SomeType &in_value)
	{
		return((IsNegative(in_value)) ? (-in_value) : in_value);
	}
};
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
template <typename PodType>
	bool IsValueNegative(const PodType &in_value)
{
	return(IsSignedType<std::numeric_limits<PodType>::is_signed>::
		IsNegative(in_value));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename PodType>
	PodType GetValueNegated(const PodType &in_value)
{
	return(IsSignedType<std::numeric_limits<PodType>::is_signed>::
		GetValueNegated(in_value));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename PodType>
	PodType GetValueAbsolute(const PodType &in_value)
{
	return(IsSignedType<std::numeric_limits<PodType>::is_signed>::
		GetValueAbsolute(in_value));
}
// ////////////////////////////////////////////////////////////////////////////

/*
#ifdef TEST_MAIN

// ////////////////////////////////////////////////////////////////////////////
#define TEST_IS_VALUE_NEG(in_value)												\
	{																						\
		std::cerr																		\
			<< std::setw(5) << AnyToString(IsValueNegative(in_value))	\
			<< " ---> " << in_value << std::endl;								\
	}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_IsValueNegative()
{
	int          s_int_1 = -1;
	int          s_int_2 =  0;
	int          s_int_3 =  1;
	unsigned int u_int_1 = static_cast<unsigned int>(-1);
	unsigned int u_int_2 = 0;
	unsigned int u_int_3 = 1;
	double       d_1 = -1.0;
	double       d_2 =  0.0;
	double       d_3 =  1.0;

	TEST_IS_VALUE_NEG(s_int_1)
	TEST_IS_VALUE_NEG(s_int_2)
	TEST_IS_VALUE_NEG(s_int_3)
	TEST_IS_VALUE_NEG(u_int_1)
	TEST_IS_VALUE_NEG(u_int_2)
	TEST_IS_VALUE_NEG(u_int_3)
	TEST_IS_VALUE_NEG(d_1)
	TEST_IS_VALUE_NEG(d_2)
	TEST_IS_VALUE_NEG(d_3)
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN
*/

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__ValueTraits_hpp__HH

