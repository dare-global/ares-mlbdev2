// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB MFStore Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  StorageSizeIncrease.cpp

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

#include <MMapStorage/StorageSizeIncrease.hpp>

//#include <Utility/EmitterSep.hpp>
#include <Utility/GranularRound.hpp>
//#include <Utility/ThrowErrno.hpp>

#include <cstring>
#include <stdexcept>

//#include <boost/io/ios_state.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace MFStore {

/*
// ////////////////////////////////////////////////////////////////////////////
#ifndef _MSC_VER
typedef int FileHandle;
#else
typedef HANDLE FileHandle;
#endif // #ifndef _MSC_VER
// ////////////////////////////////////////////////////////////////////////////
*/

} // namespace MFStore

} // namespace MLB

// ////////////////////////////////////////////////////////////////////////////
// ****************************************************************************
// ****************************************************************************
// ****************************************************************************
// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace MFStore {

// ////////////////////////////////////////////////////////////////////////////
MFStoreLen IncreaseStorageSize(FileHandle file_handle,
	MFStoreLen &file_size, MFStoreLen &file_size_pending,
	MFStoreLen desired_file_size, MFStoreLen storage_gran)
{
	try {
		CheckFileSize(file_size, storage_gran);
		if (file_size_pending != file_size)
			throw std::invalid_argument("The pending file size (" +
				std::to_string(file_size_pending) + ") is not equal to the "
				"current file size.");
		if (desired_file_size == file_size)
			return(file_size);
		else if (desired_file_size < file_size)
			throw std::invalid_argument("The desired file size is less than the "
				"current file size.");
		CheckSize_Helper(desired_file_size, storage_gran, "desired file "));
***
	}
	catch (const std::exception &except) {
		throw std::invalid_argument("Unable to increase the storage size " +
			"to the requested size of " + std::to_string(desired_file_size) +
			" from the current file size of " + std::to_string(file_size) +
			": " + std::string(except.what()));
	}

	return(desired_file_size);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace MFStore

} // namespace MLB

