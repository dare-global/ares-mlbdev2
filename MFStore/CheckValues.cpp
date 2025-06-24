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

#include <MFStore/CheckValues.hpp>

#include <Utility/GranularRound.hpp>

#include <stdexcept>
#include <string>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace MFStore {

// ////////////////////////////////////////////////////////////////////////////
MFStoreLen CheckStorageGranularity(MFStoreLen storage_gran)
{
	if (!storage_gran)
		throw std::invalid_argument("The specified storage granularity is 0.");
	else if (storage_gran % MFStoreAllocGran)
		throw std::invalid_argument("The specified storage granularity (" +
			std::to_string(storage_gran) + ") is not an integral multiple of "
			"the basic allocation granularity (" +
			std::to_string(MFStoreAllocGran) + ").");

	return(storage_gran);
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
MFStoreLen CheckFileSize(MFStoreLen file_size,
	MFStoreLen storage_gran)
{
	return(CheckSizeHelper(file_size, storage_gran, "file"));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
MFStoreLen CheckFileSizePending(MFStoreLen file_size_pending,
	MFStoreLen storage_gran)
{
	return(CheckSizeHelper(file_size_pending, storage_gran, "pending file"));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void CheckFileSizeAndFileSizePendingGE(MFStoreLen file_size,
	MFStoreLen file_size_pending, MFStoreLen storage_gran)
{
	CheckFileSize(file_size, storage_gran);
	CheckFileSizePending(file_size_pending, storage_gran);

	if (file_size_pending < file_size)
		throw std::invalid_argument("The specified file size (" +
			std::to_string(file_size) + " is not less than the specified pending "
			"file size (" + std::to_string(file_size_pending) + ").");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void CheckFileSizeAndFileSizePending(MFStoreLen file_size,
	MFStoreLen file_size_pending, MFStoreLen storage_gran)
{
	CheckFileSizeAndFileSizePendingGE(file_size, file_size_pending,
		storage_gran);

	if (file_size_pending > file_size)
		throw std::invalid_argument("The specified file size (" +
			std::to_string(file_size) + " is less than the specified pending "
			"file size (" + std::to_string(file_size_pending) + ").");
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void CheckInitialFileAndMmapSizes(MFStoreLen file_size, MFStoreLen mmap_size,
	MFStoreLen storage_gran)
{
	try {
		CheckFileSize(file_size, storage_gran);
		CheckSizeHelper(mmap_size, storage_gran, "memory-mapped");
		if (mmap_size < file_size)
			throw std::invalid_argument("The memory-mapped size is less than "
				"the file size.");
	}
	catch (const std::exception &except) {
		throw std::invalid_argument("Check of file size " +
			std::to_string(file_size) + " and memory-mapped size " +
			std::to_string(mmap_size) + " failed: " + std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool CheckOffset(MFStoreLen file_size, MFStoreLen src_offset,
	bool throw_on_error)
{
	if (src_offset < file_size)
		return(true);
	else if (throw_on_error)
		throw std::invalid_argument("The specified offset (" +
			std::to_string(src_offset) + ") is not less than the file size (" +
			std::to_string(file_size) + ").");

	return(false);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
bool CheckExtent(MFStoreLen file_size, MFStoreLen src_offset,
	MFStoreLen src_length, bool throw_on_error)
{
	if (!CheckOffset(file_size, src_offset, throw_on_error))
		return(false);

	if ((!src_length) || ((src_offset + src_length) <= file_size))
		return(true);
	else if (throw_on_error)
		throw std::invalid_argument("The specified extent starting at offset (" +
			std::to_string(src_offset) + ") with an extent length of " +
			std::to_string(src_length) + " bytes is greater than the file "
			"size (" + std::to_string(file_size) + ").");

	return(false);
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace MFStore

} // namespace MLB

