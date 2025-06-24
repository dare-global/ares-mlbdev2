// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB Utility Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  XLateEscapeChars.cpp

   File Description  :  Implementation of escape character translation.
 
   Revision History  :  2024-12-28 --- Creation.
                           Michael L. Brock

      Copyright Michael L. Brock 2024.
      Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
      http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Include necessary header files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/XLateEscapeChars.hpp>

#include <Utility/ArgCheck.hpp>

#include <charconv>
#include <climits>
#include <cstring>
#include <ios>
#include <stdexcept>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
const std::string &GetEscapeListRaw()
{
	static const std::string EscapeList_Raw("\0\a\b\n\f\r\t\v", 8);

	return(EscapeList_Raw);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
const std::string &GetEscapeListMap()
{
	static const std::string EscapeList_Map("0abnfrtv");

	return(EscapeList_Map);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string XLateEscapeChars(const std::string_view &src,
	const std::string &chars_raw, const std::string &chars_map)
{
	std::string dst;
	char        hex_buffer[(CHAR_BIT * sizeof(unsigned char)) + 1];

	if (!src.empty()) {
		for (const char &this_char : src) {
			if (::isprint(this_char))
				dst.push_back(this_char);
			else {
				std::size_t raw_idx = std::string::npos;
				if ((!chars_raw.empty()) && (!chars_map.empty())) {
					raw_idx = chars_raw.find(this_char);
					if (raw_idx != std::string::npos) {
						if (raw_idx < chars_map.size()) {
							dst += '\\';
							dst += chars_map[raw_idx];
						}
						else
							raw_idx = std::string::npos;
					}
				}
				if (raw_idx == std::string::npos) {
					unsigned int         ascii_val = static_cast<unsigned int>(
						static_cast<unsigned char>(this_char));
					std::to_chars_result result    = std::to_chars(hex_buffer,
						hex_buffer + sizeof(hex_buffer) - 1, ascii_val, 16);
					if (result.ec != std::errc())
						throw std::invalid_argument(
							std::make_error_code(result.ec).message());
					*result.ptr  = '\0';
					dst         += "\\0x";
					dst         += hex_buffer;
				}
			}
		}
	}

	return(dst);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string XLateEscapeChars(const std::string &src,
	const std::string &chars_raw, const std::string &chars_map)
{
	return(XLateEscapeChars(std::string_view(src), chars_raw, chars_map));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string XLateEscapeChars(const char *src, std::size_t src_length,
	const std::string &chars_raw, const std::string &chars_map)
{
	return(XLateEscapeChars(std::string_view(src, src_length),
		chars_raw, chars_map));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string XLateEscapeChars(const char *src,
	const std::string &chars_raw, const std::string &chars_map)
{
	return(XLateEscapeChars(ThrowIfEmpty(src, "String source pointer"),
		(src) ? ::strlen(src) : 0, chars_raw, chars_map));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

