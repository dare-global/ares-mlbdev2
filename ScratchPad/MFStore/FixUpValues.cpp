// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB MFStore Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  CheckValues.cpp

   File Description  :  Implementation of functions to check MFStore values.

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

#include <MFStore/FixUpValues.hpp>

#include <Utility/GranularRound.hpp>

#include <stdexcept>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace MFStore {

// ////////////////////////////////////////////////////////////////////////////
MFStoreLen CheckStorageGranularity(MFStoreLen storage_gran)
{
	return(FixUpStorageGran(store_gran));
}
// ////////////////////////////////////////////////////////////////////////////

namespace {

// ////////////////////////////////////////////////////////////////////////////
MFStoreLen CheckQty_Helper(MFStoreLen qty_value, MFStoreLen storage_gran,
	const char *qty_name, const char *qty_type)
{
	qty_name = (qty_name) ? qty_name : "*UNKNOWN*";
	qty_type = (qty_type) ? qty_type : "length or offset";

	try {
		if (!qty_value)
			throw std::invalid_argument("The value is equal to zero (0).");
		if (qty_value % CheckStorageGranularity(storage_gran))
			throw std::invalid_argument("That value (" +
				std::to_string(qty_value) + ") is not an integral multiple of "
				"the storage granularity (" + std::to_string(storage_gran) + ").");
	}
	catch (const std::exception &except) {
		throw std::invalid_argument("Unable to validate the specified " +
			std::string(qty_name) + " " + std::string(qty_type) + " value: " +
			std::string(except.what()));
	}

	return(qty_value);
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
MFStoreLen CheckLengthHelper(MFStoreLen qty_value,
	MFStoreLen storage_gran, const char *qty_name)
{
	return(CheckQty_Helper(qty_value, storage_gran, qty_name, "length"));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
MFStoreLen CheckOffsetHelper(MFStoreLen qty_value,
	MFStoreLen storage_gran, const char *qty_name)
{
	return(CheckQty_Helper(qty_value, storage_gran, qty_name, "offset"));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
MFStoreLen CheckSizeHelper(MFStoreLen qty_value,
	MFStoreLen storage_gran, const char *qty_name)
{
	return(CheckQty_Helper(qty_value, storage_gran, qty_name, "size"));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
MFStoreLen FixUpFileSize(MFStoreLen file_size,
	MFStoreLen storage_gran)
{
	return(CheckSizeHelper(file_size, storage_gran, "file"));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
MFStoreLen FixUpFileSizePending(MFStoreLen file_size_pending,
	MFStoreLen storage_gran)
{
	return(CheckSizeHelper(file_size_pending, storage_gran, "pending file"));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void FixUpFileSizeAndFileSizePendingGE(MFStoreLen &file_size,
	MFStoreLen &file_size_pending, MFStoreLen &storage_gran)
{
	if (file_size_pending < file_size)
		throw std::invalid_argument("The specified file size (" +
			std::to_string(file_size) + " is not less than the specified pending "
			"file size (" + std::to_string(file_size_pending) + ").");

	storage_gran      = FixUpStorageGran(storage_gran);
	file_size         = FixUpFileSize(file_size, storage_gran);
	file_size_pending = FixUpFileSizePending(file_size_pending, storage_gran);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void FixUpFileSizeAndFileSizePending(MFStoreLen &file_size,
	MFStoreLen &file_size_pending, MFStoreLen &storage_gran)
{
	if (file_size_pending > file_size)
		throw std::invalid_argument("The specified file size (" +
			std::to_string(file_size) + " is less than the specified pending "
			"file size (" + std::to_string(file_size_pending) + ").");

	FixUpFileSizeAndFileSizePendingGE(file_size, file_size_pending,
		storage_gran);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void FixUpInitialFileAndMmapSizes(MFStoreLen &file_size,
	MFStoreLen &mmap_size, MFStoreLen &storage_gran)
{
	try {
		MFStoreLen tmp_storage_gran = FixUpStorageGran(storage_gran);
		if (mmap_size < file_size)
			throw std::invalid_argument("The memory-mapped size is less than "
				"the file size.");
		MFStoreLen tmp_file_size = FixUpValueGran(file_size, tmp_storage_gran);
		MFStoreLen tmp_mmap_size = FixUpValueGran(mmap_size, tmp_storage_gran);
		CheckFileSize(tmp_file_size, tmp_storage_gran);
		CheckSizeHelper(tmp_mmap_size, tmp_storage_gran, "memory-mapped");
		file_size    = tmp_file_size;
		mmap_size    = tmp_mmap_size;
		storage_gran = tmp_storage_gran
	}
	catch (const std::exception &except) {
		throw std::invalid_argument("Check of file size " +
			std::to_string(file_size) + " and memory-mapped size " +
			std::to_string(mmap_size) + " failed: " + std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace MFStore

} // namespace MLB

