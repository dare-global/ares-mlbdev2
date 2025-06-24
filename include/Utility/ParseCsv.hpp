// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  ParseCsv.hpp

   File Description  :  Include file for comma-separated-value parsing.

   Revision History  :  2024-12-28 --- Creation.
                           Michael L. Brock

      Copyright Michael L. Brock 2024.
      Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
      http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__Utility__ParseCsv_hpp__HH

#define HH__MLB__Utility__ParseCsv_hpp__HH 1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
   \file ParseCsv.hpp

   \brief   The comma-separated-value parsing header file.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/Utility.hpp>

#include <ostream>

#include <string>
#include <string_view>
#include <vector>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
class ParseCsvControl
{
public:
	ParseCsvControl(const std::string &value_sep_list = ",",
		const std::string &line_sep_list = "\n", bool enable_crlf = true,
		std::size_t max_nested_lines = 0);

	inline bool IsLineSep(char in_char) const
	{
		return(in_char == '\n');
	}

	inline bool IsValueSep(char in_char) const
	{
		return(value_sep_list_.find_first_of(in_char, 0) != std::string::npos);
	}

	inline std::size_t GetNextSepIndex(const std::string_view &src_line,
		std::size_t current_offset = 0) const
	{
		return(src_line.find_first_of(value_sep_list_, current_offset));
	}

/*
	std::size_t GetValueEnd(const std::string_view &src_line,
		std::size_t current_offset, std::size_t &error_offset) const;
*/
	std::size_t GetValueEnd(const std::string_view &src_line,
		std::size_t current_offset, std::size_t &next_offset,
		std::size_t &error_offset) const;

private:
	std::string value_sep_list_;
	std::string line_sep_list_;
	bool        enable_crlf_;
	std::size_t max_nested_lines_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
using ParseCsvColList = std::vector<std::string_view>;
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
struct ParseCsvPosition
{
	ParseCsvPosition(std::size_t row_idx = 0, std::size_t col_idx = 0,
		std::size_t row_off = 0)
		:row_idx_(row_idx)
		,col_idx_(col_idx)
		,row_off_(row_off)
		,is_line_end_(false)
	{
	}

	std::ostream &ToStream(std::ostream &o_str) const
	{
		o_str << "line number " << (row_idx_ + 1) << ", column number " <<
			(col_idx_ + 1);

		return(o_str);
	}

	std::size_t row_idx_;
	std::size_t col_idx_;
	std::size_t row_off_;
	bool        is_line_end_;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class ParseCsvState
{
public:
	ParseCsvState(std::string_view src_line);
	ParseCsvState(const std::string &src_line);

	bool IsEnd() const
	{
		return(at_end_flag_);
	}

	const ParseCsvPosition &GetPosition() const
	{
		return(position_);
	}

/*
	ParseCsvColList &ParseCsvLine(ParseCsvColList &col_list,
		const ParseCsvControl &parse_control = ParseCsvControl());
	ParseCsvColList  ParseCsvLine(
		const ParseCsvControl &parse_control = ParseCsvControl());
*/

	bool ParseCsvLine(const ParseCsvControl &parse_control,
		ParseCsvPosition &original_pos, ParseCsvPosition &current_pos,
		ParseCsvColList &col_list);
	bool ParseCsvLine(const ParseCsvControl &parse_control,
		ParseCsvPosition &current_pos, ParseCsvColList &col_list);

private:
	ParseCsvPosition position_;
	std::string_view src_line_;
	std::size_t      current_offset_;
	bool             at_end_flag_;
};
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__ParseCsv_hpp__HH

