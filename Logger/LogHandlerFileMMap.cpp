// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB Utility Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  LogHandlerFileMMap.cpp

   File Description  :  Implementation of logging to memory-mapped files.

   Revision History  :  1993-10-02 --- Creation of predecessor 'mlog' facility.
                           Michael L. Brock
                        2005-01-02 --- New ostream-based log model.
                           Michael L. Brock
                        2023-01-05 --- Migration to C++ MlbDev2/Utility.
                           Michael L. Brock

      Copyright Michael L. Brock 1993 - 2023.
      Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
      http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Logger/LogHandlerFileMMap.hpp>

#include <Utility/ExceptionRethrow.hpp>
#include <Utility/GranularRound.hpp>
#include <Utility/PageSize.hpp>
#include <Utility/AnyToString.hpp>

#include <fstream>

#include <boost/interprocess/detail/file_wrapper.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

namespace {

// ////////////////////////////////////////////////////////////////////////////
const std::size_t LogFileMMapDefaultAllocSize = 1 << 20;
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
LogHandlerFileMMap::LogHandlerFileMMap()
	:LogHandlerFileBase()
	,eol_string_("\n")
	,eol_string_length_(eol_string_.size())
	,page_alloc_size_(GetPageAllocGranularitySize())
	,chunk_alloc_size_(GranularRoundUp(LogFileMMapDefaultAllocSize,
		page_alloc_size_))
	,mapping_sptr_()
	,region_sptr_()
	,mapping_size_(0)
	,mapping_offset_(0)
	,write_offset_(0)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogHandlerFileMMap::LogHandlerFileMMap(const char *file_name,
	LogHandlerFileBaseFlag flags)
	:LogHandlerFileBase(flags)
	,eol_string_("\n")
	,eol_string_length_(eol_string_.size())
	,page_alloc_size_(GetPageAllocGranularitySize())
	,chunk_alloc_size_(GranularRoundUp(LogFileMMapDefaultAllocSize,
		page_alloc_size_))
	,mapping_sptr_()
	,region_sptr_()
	,mapping_size_(0)
	,mapping_offset_(0)
	,write_offset_(0)
{
	OpenFile(file_name);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogHandlerFileMMap::LogHandlerFileMMap(const std::string &file_name,
	LogHandlerFileBaseFlag flags)
	:LogHandlerFileBase(flags)
	,eol_string_("\n")
	,eol_string_length_(eol_string_.size())
	,page_alloc_size_(GetPageAllocGranularitySize())
	,chunk_alloc_size_(GranularRoundUp(LogFileMMapDefaultAllocSize,
		page_alloc_size_))
	,mapping_sptr_()
	,region_sptr_()
	,mapping_size_(0)
	,mapping_offset_(0)
	,write_offset_(0)
{
	OpenFile(file_name);
}
// ////////////////////////////////////////////////////////////////////////////

/* 
	CODE NOTE: Decide whether to remove the elaborate instances of the OpenFile() overload.
// ////////////////////////////////////////////////////////////////////////////
LogHandlerFileMMap::LogHandlerFileMMap(const char *base_name,
	const char *dir_name, LogHandlerFileBaseFlag flags)
	:LogHandlerFileBase(flags)
	,eol_string_("\n")
	,eol_string_length_(eol_string_.size())
	,page_alloc_size_(GetPageAllocGranularitySize())
	,chunk_alloc_size_(GranularRoundUp(LogFileMMapDefaultAllocSize,
		page_alloc_size_))
	,mapping_sptr_()
	,region_sptr_()
	,mapping_size_(0)
	,mapping_offset_(0)
	,write_offset_(0)
{
	OpenFile(base_name, dir_name);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogHandlerFileMMap::LogHandlerFileMMap(const std::string &base_name,
	const std::string &dir_name, LogHandlerFileBaseFlag flags)
	:LogHandlerFileBase(flags)
	,eol_string_("\n")
	,eol_string_length_(eol_string_.size())
	,page_alloc_size_(GetPageAllocGranularitySize())
	,chunk_alloc_size_(GranularRoundUp(LogFileMMapDefaultAllocSize,
		page_alloc_size_))
	,mapping_sptr_()
	,region_sptr_()
	,mapping_size_(0)
	,mapping_offset_(0)
	,write_offset_(0)
{
	OpenFile(base_name, dir_name);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogHandlerFileMMap::LogHandlerFileMMap(const char *base_name,
	const char *dir_name, const MLB::Utility::TimeT &start_time,
	LogHandlerFileBaseFlag flags)
	:LogHandlerFileBase(flags)
	,eol_string_("\n")
	,eol_string_length_(eol_string_.size())
	,page_alloc_size_(GetPageAllocGranularitySize())
	,chunk_alloc_size_(GranularRoundUp(LogFileMMapDefaultAllocSize,
		page_alloc_size_))
	,mapping_sptr_()
	,region_sptr_()
	,mapping_size_(0)
	,mapping_offset_(0)
	,write_offset_(0)
{
	OpenFile(base_name, dir_name, start_time);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogHandlerFileMMap::LogHandlerFileMMap(const std::string &base_name,
	const std::string &dir_name, const MLB::Utility::TimeT &start_time,
	LogHandlerFileBaseFlag flags)
	:LogHandlerFileBase(flags)
	,eol_string_("\n")
	,eol_string_length_(eol_string_.size())
	,page_alloc_size_(GetPageAllocGranularitySize())
	,chunk_alloc_size_(GranularRoundUp(LogFileMMapDefaultAllocSize,
		page_alloc_size_))
	,mapping_sptr_()
	,region_sptr_()
	,mapping_size_(0)
	,mapping_offset_(0)
	,write_offset_(0)
{
	OpenFile(base_name, dir_name, start_time);
}
// ////////////////////////////////////////////////////////////////////////////
*/

// ////////////////////////////////////////////////////////////////////////////
LogHandlerFileMMap::~LogHandlerFileMMap()
{
	bool           truncate_file_flag = false;
	MyMappingValue truncate_file_size = 0;
	LogLockScoped  my_lock(the_lock_);

	if (region_sptr_.get() != NULL) {
		/*
			Truncate the file just beyond the last byte written in the current
			chunk. This drops the zero-filled portion of the file...
		*/
		truncate_file_flag = true;
		truncate_file_size = mapping_offset_ + write_offset_;
		try {
			region_sptr_->flush();
		}
		catch (const std::exception &) {
		}
	}

	region_sptr_.reset();
	mapping_sptr_.reset();

	if (truncate_file_flag) {
		try {
			boost::interprocess::ipcdetail::file_wrapper tmp_file(
				boost::interprocess::open_only_t(),
				out_file_name_.c_str(), boost::interprocess::read_write);
//			TruncateFileSize(out_file_name_, truncate_file_size);
			if (!boost::interprocess::ipcdetail::truncate_file(
				tmp_file.get_mapping_handle().handle, truncate_file_size))
			{
				; // Failed for some reason. Helpless to fix in a dtor.
			}
		}
		catch (const std::exception &) {
			;	// TLILB
		}
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LogHandlerFileMMap::InstallHandlerImpl()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LogHandlerFileMMap::RemoveHandlerImpl()
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LogHandlerFileMMap::OpenFileImpl(const char *file_name)
{
	bool               file_created = false;
	unsigned long long file_size    = 0;
	std::string        tmp_file_name;
	bool               file_existed;

	file_existed = ResolveFilePathGeneral(file_name, tmp_file_name, "",
		false, false, true);

	using namespace boost::interprocess;

	try {
		unsigned int   mapping_size   = chunk_alloc_size_;
		MyMappingValue mapping_offset = 0;
		MyMappingValue write_offset   = 0;
		if (file_existed) {
			/*
				If the file already exists, we attempt to determine whether the
				file ends in a string of ASCII NUL characters. If any such string
				is found, we'll attempt to 'back over' it.

				The reason such a string can come into existence is a result of
				the fact that memory-mapping, in both Windows and various Unices,
				operates on pages, and not characters. Pages are padded-out by
				being zero-filled upon assignment to the process by the operating
				system.

				The destructor logic is written to truncate the file such that
				file contents will include only characters written. But if a
				run time mishap prevents the destructor from completing correctly,
				the removal of the padding bytes may not be performed.
			*/
			if (my_flags_ & DoNotAppend) {
				TruncateFileSize(tmp_file_name, 0);
				file_size = 0;
			}
			else
				file_size = MLB::Utility::GetFileSize(tmp_file_name);
			if (!file_size) {
				mapping_offset = 0;
				write_offset   = 0;
			}
			if (!(file_size % page_alloc_size_)) {
				unsigned int   tmp_map_size;
				MyMappingValue tmp_map_offset;
				if (file_size < chunk_alloc_size_) {
					tmp_map_size   = static_cast<unsigned int>(file_size);
					tmp_map_offset = 0;
				}
				else {
					tmp_map_size   = chunk_alloc_size_;
					tmp_map_offset = file_size - chunk_alloc_size_;
				}
				file_mapping  tmp_mapping(tmp_file_name.c_str(), read_write);
				mapped_region tmp_region(tmp_mapping, read_write,
					static_cast<boost::interprocess::offset_t>(tmp_map_offset),
					tmp_map_size);
				const char *start_ptr = 
					static_cast<const char *>(tmp_region.get_address());
				const char *end_ptr   = start_ptr + tmp_map_size;
				if (end_ptr[-1]) {
					TruncateFileSize(tmp_file_name, file_size + chunk_alloc_size_);
					mapping_offset = file_size;
					write_offset   = 0;
				}
				else {
					while ((end_ptr > start_ptr) && (!end_ptr[-1]))
						--end_ptr;
					if (end_ptr > start_ptr) {
						mapping_size   = tmp_map_size;
						mapping_offset = tmp_map_offset;
						write_offset   =
							static_cast<MyMappingValue>(end_ptr - start_ptr);
					}
					else {
						mapping_size   = tmp_map_size;
						mapping_offset = tmp_map_offset;
						write_offset   = 0;
					}
				}
			}
			else {
				file_size = GranularRoundUp<unsigned long long>(file_size,
					chunk_alloc_size_);
				TruncateFileSize(tmp_file_name, file_size);
				unsigned int   tmp_map_size   = chunk_alloc_size_;
				MyMappingValue tmp_map_offset = file_size - chunk_alloc_size_;
				file_mapping   tmp_mapping(tmp_file_name.c_str(), read_write);
				mapped_region  tmp_region(tmp_mapping, read_write,
					static_cast<boost::interprocess::offset_t>(tmp_map_offset),
					tmp_map_size);
				const char *start_ptr = 
					static_cast<const char *>(tmp_region.get_address());
				const char *end_ptr   = start_ptr + tmp_map_size;
				while ((end_ptr > start_ptr) && (!end_ptr[-1]))
					--end_ptr;
				if (end_ptr > start_ptr) {
					mapping_size   = tmp_map_size;
					mapping_offset = tmp_map_offset;
					write_offset   =
						static_cast<MyMappingValue>(end_ptr - start_ptr);
				}
				else {
					mapping_size   = tmp_map_size;
					mapping_offset = tmp_map_offset;
					write_offset   = 0;
				}
			}
		}
		else {
			/*
				Otherwise, the file doesn't exist. So create it...
			*/
			{
				std::ofstream this_file(tmp_file_name.c_str(),
					(!(my_flags_ & DoNotAppend)) ?
					(std::ios_base::app | std::ios_base::ate) :
					(std::ios_base::app | std::ios_base::trunc));
				if (this_file.fail())
					ThrowErrno("Open attempt for the new file failed.");
			}
			TruncateFileSize(tmp_file_name, mapping_size);
			file_created   = true;
			mapping_offset = 0;
			write_offset   = 0;
		}
		MyFileMappingSPtr mapping_sptr(
			new file_mapping(tmp_file_name.c_str(), read_write));
		MyMappedRegionSPtr region_sptr(
			new mapped_region(*mapping_sptr, read_write,
			static_cast<boost::interprocess::offset_t>(mapping_offset),
			mapping_size));
		{
			LogLockScoped my_lock(the_lock_);
			mapping_sptr_.swap(mapping_sptr);
			region_sptr_.swap(region_sptr);
			out_file_name_.swap(tmp_file_name);
			mapping_size_   = mapping_size;
			mapping_offset_ = mapping_offset;
			write_offset_   = write_offset;
		}
	}
	catch (const std::exception &except) {
		if (file_created) {
			try {
				RemoveFile(tmp_file_name, true);
			}
			catch (const std::exception &) {
				; // TLILB
			}
		}
		MLB::Utility::Rethrow(except, "Attempt to open log file '" +
			tmp_file_name + "' for memory-mapped write access failed: " +
			std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LogHandlerFileMMap::FlushImpl()
{
	if (region_sptr_.get() != NULL)
		region_sptr_->flush();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LogHandlerFileMMap::EmitLineImpl(const LogEmitControl &emit_control)
{
	if (region_sptr_.get() != NULL) {
		EnsureNeededSpace(emit_control.GetLeaderLength() +
			emit_control.line_buffer_.size() + eol_string_length_);
		::memcpy(GetCurrentPtr(), emit_control.GetLeaderPtr(),
			emit_control.GetLeaderLength());
		AddToOffset(emit_control.GetLeaderLength());
		::memcpy(GetCurrentPtr(), emit_control.line_buffer_.c_str(),
			emit_control.line_buffer_.size());
		AddToOffset(emit_control.line_buffer_.size());
		::memcpy(GetCurrentPtr(), eol_string_.c_str(), eol_string_length_);
		AddToOffset(eol_string_length_);
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LogHandlerFileMMap::EmitLiteralImpl(unsigned int literal_length,
	const char *literal_string)
{
	if (region_sptr_.get() != NULL) {
		EnsureNeededSpace(literal_length + eol_string_length_);
		::memcpy(GetCurrentPtr(), literal_string, literal_length);
		AddToOffset(literal_length);
		::memcpy(GetCurrentPtr(), eol_string_.c_str(), eol_string_length_);
		AddToOffset(eol_string_length_);
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void *LogHandlerFileMMap::GetCurrentPtr()
{
	return(static_cast<void *>(static_cast<char *>(
		region_sptr_->get_address()) + write_offset_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void LogHandlerFileMMap::AddToOffset(std::size_t added_offset)
{
	if ((write_offset_ + added_offset) > mapping_size_)
		throw std::invalid_argument("The value specified to "
			"'LogHandlerFileMMap::AddToOffset()' (" +
			AnyToString(added_offset) + ") when added to the current "
			"'write_offset_' value (" + AnyToString(write_offset_) +
			") is greater than the 'mapped_size_' member (" +
			AnyToString(mapping_size_) + ").");

	write_offset_ += added_offset;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
/*
	Assumes the lock has been acquired.
*/
void LogHandlerFileMMap::EnsureNeededSpace(std::size_t needed_length)
{
	MyMappingValue remaining_length = (write_offset_ <= mapping_size_) ?
		(mapping_size_ - write_offset_) : 0;

	//	Note that if equal we'll go ahead and allocate more space...
	if (needed_length < remaining_length)
		return;

	MyMappingValue mapping_offset = GranularRoundDown<MyMappingValue>(
		mapping_offset_ + write_offset_, page_alloc_size_);
	MyMappingValue write_offset   = (mapping_offset_ + write_offset_) -
		mapping_offset;
	MyMappingValue mapping_size   = (std::max)(chunk_alloc_size_,
		GranularRoundUp(needed_length, page_alloc_size_) +
		page_alloc_size_);

	TruncateFileSize(out_file_name_, mapping_offset + mapping_size);

	using namespace boost::interprocess;

	MyFileMappingSPtr        mapping_sptr(
		new file_mapping(out_file_name_.c_str(), read_write));
	MyMappedRegionSPtr        region_sptr(
		new mapped_region(*mapping_sptr, read_write,
		static_cast<boost::interprocess::offset_t>(mapping_offset),
		static_cast<std::size_t>(mapping_size)));

	mapping_sptr_.swap(mapping_sptr);
	region_sptr_.swap(region_sptr);

	mapping_size_   = static_cast<unsigned int>(mapping_size);
	mapping_offset_ = mapping_offset;
	write_offset_   = write_offset;
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

#include <Logger/LogHandlerFile.hpp>
#include <Logger/LogManager.hpp>
#include <Logger/LogTestSupport.hpp>

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	using namespace MLB::Utility;

	int return_code = EXIT_SUCCESS;

	try {
		//	Create a LogHandlerFile...
		LogHandlerPtr my_log_handler(
			new LogHandlerFileMMap("TestLogFileMMap.VERSION_OLD_001.log"));
		TEST_TestControl(my_log_handler, 10000, 200, 1, 2000000);
	}
	catch (const std::exception &except) {
		std::cerr << std::endl << std::endl << "ERROR: " << except.what() <<
			std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

