// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB Utility Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  ParseCsv.cpp

   File Description  :  Implementation of comma-separated-value parsing.
 
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

#include <Utility/ParseCsv.hpp>

#include <Utility/ArgCheck.hpp>

#include <iostream>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
ParseCsvControl::ParseCsvControl(const std::string &value_sep_list,
	const std::string &line_sep_list, bool enable_crlf,
	std::size_t max_nested_lines)
	:value_sep_list_(ThrowIfEmpty(value_sep_list,
		"The CSV parser value separator character list"))
	,line_sep_list_(line_sep_list)
	,enable_crlf_(enable_crlf)
	,max_nested_lines_(max_nested_lines)
{
	if (value_sep_list_.empty())
		throw std::invalid_argument("The specified value separator list "
			"is empty.");

	if (value_sep_list_.find_first_of(line_sep_list_) != std::string::npos)
		throw std::invalid_argument("The specified value separator list "
			"contains at least one line separator character.");

	if (line_sep_list_.empty())
		throw std::invalid_argument("The specified line separator list "
			"is empty.");

	if (line_sep_list_.find_first_of(value_sep_list_) != std::string::npos)
		throw std::invalid_argument("The specified line separator list "
			"contains at least one value separator character.");

	if (enable_crlf && (line_sep_list_.find('\n') == std::string::npos))
		throw std::invalid_argument("The specified line separator list "
			"does not contain a line-feed character, but the enable CRLF flag "
			"is set to 'true'.");

	if (enable_crlf && (line_sep_list_.find('\r') != std::string::npos))
		throw std::invalid_argument("The specified line separator list "
			"includes a carriage-return character, but the enable CRLF flag "
			"is set to 'true'.");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/*
std::size_t ParseCsvControl::GetValueEnd(const std::string_view &src_line,
	std::size_t current_offset, std::size_t &error_offset) const
{
	if (current_offset >= src_line.size()) {
		error_offset = current_offset;
		throw std::invalid_argument("Invocation of ParseCsvState::GetValueEnd()"
			" with a source datum length of " + std::to_string(src_line.size()) +
			" and a current offset of " + std::to_string(current_offset) + ".");
	}

	std::size_t start_offset = current_offset;
	std::size_t last_offset  = src_line.size() - 1;

	while (current_offset <= last_offset) {
		char this_char = src_line[current_offset];
		if ((this_char == '"') || (this_char == '\'')) {
			char        quote_char   = this_char;
			bool        in_quote     = true;
			std::size_t quote_offset = current_offset;
			std::size_t nested_count = 0;
			while (++current_offset <= last_offset) {
				this_char = src_line[current_offset];
				if (this_char == quote_char) {
					// Is this a quoted by doubling ('' or "") quote char?
					if ((current_offset - 1) == quote_offset) {
						in_quote = false;
						break;
					}
					if ((current_offset < last_offset) &&
						 (src_line[current_offset - 1] == quote_char))
						++current_offset;
					else {
						// If not, it's the end of the quote.
						in_quote = false;
						if (current_offset == last_offset)
							return(current_offset);
						break;
					}
				}
				else if (IsLineSep(this_char)) {
					if (nested_count == max_nested_lines_) {
						error_offset = current_offset;
						throw std::invalid_argument("The CSV quoted string value "
							"contains more line separator characters than is "
							"permitted by the current CSV parser configuration (" +
							std::to_string(max_nested_lines_) + ").");
					}
					++nested_count;
				}
			}
			if (in_quote) {
				error_offset = quote_offset;
				throw std::invalid_argument("Unterminated quote character: "
					"opening of quote was at index " +
					std::to_string(quote_offset) + ".");
			}
			++current_offset;
		}
		else if (IsValueSep(this_char)) {
			return(current_offset);
		}
		else if (IsLineSep(this_char))
			return(current_offset);
		else
			++current_offset;
	}

	return(current_offset);
}
*/
std::size_t ParseCsvControl::GetValueEnd(const std::string_view &src_line,
	std::size_t current_offset, std::size_t &next_offset,
	std::size_t &error_offset) const
{
/*
	if (current_offset >= src_line.size()) {
		error_offset = current_offset;
		throw std::invalid_argument("Invocation of ParseCsvState::GetValueEnd()"
			" with a source datum length of " + std::to_string(src_line.size()) +
			" and a current offset of " + std::to_string(current_offset) + ".");
	}
*/
	if (current_offset > src_line.size()) {
		error_offset = current_offset;
		throw std::invalid_argument("Invocation of ParseCsvState::GetValueEnd()"
			" with a source datum length of " + std::to_string(src_line.size()) +
			" and a current offset of " + std::to_string(current_offset) + ".");
	}
	else if (current_offset == src_line.size()) {
/*
		error_offset = current_offset;
		throw std::invalid_argument("Invocation of ParseCsvState::GetValueEnd()"
			" with a source datum length of " + std::to_string(src_line.size()) +
			" and a current offset of " + std::to_string(current_offset) + ".");
*/
		next_offset = current_offset;
		return(current_offset);
	}

	std::size_t start_offset = current_offset;
	std::size_t last_offset  = src_line.size() - 1;

	while (current_offset <= last_offset) {
		char this_char = src_line[current_offset];
		if ((this_char == '"') || (this_char == '\'')) {
			char        quote_char   = this_char;
			bool        in_quote     = true;
			std::size_t quote_offset = current_offset;
			std::size_t nested_count = 0;
			while (++current_offset <= last_offset) {
				this_char = src_line[current_offset];
				if (this_char == quote_char) {
					// Is this a quoted by doubling ('' or "") quote char?
					if ((current_offset - 1) == quote_offset) {
						in_quote = false;
						break;
					}
					if ((current_offset < last_offset) &&
						 (src_line[current_offset - 1] == quote_char))
						++current_offset;
					else {
						// If not, it's the end of the quote.
						in_quote = false;
						if (current_offset == last_offset) {
							next_offset = current_offset;
							return(current_offset);
						}
						break;
					}
				}
				else if (IsLineSep(this_char)) {
					if (nested_count == max_nested_lines_) {
						error_offset = current_offset;
						throw std::invalid_argument("The CSV quoted string value "
							"contains more line separator characters than is "
							"permitted by the current CSV parser configuration (" +
							std::to_string(max_nested_lines_) + ").");
					}
					++nested_count;
				}
			}
			if (in_quote) {
				error_offset = quote_offset;
				throw std::invalid_argument("Unterminated quote character: "
					"opening of quote was at index " +
					std::to_string(quote_offset) + ".");
			}
			++current_offset;
		}
		else if (IsValueSep(this_char)) {
			next_offset = current_offset + 1;
			return(current_offset);
		}
		else if (IsLineSep(this_char)) {
			next_offset = current_offset + 1;
			if ((this_char == '\n') && enable_crlf_ &&
				 (current_offset > start_offset) &&
				 (src_line[current_offset - 1] == '\r'))
				current_offset--;
			return(current_offset);
		}
		else
			++current_offset;
	}

	next_offset = current_offset + ((current_offset < last_offset) ? 1 : 0);

	return(current_offset);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ParseCsvState::ParseCsvState(std::string_view src_line)
	:position_()
	,src_line_(src_line)
	,current_offset_(0)
	,at_end_flag_(false)
{
	ThrowIfNull(src_line_.data(), "Source data pointer");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ParseCsvState::ParseCsvState(const std::string &src_line)
	:ParseCsvState(std::string_view(src_line.data(), src_line.size()))
{
}
// ////////////////////////////////////////////////////////////////////////////

/*
// ////////////////////////////////////////////////////////////////////////////
ParseCsvColList &ParseCsvState::ParseCsvLine(ParseCsvColList &col_list,
	const ParseCsvControl &parse_control)
{
	col_list.clear();

	if (IsEnd())
		return(col_list);

	while (current_offset_ <= src_line_.size()) {
		if (current_offset_ == src_line_.size()) {
			col_list.push_back(std::string_view());
			at_end_flag_ = true;
			break;
		}
		std::size_t error_idx = 0;
		std::size_t start_idx = current_offset_;
		std::size_t end_idx   =
			parse_control.GetValueEnd(src_line_, current_offset_, error_idx);
		col_list.emplace_back(src_line_.data() + current_offset_,
			end_idx - current_offset_);
		current_offset_ = end_idx + 1;
	}

	return(col_list);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ParseCsvColList ParseCsvState::ParseCsvLine(
	const ParseCsvControl &parse_control)
{
	ParseCsvColList col_list;

	return(ParseCsvLine(col_list, parse_control));
}
// ////////////////////////////////////////////////////////////////////////////
*/

// ////////////////////////////////////////////////////////////////////////////
bool ParseCsvState::ParseCsvLine(const ParseCsvControl &parse_control,
	ParseCsvPosition &original_pos, ParseCsvPosition &current_pos,
	ParseCsvColList &col_list)
{
	col_list.clear();

	if (IsEnd())
		return(false);

	ParseCsvPosition tmp_current_pos = original_pos;

	while (tmp_current_pos.row_off_ <= src_line_.size()) {
		if (tmp_current_pos.row_off_ == src_line_.size()) {
			col_list.push_back(std::string_view());
			at_end_flag_ = true;
			break;
		}
		std::size_t error_idx = 0;
		std::size_t start_idx = tmp_current_pos.row_off_;
		std::size_t next_idx  = 0;
		std::size_t end_idx   =
			parse_control.GetValueEnd(src_line_,
				tmp_current_pos.row_off_, next_idx, error_idx);
		col_list.emplace_back(src_line_.data() + start_idx,
			end_idx - start_idx);
		if (end_idx >= src_line_.size()) {
			std::cerr << "***** [" << end_idx << '/' << src_line_.size()
				<< "] ******" << std::endl;
			at_end_flag_ = true;
			break;
		}
		else if (!parse_control.IsLineSep(src_line_[end_idx]))
			tmp_current_pos.row_off_ = end_idx + 1;
		else {
			tmp_current_pos.is_line_end_ = true;
			++original_pos.row_idx_;
			original_pos.col_idx_        = 0;
			original_pos.row_off_        = end_idx + 1;
			original_pos.is_line_end_    = false;
			break;
		}
	}

	current_pos = tmp_current_pos;

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool ParseCsvState::ParseCsvLine(const ParseCsvControl &parse_control,
	ParseCsvPosition &current_pos, ParseCsvColList &col_list)
{
	return(ParseCsvLine(parse_control, position_, current_pos, col_list));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

// ////////////////////////////////////////////////////////////////////////////
// ****************************************************************************
// ****************************************************************************
// ****************************************************************************
// ////////////////////////////////////////////////////////////////////////////

#ifdef TEST_MAIN

#include <Utility/EmitterSep.hpp>
#include <Utility/XLateEscapeChars.hpp>

#include <iostream>
#include <iomanip>

namespace {

// ////////////////////////////////////////////////////////////////////////////
const std::vector<std::string> TEST_TestList =
{
//"A\"\"B",
//"A\"\"\"\"B",
//	"\n",
	"A\nB",
	"A,B,C\nD\n\nE,F,G",
	"A'\n'B",
	"A'\n\n'B",
	"A,,',,\n,,',,B",
//"A,,\",,\n,,\",,B",
	"A,'B','C,,'',,D',E",
//"A,\"B\",\"C,,\"\",,D\",E",
	"A,B,C,,",
	"",
	",",
	",,",
	"A,B,C",
	"",
	",",
	",,",
	",,A",
	",,A,B",
	",,A,B,,,",
	",,A,B,,,C"
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/*
void TEST_RunTest()
{
	using namespace MLB::Utility;

	ParseCsvControl parse_control(",", "\n", true, 2);
	ParseCsvColList col_list;

	for (const std::string &this_element : TEST_TestList) {
		std::cout <<
			EmitterSep((&this_element == &TEST_TestList.front()) ? '=' : '-');
		std::cout << "INPUT   : [" << XLateEscapeChars(this_element)
			<< "]" << std::endl;
		ParseCsvState csv_state(this_element);
		std::size_t   col_index = 0;
		csv_state.ParseCsvLine(col_list, parse_control);
		for (const auto &this_col : col_list)
			std::cout << "   " << std::setw(5) << col_index++
				<< ": [" << XLateEscapeChars(this_col) << "]\n";
	}

	std::cout << EmitterSep('=') << std::endl;
}
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_RunTestStep()
{
	using namespace MLB::Utility;

	ParseCsvControl          parse_control(",", "\n", true, 2);
	std::vector<std::string> src_list = {
//		",",
//		"\n",
//		"\r\n",
//		"\r\n\n",
//		"\r\n\r\n",
		",A,B",
		"A,B",
		"A\nB",
		"A\r\nB",
		"AA,BB",
		"",
		",",
		",,",
		",,,,,",
		",,,,,\n",
		",,,,,\r\n",
	};

	for (const auto &this_element : src_list)
	{
		std::size_t start_offset = 0;
		std::size_t end_offset   = 0;
		std::size_t next_offset  = 0;
		std::size_t error_offset = 0;
		std::size_t test_index   = 0;
		std::cout << EmitterSep('=');
		std::cout << "INPUT   : [" << XLateEscapeChars(this_element)
			<< "]" << std::endl;
		while (start_offset <= this_element.size()) {
/*
			std::cout
				<< std::setw(3) << test_index << ": "
				<< "START=" << std::setw(10) << start_offset << " / "
				<< "END  =" << std::setw(10) << end_offset   << " / "
				<< "NEXT =" << std::setw(10) << next_offset  << '\n';
*/
			end_offset = parse_control.GetValueEnd(this_element,
				start_offset, next_offset, error_offset);
/*
//			std::cout << EmitterSep('-') << std::endl;
			std::cout
				<< std::setw(3) << test_index << ": "
				<< "START=" << std::setw(10) << start_offset << " / "
				<< "END  =" << std::setw(10) << end_offset   << " / "
				<< "NEXT =" << std::setw(10) << next_offset  << '\n';
			std::cout << '[' << XLateEscapeChars(std::string(
				this_element.c_str() + start_offset,
				this_element.c_str() + end_offset)) << "]\n";
*/
			std::cout
				<< "   START:" << std::setw(5) << start_offset
				<< "  END:"    << std::setw(5) << end_offset
				<< " NEXT:"    << std::setw(5) << next_offset
				<< " ["        << XLateEscapeChars(std::string(
				this_element.c_str() + start_offset,
				this_element.c_str() + end_offset)) << "]\n";
			if (next_offset == end_offset)
				break;
			++test_index;
			start_offset = next_offset;
			end_offset   = 0;
			next_offset  = 0;
		}
	}

	std::cout << EmitterSep('=') << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_RunTest()
{
	using namespace MLB::Utility;

	ParseCsvControl parse_control(",", "\n", true, 2);
	ParseCsvColList col_list;

/*
	{
		std::size_t start_offset = 0;
		std::size_t end_offset   = 0;
		std::size_t next_offset  = 0;
		std::size_t error_offset = 0;
		std::size_t test_index   = 0;
//		std::string src_data(",A,B");
//		std::string src_data("A,B");
//		std::string src_data("A\nB");
//		std::string src_data("A\r\nB");
		std::string src_data("AA,BB");
std::cout
	<< std::setw(3) << test_index << ": "
	<< "START=" << std::setw(10) << start_offset << " / "
	<< "END  =" << std::setw(10) << end_offset   << " / "
	<< "NEXT =" << std::setw(10) << next_offset  << '\n';
		end_offset = parse_control.GetValueEnd(src_data,
			start_offset, next_offset, error_offset);
std::cout
	<< std::setw(3) << test_index << ": "
	<< "START=" << std::setw(10) << start_offset << " / "
	<< "END  =" << std::setw(10) << end_offset   << " / "
	<< "NEXT =" << std::setw(10) << next_offset  << '\n';
++test_index;
		start_offset = next_offset;
		end_offset   = 0;
		next_offset  = 0;
std::cout
	<< std::setw(3) << test_index << ": "
	<< "START=" << std::setw(10) << start_offset << " / "
	<< "END  =" << std::setw(10) << end_offset   << " / "
	<< "NEXT =" << std::setw(10) << next_offset  << '\n';
		end_offset = parse_control.GetValueEnd(src_data,
			start_offset, next_offset, error_offset);
std::cout
	<< std::setw(3) << test_index << ": "
	<< "START=" << std::setw(10) << start_offset << " / "
	<< "END  =" << std::setw(10) << end_offset   << " / "
	<< "NEXT =" << std::setw(10) << next_offset  << '\n';
++test_index;
	}
*/

	for (const std::string &this_element : TEST_TestList) {
		std::cout <<
			EmitterSep((&this_element == &TEST_TestList.front()) ? '=' : '-');
		std::cout << "INPUT   : [" << XLateEscapeChars(this_element)
			<< "]" << std::endl;
		ParseCsvState csv_state(this_element);
//		const ParseCsvPosition &pos_ref(csv_state.GetPosition());
//		ParseCsvPosition start_pos;
		ParseCsvPosition end_pos;
//		csv_state.ParseCsvLine(col_list, parse_control);
		for ( ; ; ) {
			ParseCsvPosition start_pos(csv_state.GetPosition());
			if (!csv_state.ParseCsvLine(parse_control, end_pos, col_list))
				break;
			std::size_t col_index = 0;
			for (const auto &this_col : col_list)
				std::cout << "   "
					<< std::setfill('0')
					<< "RIDX=" << std::setw(5) << start_pos.row_idx_ << ":"
					<< "CIDX=" << std::setw(5) << start_pos.col_idx_ << ":"
					<< "ROFF=" << std::setw(5) << start_pos.row_off_ << ":"
					<< "CNUM=" << std::setw(5) << col_index++
					<< std::setfill(' ')
					<< ": [" << XLateEscapeChars(this_col) << "]\n";
		}
	}

	std::cout << EmitterSep('=') << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
//		TEST_RunTest();
TEST_RunTestStep();
	}
	catch (const std::exception &except) {
		return_code = EXIT_FAILURE;
		std::cerr << "\n\nERROR: " << except.what() << std::endl;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

