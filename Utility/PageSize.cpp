// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB Utility Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  PageSize.cpp

   File Description  :  Implementation of the portable page size functions.

   Revision History  :  1998-04-08 --- Creation.
                           Michael L. Brock
                        2023-01-05 --- Migration to C++ MlbDev2/Utility.
                           Michael L. Brock

      Copyright Michael L. Brock 1998 - 2023.
      Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
      http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/PageSize.hpp>

#ifndef _MSC_VER
# include <unistd.h>
#elif (_MSC_VER < 1900)
# include <Windows.h>
#else
# pragma warning(push)
# pragma warning(disable:5039)
# include <Windows.h>
# pragma warning(pop)
#endif // #ifndef _MSC_VER

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
unsigned int GetPageSize()
{
#ifdef __MSDOS__
	return(4096);								// Whatever...
#elif _Windows
	SYSTEM_INFO system_data;

	::GetSystemInfo(&system_data);

	return(static_cast<unsigned int>(system_data.dwPageSize));
#elif __SVR4
	int return_code = ::sysconf(_SC_PAGESIZE);

	if (return_code == -1)
		ThrowErrno("Invocation of 'sysconf(_SC_PAGESIZE)' failed");

	return(static_cast<unsigned int>(return_code));
#else
	return(static_cast<unsigned int>(::getpagesize()));
#endif // #ifdef __MSDOS__
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
unsigned int GetPageAllocGranularitySize()
{
#if _Windows
   SYSTEM_INFO system_data;

   ::GetSystemInfo(&system_data);

   return(static_cast<unsigned int>(system_data.dwAllocationGranularity));
#else
   return(GetPageSize());
#endif /* #if _Windows */
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#ifdef TEST_MAIN

#include <cstdlib>
#include <iostream>

using namespace MLB::Utility;

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	try {
		std::cout << "GetPageSize()                : " <<
			GetPageSize() << std::endl;
		std::cout << "GetPageAllocGranularitySize(): " <<
			GetPageAllocGranularitySize() << std::endl;
	}
	catch (const std::exception &except) {
		std::cout << std::endl;
		std::cout << "Regression test error: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

