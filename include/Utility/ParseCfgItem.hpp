// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  ParseCfgItem.hpp

   File Description  :  Include file for configuration item parsing.

   Revision History  :  2023-03-03 --- Migration to C++ MlbDev2/Utility.
                           Michael L. Brock

      Copyright Michael L. Brock 2023.
      Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
      http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__Utility__ParseCfgItem_hpp__HH

#define HH__MLB__Utility__ParseCfgItem_hpp__HH 1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
   \file ParseCfgItem.hpp

   \brief   The configuration item parsing header file.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/ParseNumericString.hpp>
#include <Utility/StringCompareIgnoreCase.hpp>

#include <cstdint>
#include <functional>
#include <map>
#include <string>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
template <typename ConfigHolder> using ParseCfgItemFunc =
	std::function<void(ConfigHolder &cfg_holder, const std::string &src)>;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// IMPL NOTE: Use this for case-sensitive config name comparisons.
template <typename ConfigHolder> using ParseCfgItemMap  =
	std::map<std::string, ParseCfgItemFunc<ConfigHolder> >;
// ----------------------------------------------------------------------------
// IMPL NOTE: Use this for case-insensitive config name comparisons.
template <typename ConfigHolder> using ParseCfgItemMapI =
	std::map<std::string, ParseCfgItemFunc<ConfigHolder>,
		StringIsLessThanIgnoreCase>;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename DatumType>
	void ParseCfgItemDatum(const std::string &src, DatumType &dst)
{
	// IMPL NOTE: Unlike atoi() and atof(), does nothing with empty strings.
	if (!src.empty())
		ParseNumericString(src, dst, true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void ParseCfgItemDatum(const std::string &src, bool &dst);
void ParseCfgItemDatum(const std::string &src, std::string &dst);
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

// ////////////////////////////////////////////////////////////////////////////
/* 
   Ensure these macros are expanded in a context for which the overloaded
   functions 'ParseCfgItemDatum() are visible. For example, for the functions
   above, a 'using namespace MLB::Utility;' statement will suffice.
*/
#define DEF_CfgItemMemDatum(class_name, member_name)    								\
	& class_name :: member_name
// ----------------------------------------------------------------------------
#define DEF_CfgItemNamed(cfg_name, class_name, member_name)  								\
	{   																								\
		cfg_name,   																				\
		[](class_name &class_instance, const std::string &src_datum) 				\
		{																								\
			ParseCfgItemDatum(src_datum,  													\
				class_instance .*DEF_CfgItemMemDatum(class_name, member_name) );  \
		}																								\
	}
// ----------------------------------------------------------------------------
#define DEF_CfgItem(class_name, member_name)  											\
	DEF_CfgItemNamed(#member_name, class_name, member_name)
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifndef HH__MLB__Utility__GetCmdLineArg_hpp__HH

