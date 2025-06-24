// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB MFStore Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  FixUpValues.hpp

   File Description  :  Include file for functions to check MFStore values.

   Revision History  :  2021-02-14 --- Creation.
                           Michael L. Brock

      Copyright Michael L. Brock 2021 - 2024.
      Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
      http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__MFStore__FixUpValues_hpp__HH

#define HH__MLB__MFStore__FixUpValues_hpp__HH 1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
   \file FixUpValues.hpp

   \brief   Definition of the mmap section class.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <MFStore/MFStore.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace MFStore {

// ////////////////////////////////////////////////////////////////////////////
MFStoreLen CheckStorageGranularity(MFStoreLen storage_gran);

MFStoreLen CheckLengthHelper(MFStoreLen qty_value, MFStoreLen storage_gran,
	const char *qty_name);
MFStoreLen CheckOffsetHelper(MFStoreLen qty_value, MFStoreLen storage_gran,
	const char *qty_name);
MFStoreLen CheckSizeHelper(MFStoreLen qty_value, MFStoreLen storage_gran,
	const char *qty_name);
MFStoreLen CheckFileSize(MFStoreLen file_size,
	MFStoreLen storage_gran = MFStoreAllocGran);
MFStoreLen CheckFileSizePending(MFStoreLen file_size_pending,
	MFStoreLen storage_gran = MFStoreAllocGran);

void       CheckFileSizeAndFileSizePendingGE(MFStoreLen file_size,
	MFStoreLen file_size_pending, MFStoreLen storage_gran = MFStoreAllocGran);
void       CheckFileSizeAndFileSizePending(MFStoreLen file_size,
	MFStoreLen file_size_pending, MFStoreLen storage_gran = MFStoreAllocGran);

void       CheckInitialFileAndMmapSizes(MFStoreLen file_size,
	MFStoreLen mmap_size, MFStoreLen storage_gran = MFStoreAllocGran);
// ////////////////////////////////////////////////////////////////////////////

} // namespace MFStore

} // namespace MLB

#endif // #ifndef HH__MLB__MFStore__FixUpValues_hpp__HH

