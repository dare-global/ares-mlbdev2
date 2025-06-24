// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  HostName.hpp

   File Description  :  Implementation of functions to get and set the
                        host name.

   Revision History  :  1998-04-08 --- Creation
                           Michael L. Brock
                        2023-01-09 --- Migration to C++ MlbDev2/Utility.
                           Michael L. Brock

      Copyright Michael L. Brock 1998 - 2023.
      Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
      http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__Utility__HostName_hpp__HH

#define HH__MLB__Utility__HostName_hpp__HH   1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
   \file HostName.hpp

   \brief   The host name functions header file.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/Utility.hpp>

#include <string>

#ifndef _MSC_VER
# ifdef __linux__
#  include <sys/param.h>
# else
#  include <unistd.h>
# endif // # ifdef __linux__
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
// Added 255 to max host names so as to ensure space to handle FQDNs.
#ifdef _Windows
const std::size_t MaxHostNameLength = MAX_COMPUTERNAME_LENGTH + 255;
#elif __MSDOS__
const std::size_t MaxHostNameLength = 64;
#elif sun
const std::size_t MaxHostNameLength = MAXHOSTNAMELEN + 255;
#else
const std::size_t MaxHostNameLength = MAXHOSTNAMELEN + 255;
#endif // #ifdef _Windows
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
API_UTILITY std::string  GetHostName();
API_UTILITY char        *GetHostName(char *host_name,
	std::size_t host_name_length);

API_UTILITY std::string  GetHostNameShort();
API_UTILITY std::string  GetHostNameCanonical();

API_UTILITY void         SetHostName(const std::string &host_name);
API_UTILITY void         SetHostName(const char *host_name);

API_UTILITY std::string  ConvertHostNameToShort(const std::string &host_name);
API_UTILITY std::string  ConvertHostNameToCanonical(
	const std::string &host_name);
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__HostName_hpp__HH

