// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB MFStore Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  CreateMFStore.cpp

   File Description  :  Implementation of the CreateMFStore() function.

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

#include <MFStore/CreateMFStore.hpp>

#include <MFStore/CheckValues.hpp>
#include <MFStore/EnsureFileBackingStore.hpp>

#ifndef _MSC_VER
# include <fcntl.h>
#else
#endif // #ifndef _MSC_VER

#include <Utility/ThrowErrno.hpp>
#include <Utility/ThrowSystemError.hpp>

#include <sstream>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace MFStore {

namespace {

#ifndef _MSC_VER

// ////////////////////////////////////////////////////////////////////////////
MFStoreControl CreateMFStoreForOS(const std::string &file_name,
	MFStoreLen file_size, MFStoreLen mmap_size, MFStoreLen storage_gran)
{
	int file_handle = ::open(file_name.c_str(), O_CREAT|O_EXCL|O_RDWR,
		S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

	if (file_handle < 0) {
		std::string error_text = "Call to ::open() returned " +
			std::to_string(file_handle);
		MLB::Utility::ThrowErrno(error_text);
	}

	EnsureFileBackingStore(file_name, file_handle, 0, file_size);

	return(MFStoreControl(file_name, true, file_size, mmap_size, storage_gran));
}
// ////////////////////////////////////////////////////////////////////////////

#else

// ////////////////////////////////////////////////////////////////////////////
MFStoreControl CreateMFStoreForOS(const std::string &file_name,
	MFStoreLen file_size, MFStoreLen mmap_size, MFStoreLen storage_gran)
{
//	throw std::logic_error("Operation not supported on this operating system.");

	HANDLE file_handle;

	file_handle = ::CreateFileA(
		file_name.c_str(),
/*
		static_cast<DWORD>((is_read_only_) ? FILE_READ_DATA :
			(FILE_READ_DATA | FILE_WRITE_DATA)),
*/
		(FILE_READ_DATA | FILE_WRITE_DATA),
		FILE_SHARE_READ,
		NULL,
		CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (file_handle == INVALID_HANDLE_VALUE) {
		std::ostringstream o_str;
		o_str << "Attempt to open file '" + file_name + "' for memory-mapping "
			"use with 'CreateFile()' failed";
		MLB::Utility::ThrowSystemError(o_str.str());
	}

	EnsureFileBackingStore(file_name, file_handle, 0, file_size);

	return(MFStoreControl(file_name, true, file_size, mmap_size, storage_gran));
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifndef _MSC_VER

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
MFStoreControl CreateMFStore(const std::string &file_name,
	MFStoreLen file_size, MFStoreLen mmap_size, MFStoreLen storage_gran)
{
	MFStoreControl mfstore_ctl;

	try {
		CheckInitialFileAndMmapSizes(file_size, mmap_size, storage_gran);
		mfstore_ctl = CreateMFStoreForOS(file_name, file_size, mmap_size,
			storage_gran);
	}
	catch (const std::exception &except) {
		throw std::runtime_error("Unable to create file '" + file_name +
			"' with a size of " + std::to_string(file_size) + " bytes and a "
			"mmap size of " + std::to_string(mmap_size) + " bytes: " +
			std::string(except.what()));
	}

	return(mfstore_ctl);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
MFStoreControl CreateMFStoreAdjusted(const std::string &file_name,
	MFStoreLen &file_size, MFStoreLen &mmap_size, MFStoreLen &storage_gran)
{
	MFStoreControl mfstore_ctl;
	MFStoreLen     new_storage_gran = FixUpStorageGran(storage_gran);
	MFStoreLen     new_file_size    = FixUpValueGran(file_size, storage_gran);
	MFStoreLen     new_mmap_size    = FixUpValueGran(mmap_size, storage_gran);

	try {
		mfstore_ctl  = CreateMFStore(file_name, new_file_size,
			new_mmap_size, new_storage_gran);
		file_size    = new_file_size;
		mmap_size    = new_mmap_size;
		storage_gran = new_storage_gran;
	}
	catch (const std::exception &except) {
		if ((new_storage_gran == storage_gran) &&
			 (new_file_size    == file_size)    &&
			 (new_mmap_size    == mmap_size))
			throw;
		std::ostringstream o_str;
		o_str << "Unable to create file '" + file_name +
			"' with an original/adjusted file size of " << file_size << '/' <<
			new_file_size << " bytes and an original/adjusted mmap size of " <<
			mmap_size << '/' << new_mmap_size << " bytes using an original/"
			"adjusted storage granularity of " << storage_gran << '/' <<
			new_storage_gran << " bytes: " << except.what();
		throw std::runtime_error(o_str.str());
	}

	return(mfstore_ctl);
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

#include <MFStore/MFStoreSection.hpp>

#include <Utility/HostName.hpp>
#include <Utility/ParseNumericString.hpp>
#include <Utility/ProcessId.hpp>
#include <Utility/UserName.hpp>

#include <Utility/TimeSpec.hpp>

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
MFStoreSectionList TEST_CreateSections()
{
	MFStoreSectionList section_list;

	for (std::size_t section_idx = 0; section_idx < TEST_SectionCount;
		++section_idx)
		MFStoreSection::AppendSection(TEST_SectionList[section_idx],
			section_list);

	MFStoreSection::FixupSectionList(section_list);

	MFStoreSection::CheckSectionList(1, section_list);

//	MFStoreSection::ToStreamTabular(section_list) << '\n';

	return(section_list);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_CreateMFStore(bool reuse_file_name = false, uint64_t mmap_size = 0)
{
reuse_file_name = true;
	MFStoreSectionList section_list = TEST_CreateSections();

	MFStoreSection::ToStreamTabular(section_list) << '\n';

	std::string file_name(
		[](bool reuse_file_name_flag)
		{
			if (reuse_file_name_flag)
				return(std::string("./TEST_MAIN.CreateMFStore."
					"YYYYMMDD_hhmmss_nnnnnnnnn.hostname.username.pid.txt"));

// YYYY-MM-DD hh:mm:ss.123456789
// 0123456789012345678901234567890
//           1         2         3
			std::string dt(TimeSpec::Now().ToString());
			std::string tmp_name("./TEST_MAIN.CreateMFStore." +
				dt.substr( 0, 4) + dt.substr( 5, 2) + dt.substr( 8, 2) + "_" +
				dt.substr(11, 2) + dt.substr(14, 2) + dt.substr(17, 2) + "_" +
				dt.substr(20, 9) + "." +
				GetHostName() + "." +
				GetCurrentUserName() + "." +
				std::to_string(CurrentProcessId()) + ".bin");
			return(tmp_name);
		}(reuse_file_name)
	);

	MFStoreLen file_size = section_list.back().section_offset_ +
		section_list.back().length_padded_;

	mmap_size = (!mmap_size) ? file_size : mmap_size;

	std::cout << "Creating file '" << file_name << "' with file size " <<
		file_size << " and mmap size " << mmap_size << " ..." << std::flush;

	MFStoreControl mfstore_ctl(CreateMFStore(file_name, file_size, mmap_size));

	mfstore_ctl.SetSectionList(section_list);
	mfstore_ctl.CheckSectionList();

	std::cout << " done.\n" << std::endl;

	std::cout << "GetFileName   : " << mfstore_ctl.GetFileName()    << '\n';
	std::cout << "GetFileHandle : " << mfstore_ctl.GetFileHandle()  << '\n';
	std::cout << "GetFileSize   : " << mfstore_ctl.GetFileSize()    << '\n';
	std::cout << "GetMmapSize   : " << mfstore_ctl.GetMmapSize()    << '\n';
	std::cout << "GetAllocGran  : " << mfstore_ctl.GetAllocGran()   << '\n';
	std::cout << "GetMmapAddress: " << mfstore_ctl.GetMmapAddress() << '\n';
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int return_code = EXIT_SUCCESS;

	try {
		uint64_t mmap_size = 0;
		if (argc < 3) {
			if (argc == 2)
				mmap_size = CheckIsNumericString<uint64_t>(argv[1]);
			TEST_CreateMFStore(false, mmap_size);
		}
		else
			throw std::invalid_argument("Unexpected command line arguments --- "
				"expected [ <mmap-size> ]");
	}
	catch (const std::exception &except) {
		return_code = EXIT_FAILURE;
		std::cerr << "\n\nERROR: " << except.what() << std::endl;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

