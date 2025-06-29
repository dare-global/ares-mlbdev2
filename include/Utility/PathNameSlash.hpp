// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB Utility Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  PathNameSlash.hpp

   File Description  :  Include file for the path name slash support logic.

   Revision History  :  1985-10-23 --- Creation in genfuncs/genfuncs.h.
                           Michael L. Brock
                        1998-04-08 --- Modified for use with C++.
                           Michael L. Brock
                        2024-08-04 --- Migration to C++ MlbDev2/Utility.
                           Michael L. Brock

      Copyright Michael L. Brock 1985 - 2024.
      Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
      http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

#ifndef HH__MLB__Utility__PathNameSlash_hpp__HH

#define HH__MLB__Utility__PathNameSlash_hpp__HH 1

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/**
   \file PathNameSlash.hpp

   \brief   The path name slash logic header file.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/Utility.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
API_UTILITY bool        IsPathNameSlash(char in_path_char);
API_UTILITY bool        IsPathNameSlash(const char *in_path_string);

API_UTILITY char        GetPathNameSlashCanonical();
API_UTILITY const char *GetPathNameSlashCanonicalString();
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__PathNameSlash_hpp__HH

