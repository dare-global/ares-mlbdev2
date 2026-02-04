// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB Logger Library Include File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  Logger.hpp

   File Description  :  Header file for the Logger library.

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

#ifndef HH__MLB__Utility__Logger_hpp__HH

#define HH__MLB__Utility__Logger_hpp__HH 1

#define MLB__LIB__Logger

// ////////////////////////////////////////////////////////////////////////////
/**
   \file    Logger.hpp

   \brief   The header file for the Logger library.
*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/Utility.hpp>

#include <boost/config.hpp>

// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
/*
   We _can't_ use thread-local storage for our log ostreams if Boost says the
   compiler doesn't support them or if the C++ standard supported by the
   compiler is earlier than C++ 11.

   Alternatively, we _won't_ use thread-local storage for our log ostream if
   we've defined the manifest constant 'MLB_LOGGER_DO_NOT_USE_THREAD_LOCAL'.
*/
// ////////////////////////////////////////////////////////////////////////////
// C++23 required - thread_local is always available
#ifndef MLB_LOGGER_DO_NOT_USE_THREAD_LOCAL
# if defined(BOOST_NO_CXX11_THREAD_LOCAL)
#  define MLB_LOGGER_DO_NOT_USE_THREAD_LOCAL    1
# endif // # if defined(BOOST_NO_CXX11_THREAD_LOCAL)
#endif // #ifndef MLB_LOGGER_DO_NOT_USE_THREAD_LOCAL
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
#define Logger_IMPL_001_MUTEX_MAPPER  "IMPL_001_MUTEX_MAPPER"
#define Logger_IMPL_002_THREAD_LOCAL  "IMPL_002_THREAD_LOCAL"

/*
   IMPL NOTE: Select one of the two macro definitions based upon whether the
              current implementation build uses the mutex-locked map or the
              thread-local storage.
*/
#ifdef MLB_LOGGER_DO_NOT_USE_THREAD_LOCAL
# define Logger_IMPL_CURRENT  Logger_IMPL_001_MUTEX_MAPPER
#else
# define Logger_IMPL_CURRENT  Logger_IMPL_002_THREAD_LOCAL
#endif // #ifdef MLB_LOGGER_DO_NOT_USE_THREAD_LOCAL

/*
   IMPL NOTE: This is the version of the implementation build (mutex-locked
              map or thread-local storage).
*/
#define Logger_VER_CURRENT   "VER_001"

// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Set up library symbol visibility ...
// ////////////////////////////////////////////////////////////////////////////

#ifdef MLB__LIB__Logger___NOT
# ifdef LOGGER_DLL_EXPORTS
#  define API_LOGGER      MB_LIB_EXPORT
# else
#  define API_LOGGER      MB_LIB_IMPORT
# endif // #ifdef LOGGER_DLL_EXPORTS
#else
# define API_LOGGER
#endif // #ifdef MLB__LIB__Logger___NOT

   // Define MB_LIB_THIS_SIDE to be the {im|ex}port side for shared libs...
#ifdef MB_LIB_THIS_SIDE
#undef MB_LIB_THIS_SIDE
#endif // #ifdef MB_LIB_THIS_SIDE
#define MB_LIB_THIS_SIDE   API_LOGGER

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

#endif // #ifndef HH__MLB__Utility__Logger_hpp__HH

