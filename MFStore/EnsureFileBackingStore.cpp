// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB MFStore Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  EnsureFileBackingStore.cpp

   File Description  :  Implementation of the EnsureFileBackingStore()
                        functions.

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

#include <MFStore/EnsureFileBackingStore.hpp>

#include <Utility/ThrowSystemError.hpp>

#include <sstream>

#include <boost/interprocess/detail/os_file_functions.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace MFStore {

namespace {

// ////////////////////////////////////////////////////////////////////////////
void EnsureFileBackingStore_Helper(const char *file_name,
	MFStoreFileHandle file_handle, MFStoreOff store_offset,
	MFStoreLen store_length)
{
	try {
#ifdef __linux__
		int falloc_return =
			::fallocate(file_handle, 0, store_offset, store_length);
		if (falloc_return != 0) {
			std::string error_text = "Call to ::fallocate() returned " +
				std::to_string(falloc_return);
			MLB::Utility::ThrowSystemError(error_text);
		}
#elif defined(_POSIX_C_SOURCE) && (_POSIX_C_SOURCE >= 200112L)
		int falloc_return =
			::posix_fallocate(file_handle, store_offset, store_length);
		if (falloc_return != 0) {
			std::string error_text = "Call to ::posix_fallocate() returned " +
				std::to_string(falloc_return);
			MLB::Utility::ThrowErrno(falloc_return, error_text);
		}
#elif defined(BOOST_INTERPROCESS_WINDOWS)
		// In boost/interprocess/detail/os_file_functions.hpp .
		if (!boost::interprocess::ipcdetail::truncate_file(file_handle,
			store_offset + store_length))
			MLB::Utility::ThrowSystemError("NTFS file truncation attempt "
				"failed.");
#else
		throw std::logic_error("No logic to implement allocation of file system "
			"backing store to the file has been implemented.");
#endif // #ifdef __linux__
	}
	catch (const std::exception &except) {
		std::ostringstream o_str;
		o_str << "Attempt to ensure file ";
		if (file_name && *file_name)
			o_str << '\'' << file_name << "' ";
		else
			o_str << "*UNKNOWN-FILE-NAME*";
		o_str << "using file handle " << file_handle << " has backing "
			"storage assigned beginning at offset " << store_offset << " and "
			"continuing for " << store_length << " bytes failed: " <<
			except.what();
		throw std::logic_error(o_str.str());
	}
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
void EnsureFileBackingStore(MFStoreFileHandle file_handle,
	MFStoreOff store_offset, MFStoreLen store_length)
{
	EnsureFileBackingStore_Helper(nullptr, file_handle,
		store_offset, store_length);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void EnsureFileBackingStore(const char *file_name,
	MFStoreFileHandle file_handle, MFStoreOff store_offset,
	MFStoreLen store_length)
{
	EnsureFileBackingStore_Helper(file_name, file_handle,
		store_offset, store_length);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void EnsureFileBackingStore(const std::string &file_name,
	MFStoreFileHandle file_handle, MFStoreOff store_offset,
	MFStoreLen store_length)
{
	EnsureFileBackingStore_Helper(file_name.c_str(), file_handle,
		store_offset, store_length);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void EnsureFileBackingStore(MFStoreControl &mfstore_ctl,
	MFStoreOff store_offset, MFStoreLen store_length)
{
	mfstore_ctl.CheckIsActive();

	EnsureFileBackingStore_Helper(mfstore_ctl.GetFileName().c_str(),
		mfstore_ctl.GetFileHandle(), store_offset, store_length);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace MFStore

} // namespace MLB

