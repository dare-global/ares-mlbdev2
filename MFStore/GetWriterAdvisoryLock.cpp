// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB MFStore Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  GetWriterAdvisoryLock.cpp

   File Description  :  Implementation of the GetWriterAdvisoryLock()
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

#include <MFStore/GetWriterAdvisoryLock.hpp>

#include <Utility/ThrowErrno.hpp>

#include <sstream>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace MFStore {

namespace {

// ////////////////////////////////////////////////////////////////////////////
void GetWriterAdvisoryLock_Helper(const char *file_name,
	MFStoreFileHandle file_handle)
{
	try {
#ifdef __unix
		struct flock my_flock{ };
		my_flock.l_type    = F_WRLCK;
		my_flock.l_whence  = SEEK_SET;
		my_flock.l_start   = 0;
		my_flock.l_len     = 0;
		int fcntl_return = ::fcntl(file_handle, F_SETLK, &my_flock);
		if (fcntl_return != 0) {
			std::ostringstream o_str;
			o_str << "Call to ::fcntl() returned " << fcntl_return;
			if ((errno == EACCES) || (errno == EAGAIN))
				o_str << ": Because errno has been set to " << errno <<
					((errno == EACCES) ? " (EACCES" : " (EAGAIN") << ") it seems "
					"likely the file has been opened for writing by another "
					"process which has acquired the writer advisory lock";
			MLB::Utility::ThrowErrno(o_str.str());
		}
#else
		throw std::logic_error("No logic to implement advisory file locking "
			"is available.");
#endif // #ifdef __unix
	}
	catch (const std::exception &except) {
		std::ostringstream o_str;
		o_str << "Attempt to acquire a writer advisory lock for file ";
		if (file_name && *file_name)
			o_str << '\'' << file_name << '\'';
		else
			o_str << "*UNKNOWN-FILE-NAME*";
		o_str << " using file handle " << file_handle << " failed: " <<
			except.what();
		throw std::logic_error(o_str.str());
	}
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
void GetWriterAdvisoryLock(MFStoreFileHandle file_handle)
{
	GetWriterAdvisoryLock_Helper(nullptr, file_handle);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void GetWriterAdvisoryLock(const char *file_name,
	MFStoreFileHandle file_handle)
{
	GetWriterAdvisoryLock_Helper(file_name, file_handle);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void GetWriterAdvisoryLock(const std::string &file_name,
	MFStoreFileHandle file_handle)
{
	GetWriterAdvisoryLock_Helper(file_name.c_str(), file_handle);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void GetWriterAdvisoryLock(MFStoreControl &mfstore_ctl)
{
	mfstore_ctl.CheckIsActive();

	GetWriterAdvisoryLock_Helper(mfstore_ctl.GetFileName().c_str(),
		mfstore_ctl.GetFileHandle());
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

#include <Utility/CriticalEventHandler.hpp>
#include <Utility/Sleep.hpp>

#include <iostream>

namespace {

// ////////////////////////////////////////////////////////////////////////////
void TEST_GetWriterAdvisoryLock()
{
	using namespace MLB::MFStore;

	std::string file_name("TEST_MAIN.TEST_GetWriterAdvisoryLock.txt");

	std::cout << "Creating new file or opening existing file '" <<
		file_name << " ..." << std::flush;

	int file_handle = ::open(file_name.c_str(), O_CREAT|O_RDWR,
		S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

	if (file_handle < 0) {
		std::string error_text = "Call to ::open() returned " +
			std::to_string(file_handle);
		MLB::Utility::ThrowErrno(error_text);
	}

	std::cout << " done.\n" << std::endl;

	std::cout << "Attempting to acquire a writer advisory lock ..." <<
		std::flush;

	GetWriterAdvisoryLock(file_name, file_handle);

	std::cout << " done.\n" << std::endl;

	std::cout << "Sleeping until receipt of SIGINT or SIGTERM ..." << std::endl;

	while (!MLB::Utility::CriticalEventHandler::GetFlag())
		MLB::Utility::SleepMilliSecs(100);

	std::cout << " awakened.\n";

	std::cout << "Exiting.\n" << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		MLB::Utility::CriticalEventHandler event_handler;
		TEST_GetWriterAdvisoryLock();
	}
	catch (const std::exception &except) {
		return_code = EXIT_FAILURE;
		std::cerr << "\n\nERROR: " << except.what() << std::endl;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

