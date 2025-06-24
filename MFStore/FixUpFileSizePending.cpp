// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB MFStore Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  FixUpFileSizePending.cpp

   File Description  :  Implementation of the FixUpFileSizePending() function.

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

#include <MFStore/FixUpFileSizePending.hpp>

#include <MFStore/CheckValues.hpp>
#include <MFStore/EnsureFileBackingStore.hpp>

#include <Utility/GranularRound.hpp>

#include <filesystem>
#include <sstream>
#include <stdexcept>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace MFStore {

// ////////////////////////////////////////////////////////////////////////////
void FixUpFileSizePending(MFStoreControl &mfstore_ctl,
	std::atomic<MFStoreLen> &file_size, MFStoreLen &file_size_pending,
	MFStoreLen storage_gran)
{
	std::ostringstream o_str_header;

	try {
		CheckFileSizeAndFileSizePendingGE(file_size, file_size_pending,
			storage_gran);
		if (file_size_pending == file_size)
			return;
		o_str_header << "Open of " << mfstore_ctl.GetFileName() << " for "
			"writing shows that the stored original file size value of " <<
			file_size << " is not equal to the stored pending file size " <<
			"value of " << file_size_pending << ": ";
		std::uintmax_t actual_file_size = std::filesystem::file_size(
			std::filesystem::path(mfstore_ctl.GetFileName()));
		if (actual_file_size < file_size)
			throw std::logic_error("The actual file size (" +
				std::to_string(actual_file_size) + ") is less than the stored "
				"original file size.");
		if (actual_file_size > file_size_pending)
			throw std::logic_error("The actual file size (" +
				std::to_string(actual_file_size) + ") is greater than the stored "
				"pending file size.");
		if (actual_file_size == file_size) {
#if MFStore_WITH_LOGGING
			LogWarning << o_str_header.str() << ": Because the actual file size "
				"is equal to the stored original file size, the current logic "
				"will reset the pending file size to that value.\n";
#endif // #if MFStore_WITH_LOGGING
			file_size_pending = file_size;
			return;
		}
		if (actual_file_size == file_size_pending) {
#if MFStore_WITH_LOGGING
			LogWarning << o_str_header.str() << ": Because the actual file size "
				"is equal to the stored pending file size, the current logic will "
				"attempt to ensure allocation of backing storage to the " <<
				(file_size_pending - file_size) << " bytes between the stored "
				"original file size and the stored pending file size and, if "
				"successful, change the stored original file size to the value "
				"of the pending file size.\n";
#endif // #if MFStore_WITH_LOGGING
			EnsureFileBackingStore(mfstore_ctl, file_size, file_size_pending);
			file_size = file_size_pending;
			return;
		}
	}
	catch (const std::exception &except) {
		std::ostringstream o_str;
		o_str << "Fix-up attempt of the stored original file size (" <<
			file_size << ") vis-a-vis the stored pending file size (" <<
			file_size_pending << ") failed: " << except.what();
#if MFStore_WITH_LOGGING
		LogFatal << o_str.str() << '\n';
#endif // #if MFStore_WITH_LOGGING
		throw std::runtime_error(o_str.str());
	}
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace MFStore

} // namespace MLB

