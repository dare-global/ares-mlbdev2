// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB MFStore Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  MFStoreSection.hpp

   File Description  :  Include file for the mmap section class.

   Revision History  :  2021-02-14 --- Creation.
                           Michael L. Brock

      Copyright Michael L. Brock 2021 - 2024.
      Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
      http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__MFStore__MFStoreSection_hpp__HH

#define HH__MLB__MFStore__MFStoreSection_hpp__HH 1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
   \file MFStoreSection.hpp

   \brief   Definition of the mmap section class.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <MFStore/MFStore.hpp>

#include <iostream>
#include <string>
#include <vector>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace MFStore {

// ////////////////////////////////////////////////////////////////////////////
class MFStoreSection
{
public:
	using MFStoreSectionList = std::vector<MFStoreSection>;

	static const uint64_t MaxElementValue      = 1000000000ULL;
	static const uint64_t MaxDescriptionLength = 63ULL;

	MFStoreSection();

	MFStoreSection(
		uint64_t section_index,
		uint64_t element_size,
		uint64_t element_count,
		uint64_t section_offset,
		uint64_t length_actual,
		uint64_t length_padded,
		uint64_t section_flags,
		uint64_t reserved,
		const std::string &description);

	uint64_t CalcNextOffset() const;
	uint64_t CalcLengthUsed() const;
	uint64_t CalcLengthGran(uint64_t section_gran) const;

	void CheckElementInfo() const;
	void CheckElementInfo(uint64_t section_idx) const;

	uint64_t section_index_;
	uint64_t element_size_;
	uint64_t element_count_;
	uint64_t section_offset_;
	uint64_t length_actual_;
	uint64_t length_padded_;
	uint64_t section_flags_;
	uint64_t reserved_;
	char     description_[MaxDescriptionLength + 1];

	std::ostream &ToStreamTabular(std::ostream &o_str = std::cout) const;
	std::ostream &ToStream(std::ostream &o_str = std::cout) const;
	std::string   ToString() const;

	static void AppendSection(const MFStoreSection &src,
		MFStoreSectionList &dst, uint64_t section_gran = MFStoreAllocGran);
	static void FixupSectionList(MFStoreSectionList &dst,
		uint64_t section_gran = MFStoreAllocGran);
	static void CheckSectionList(const MFStoreSectionList &dst,
		uint64_t section_gran = MFStoreAllocGran, uint64_t file_size = 0);
	static void CheckSectionList(std::size_t section_list_index,
		const MFStoreSectionList &dst, uint64_t section_gran = MFStoreAllocGran);

	static std::ostream &ToStreamTabular(const MFStoreSectionList &section_list,
		std::ostream &o_str = std::cout);
	static std::ostream &ToStream(const MFStoreSectionList &section_list,
		std::ostream &o_str = std::cout);
	
private:
	void SetDescription(const std::string &description);
	void SetDescription();
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
using MFStoreSectionList = MFStoreSection::MFStoreSectionList;
// ////////////////////////////////////////////////////////////////////////////

} // namespace MFStore

} // namespace MLB

#endif // #ifndef HH__MLB__MFStore__MFStoreSection_hpp__HH

