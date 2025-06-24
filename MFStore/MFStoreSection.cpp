// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB MFStore Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  MFStoreSection.cpp

   File Description  :  Implementation of the mmap section class.

   Revision History  :  2021-02-14 --- Creation.
                           Michael L. Brock

      Copyright Michael L. Brock 2021 - 2024.
      Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
      http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <MFStore/MFStoreSection.hpp>

#include <MFStore/CheckValues.hpp>

#include <Utility/EmitterSep.hpp>
#include <Utility/GranularRound.hpp>
#include <Utility/ThrowErrno.hpp>

#include <cstring>
#include <sstream>
#include <stdexcept>

#include <boost/io/ios_state.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace MFStore {

// ////////////////////////////////////////////////////////////////////////////
MFStoreSection::MFStoreSection()
	:section_index_(0)
	,element_size_(0)
	,element_count_(0)
	,section_offset_(0)
	,length_actual_(0)
	,length_padded_(0)
	,section_flags_(0)
	,reserved_(0)
{
	SetDescription();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
MFStoreSection::MFStoreSection(
	uint64_t section_index,
	uint64_t element_size,
	uint64_t element_count,
	uint64_t section_offset,
	uint64_t length_actual,
	uint64_t length_padded,
	uint64_t section_flags,
	uint64_t reserved,
	const std::string &description)
	:section_index_(section_index)
	,element_size_(element_size)
	,element_count_(element_count)
	,section_offset_(section_offset)
	,length_actual_(length_actual)
	,length_padded_(length_padded)
	,section_flags_(section_flags)
	,reserved_(reserved)
{
	SetDescription(description);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
uint64_t MFStoreSection::CalcNextOffset() const
{
	return(section_offset_ + length_padded_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
uint64_t MFStoreSection::CalcLengthUsed() const
{
	return(element_size_ * element_count_);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
uint64_t MFStoreSection::CalcLengthGran(uint64_t section_gran) const
{
	return(MLB::Utility::GranularRoundUp(length_actual_,
		FixUpStorageGran(section_gran)));
}
// ////////////////////////////////////////////////////////////////////////////

namespace {

// ////////////////////////////////////////////////////////////////////////////
void CheckAnElementInfo(uint64_t element_value, const char *element_name)
{
	try {
		if (!element_value)
			throw std::invalid_argument("Value is zero.");
		if (element_value > MFStoreSection::MaxElementValue)
			throw std::invalid_argument("Value exceeds the maximum "
				"permissible (" + std::to_string(MFStoreSection::MaxElementValue) +
				").");
	}
	catch (const std::exception &except) {
		throw std::invalid_argument("Invalid section element " +
			std::string(element_name) + ": " + std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespac

// ////////////////////////////////////////////////////////////////////////////
void MFStoreSection::CheckElementInfo() const
{
	CheckAnElementInfo(element_size_, "size");
	CheckAnElementInfo(element_count_, "count");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void MFStoreSection::CheckElementInfo(uint64_t section_idx) const
{
	try {
		CheckElementInfo();
	}
	catch (const std::exception &except) {
		throw std::invalid_argument("Section at index " +
			std::to_string(section_idx) + " is invalid: " +
			std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream &MFStoreSection::ToStream(std::ostream &o_str) const
{
	boost::io::ios_all_saver io_state(o_str);

	o_str
		<< std::right << std::dec
		<< "Section Index : " << std::setw(20) << section_index_  << '\n'
		<< "Element Size  : " << std::setw(20) << element_size_   << '\n'
		<< "Element Count : " << std::setw(20) << element_count_  << '\n'
		<< "Section Offset: " << std::setw(20) << section_offset_ << '\n'
		<< "Length Used   : " << std::setw(20) << length_actual_  << '\n'
		<< "Length Padded : " << std::setw(20) << length_padded_  << '\n'
		<< "Section Flags : " << std::setw(20) << section_flags_  << '\n'
		<< "Reserved      : " << std::setw(20) << reserved_       << '\n'
		<< "Description   : " << std::left     << description_    << '\n'
			;

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string MFStoreSection::ToString() const
{
	std::ostringstream o_str;

	ToStream(o_str);

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void MFStoreSection::AppendSection(const MFStoreSection &src,
	MFStoreSectionList &dst, uint64_t section_gran)
{
	std::size_t section_idx = dst.size();

	try {
		src.CheckElementInfo(section_idx);
		dst.push_back(src);
		dst.back().section_index_  = section_idx;
		dst.back().section_offset_ =
			(!section_idx) ? 0 : dst[section_idx - 1].CalcNextOffset();
		dst.back().length_actual_  = src.CalcLengthUsed();
		dst.back().length_padded_  = src.CalcLengthGran(section_gran);
	}
	catch (const std::exception &except) {
		throw std::invalid_argument("Unable to append new section at index " +
			std::to_string(section_idx) + ": " + std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void MFStoreSection::FixupSectionList(MFStoreSectionList &dst,
	uint64_t section_gran)
{
	if (dst.empty())
		throw std::invalid_argument("Section list is empty.");

	section_gran = FixUpStorageGran(section_gran);

	for (std::size_t section_idx = 0; section_idx < dst.size(); ++section_idx) {
		MFStoreSection &section = dst[section_idx];
		section.CheckElementInfo(section_idx);
		section.section_index_  = section_idx;
		section.section_offset_ =
			(!section_idx) ? 0 : dst[section_idx - 1].CalcNextOffset();
		section.length_actual_  = section.CalcLengthUsed();
		section.length_padded_  = section.CalcLengthGran(section_gran);
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void MFStoreSection::CheckSectionList(const MFStoreSectionList &dst,
	uint64_t section_gran, uint64_t file_size)
{
	if (dst.empty())
		throw std::invalid_argument("Section list is empty.");

	section_gran = FixUpStorageGran(section_gran);

	for (std::size_t section_idx = 0; section_idx < dst.size(); ++section_idx) {
		try {
			const MFStoreSection &section = dst[section_idx];
			section.CheckElementInfo();
			if (section.section_index_ != section_idx)
				throw std::invalid_argument("The section index (" +
					std::to_string(section.section_index_) + ") is not equal to "
					"its calculated value (" + std::to_string(section_idx) + ").");
			uint64_t section_offset = 
				(!section_idx) ? 0 : dst[section_idx - 1].CalcNextOffset();
			if (section.section_offset_ != section_offset)
				throw std::invalid_argument("The section offset (" +
					std::to_string(section.section_offset_) + ") is not equal to "
					"its calculated value as " +
					std::string((!section_idx) ? "the first" :
					"immediately following the previous") + " section (" +
					std::to_string(section_offset) + ").");
			if (section.length_actual_ != section.CalcLengthUsed())
				throw std::invalid_argument("The section actual byte length (" +
					std::to_string(section.length_actual_) + ") is not equal to "
					"its calculated value (" +
					std::to_string(section.CalcLengthUsed()) + ").");
			if (section.length_padded_ != section.CalcLengthGran(section_gran))
				throw std::invalid_argument("The section padded byte length (" +
					std::to_string(section.length_padded_) + ") is not equal to "
					"its calculated value (" +
					std::to_string(section.CalcLengthGran(section_gran)) + ").");
			if (!section.description_[0])
				throw std::invalid_argument("The section description string is "
					"empty.");
			if (file_size)
				CheckExtent(file_size, section.section_offset_,
					section.length_padded_, true);
		}
		catch (const std::exception &except) {
			throw std::invalid_argument("Section at index " +
				std::to_string(section_idx) + " is invalid: " +
				std::string(except.what()));
		}
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void MFStoreSection::CheckSectionList(std::size_t section_list_index,
	const MFStoreSectionList &dst, uint64_t section_gran)
{
	CheckSectionList(dst, section_gran);

	try {
		if (section_list_index >= dst.size())
			throw std::invalid_argument("The specified section index "
				"is not less than the number of sections in the list to be "
				"checked (" + std::to_string(dst.size()) + ").");
		if (dst[section_list_index].element_size_ != sizeof(MFStoreSection))
			throw std::invalid_argument("The size of the elements in that "
				"section (" +
				std::to_string(dst[section_list_index].element_size_) +
				") is not equal to the size of the 'MFStoreSection' class (" +
				std::to_string(sizeof(MFStoreSection)) + ").");
		if (dst[section_list_index].element_count_ != dst.size())
			throw std::invalid_argument("The number of elements in the specified "
				"section element (" +
				std::to_string(dst[section_list_index].element_count_) + ") is "
				"not equal to the number of elements in the section list (" +
				std::to_string(dst.size()) + ").");
	}
	catch (const std::exception &except) {
		throw std::invalid_argument("Unable to check the section at index " +
			std::to_string(section_list_index) + ", which is where the list of "
			"MFStoreSection elements is to be located: " +
			std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

namespace {

// ////////////////////////////////////////////////////////////////////////////
const std::pair<const char *, const char *> TabularColList[] =
{
	 { "Section",     "Index" }
	,{ "Element",     "Size" }
	,{ "Element",     "Count" }
	,{ "Section",     "Offset" }
	,{ "Section",     "Length Actual" }
	,{ "Section",     "Length Padded" }
	,{ "Section",     "Flags" }
	,{ "Section",     "Reserved" }
	,{ "Section",     "Description" }
};

const std::size_t                           TabularColCount  =
	sizeof(TabularColList) / sizeof(TabularColList[0]);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::streamsize GetTabularColWidth(std::size_t col_idx)
{
	return(static_cast<std::streamsize>(
		(col_idx < (TabularColCount - 1)) ? 20 : 63));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void EmitTabularText(bool is_first, std::ostream &o_str)
{
	boost::io::ios_all_saver io_state(o_str);

	o_str << std::left;

	for (std::size_t col_idx = 0; col_idx < TabularColCount; ++col_idx)
			o_str
				<< ((col_idx) ? " " : "")
				<< std::setw(GetTabularColWidth(col_idx))
				<< ((is_first) ? TabularColList[col_idx].first :
					 TabularColList[col_idx].second);

	o_str << '\n';
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void EmitTabularSep(char sep_char, std::ostream &o_str)
{
	boost::io::ios_all_saver io_state(o_str);

	o_str << std::setfill(sep_char);

	for (std::size_t col_idx = 0; col_idx < TabularColCount; ++col_idx)
			o_str
				<< ((col_idx) ? " " : "")
				<< std::setw(GetTabularColWidth(col_idx))
				<< "";

	o_str << '\n';
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void EmitTabularHeader(std::ostream &o_str)
{
	EmitTabularSep('=',    o_str);
	EmitTabularText( true, o_str);
	EmitTabularText(false, o_str);
	EmitTabularSep('-',    o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void EmitTabularTrailer(std::ostream &o_str)
{
	EmitTabularSep('=', o_str);
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
std::ostream &MFStoreSection::ToStreamTabular(
	const MFStoreSectionList &section_list, std::ostream &o_str)
{
	boost::io::ios_all_saver io_state(o_str);

	EmitTabularHeader(o_str);

	o_str << std::setfill(' ') << std::dec;

	uint64_t total_padded_length = 0;

	for (const auto &this_section : section_list) {
		o_str
			<< std::right
			<< std::setw(20) << this_section.section_index_  << ' '
			<< std::setw(20) << this_section.element_size_   << ' '
			<< std::setw(20) << this_section.element_count_  << ' '
			<< std::setw(20) << this_section.section_offset_ << ' '
			<< std::setw(20) << this_section.length_actual_  << ' '
			<< std::setw(20) << this_section.length_padded_  << ' '
			<< std::setw(20) << this_section.section_flags_  << ' '
			<< std::setw(20) << this_section.reserved_       << ' '
			<< std::left     << this_section.description_    << '\n';
		total_padded_length += this_section.length_padded_;
	}

	EmitTabularTrailer(o_str);

	o_str
		<< std::right
		<< std::setw(20 + 1 + 20 + 1 + 20 + 1 + 20 + 1) << ""
		<< std::setw(20) << "Total:"                             << ' '
		<< std::setw(20) << total_padded_length                  << '\n'
		<< std::right
		<< std::setw(20 + 1 + 20 + 1 + 20 + 1 + 20 + 1 + 20 + 1) << ""
		<< std::setfill('=') << std::setw(20) << "" << std::setfill(' ') << '\n';

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream &MFStoreSection::ToStream(const MFStoreSectionList &section_list,
	std::ostream &o_str)
{
	o_str << MLB::Utility::EmitterSep('=', 16 + MaxDescriptionLength);

	for (const auto &this_section : section_list) {
		if (&this_section != &section_list.front())
			o_str << MLB::Utility::EmitterSep('-', 16 + MaxDescriptionLength);
		this_section.ToStream(o_str);
	}

	o_str << MLB::Utility::EmitterSep('=', 16 + MaxDescriptionLength);

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void MFStoreSection::SetDescription(const std::string &description)
{
	if (description.size() >= (sizeof(description_) - 1))
		throw std::invalid_argument("The length of the description  "
			"parameter (" + std::to_string(description.size()) + ") exceeds the "
			"maximum permissible (" + std::to_string(sizeof(description_) - 1) +
			").");

	SetDescription();

	::strcpy(description_, description.c_str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void MFStoreSection::SetDescription()
{
	::memset(description_, '\0', sizeof(description_));
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace MFStore

} // namespace MLB

// ////////////////////////////////////////////////////////////////////////////
// ****************************************************************************
// ****************************************************************************
// ****************************************************************************
// ////////////////////////////////////////////////////////////////////////////

#ifdef TEST_MAIN

#include <Utility/Sleep.hpp>

#include <iostream>

using namespace MLB::Utility;
using namespace MLB::MFStore;

namespace {

// ////////////////////////////////////////////////////////////////////////////
const MFStoreSection TEST_SectionList[] =
{
	 MFStoreSection( 0,                   1234,      1, 0, 0, 0, 0, 0, "Header")
	,MFStoreSection( 0, sizeof(MFStoreSection),      8, 0, 0, 0, 0, 0, "Section List")
	,MFStoreSection( 0,                      4,      6, 0, 0, 0, 0, 0, "Type Info")
	,MFStoreSection( 0,                    150, 112233, 0, 0, 0, 0, 0, "Info List All")
	,MFStoreSection( 0,                    987, 112233, 0, 0, 0, 0, 0, "Info Serial")
	,MFStoreSection( 0,                    150,   1024, 0, 0, 0, 0, 0, "Info List Sub")
	,MFStoreSection( 0,                     64,   1024, 0, 0, 0, 0, 0, "ToB")
	,MFStoreSection( 0,                    200,   1024, 0, 0, 0, 0, 0, "MStats")
};

const std::size_t TEST_SectionCount  =
	sizeof(TEST_SectionList) / sizeof(TEST_SectionList[0]);
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
MLB::MFStore::MFStoreSectionList TEST_CreateSections()
{
	using namespace MLB::MFStore;

	MFStoreSectionList section_list;

	for (std::size_t section_idx = 0; section_idx < TEST_SectionCount;
		++section_idx)
		MFStoreSection::AppendSection(TEST_SectionList[section_idx],
			section_list);

	MFStoreSection::FixupSectionList(section_list);

	MFStoreSection::CheckSectionList(1, section_list);

	return(section_list);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_MFStoreSection()
{
/*
	using namespace MLB::MFStore;

	MFStoreSectionList section_list;

	for (std::size_t section_idx = 0; section_idx < TEST_SectionCount;
		++section_idx)
		MFStoreSection::AppendSection(TEST_SectionList[section_idx], section_list);

	MFStoreSection::FixupSectionList(section_list);

	MFStoreSection::CheckSectionList(1, section_list);

	MFStoreSection::ToStreamTabular(section_list);
	std::cout << '\n';
*/
	MLB::MFStore::MFStoreSectionList section_list = TEST_CreateSections();

	MLB::MFStore::MFStoreSection::ToStreamTabular(section_list) << '\n';
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		TEST_MFStoreSection();
	}
	catch (const std::exception &except) {
		return_code = EXIT_FAILURE;
		std::cerr << "\n\nERROR: " << except.what() << std::endl;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

