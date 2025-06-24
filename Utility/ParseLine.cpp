// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB Utility Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  ParseLine.cpp

   File Description  :  Implementation of line-feed terminated parsing.
 
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

#include <Utility/ParseLine.hpp>

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
ParseLineState::ParseLineState(std::string_view src_data)
	:src_data_(src_data)
	,line_index_(0)
	,line_offset_(0)
	,current_offset_(0)
	,at_end_flag_(false)
{
	ThrowIfNull(src_data_.data(), "Source data pointer");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
ParseLineState::ParseLineState(const std::string &src_data)
	:ParseLineState(std::string_view(src_data.data(), src_data.size()))
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool ParseLineState::IsEnd() const
{
	return(at_end_flag_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::size_t ParseLineState::GetLineIndex() const
{
	return(line_index_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::size_t ParseLineState::GetLineOffset() const
{
	return(line_offset_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::size_t ParseLineState::GetCurrentOffset() const
{
	return(current_offset_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string_view ParseLineState::ParseLineSingle()
{
	std::string_view dst;

	if (IsEnd())
		return(dst);

	if (current_offset_ >= src_data_.size()) {
		at_end_flag_ = true;
		return(dst);
	}

	std::size_t end_idx = src_data_.find('\n', current_offset_);

	if (end_idx != std::string::npos) {
		std::size_t next_idx = end_idx;
		if ((src_data_[end_idx] == '\n') && (end_idx > current_offset_) &&
			 (src_data_[end_idx - 1] == '\r'))
			--end_idx;
		std::string_view(src_data_.data() + current_offset_,
			end_idx - current_offset_).swap(dst);
		current_offset_ = next_idx + 1;
	}
	else {
		std::string_view(src_data_.data() + current_offset_,
			src_data_.size() - current_offset_).swap(dst);
		current_offset_ = src_data_.size();
		at_end_flag_    = true;
	}

	++line_index_;
	line_offset_ = current_offset_;

	return(dst);
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
#include <vector>

namespace {

// ////////////////////////////////////////////////////////////////////////////
const std::vector<std::string> TEST_TestList =
{
	"A,B,C\n"
	"\n"
	",\r\n"
	",,\n"
	",,A\n"
	",,A,B\n"
	",,A,\tB,,,\n"
	",,A,B,,,C"
	,
	"The quick brown fox\njumped over the lazy dog."
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_RunTest()
{
	using namespace MLB::Utility;

	for (const std::string &this_element : TEST_TestList) {
		std::cout << EmitterSep('=');
		std::cout << "INPUT   : [" <<
			XLateEscapeChars(this_element) << ']' << std::endl;
		ParseLineState line_state(this_element);
		while (!line_state.IsEnd()) {
			std::cout << EmitterSep('-');
			std::size_t      line_index = line_state.GetLineIndex();
			std::string_view this_line(line_state.ParseLineSingle());
			std::cout << "   " << std::setw(5) << ++line_index
				<< ": [" << this_line << "]\n";
		}
		std::cout << EmitterSep('=') << std::endl;
	}
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	try {
		TEST_RunTest();
	}
	catch (const std::exception &except) {
		std::cerr << "\n\nERROR: " << except.what() << std::endl;
	}

	return (EXIT_SUCCESS);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

