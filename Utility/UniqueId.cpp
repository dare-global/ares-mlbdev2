// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB Utility Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  UniqueId.cpp

   File Description  :  Implementation of the UniqueId class (and a few of the
                        related free functions in the Utility library).

   Revision History  :  1998-04-08 --- Creation.
                           Michael L. Brock
                        2024-09-12 --- Added to MlbDev2 Utility library.
                           Michael L. Brock

      Copyright Michael L. Brock 1998 - 2024.
      Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
      http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////
 
// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Utility/UniqueId.hpp>

#include <iomanip>

#ifdef __GNUC__
# include <string.h>
#endif // #ifdef __GNUC__

#include <boost/random.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

// ////////////////////////////////////////////////////////////////////////////
UniqueId::UniqueId()
	:id_(GetSystemUUID())
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
UniqueId::UniqueId(bool generate_uuid_flag)
{
	if (generate_uuid_flag)
		id_ = GetSystemUUID();
	else
		memset(&id_, '\0', sizeof(id_));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
UniqueId::UniqueId(const char *unique_id)
	:id_(FromString(unique_id).id_)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
UniqueId::UniqueId(const std::string &unique_id)
	:id_(FromString(unique_id).id_)
{
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream &UniqueId::ToStream(std::ostream &o_str) const
{
	o_str << id_;

	return(o_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::istream &UniqueId::FromStream(std::istream &i_str)
{
	i_str >> id_;

	return(i_str);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string UniqueId::ToString() const
{
	std::ostringstream o_str;

	ToStream(o_str);

	return(o_str.str());
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &UniqueId::ToString(std::string &out_string) const
{
/* 
	CODE NOTE: Obsoleted. TBR. 
	char buffer[UniqueIdStringLength + 1];

	return(out_string.assign(ToString(buffer)));
*/
	return(out_string.assign(ToString()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
char *UniqueId::ToString(char *out_string) const
{
/*
	CODE NOTE: Obsoleted. TBR. 
	std::ostringstream uuid_string;

	uuid_string << id_;

	return(::strcpy(out_string, uuid_string.str().c_str()));
*/
	return(::strcpy(out_string, ToString().c_str()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string UniqueId::ToStringByteOrder() const
{
	std::string out_string;

	return(ToStringByteOrder(out_string));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &UniqueId::ToStringByteOrder(std::string &out_string) const
{
	char buffer[UniqueIdStringLength + 1];

	return(out_string.assign(ToStringByteOrder(buffer)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
char *UniqueId::ToStringByteOrder(char *out_string) const
{
	const unsigned char *tmp_ptr_i =
		reinterpret_cast<const unsigned char *>(&id_);
	char                *tmp_ptr_o = out_string;
	unsigned int         count_1;

	for (count_1 = 0; count_1 < UniqueIdStringLength; ++count_1) {
		if ((count_1 == 8) || (count_1 == 13) || (count_1 == 18) ||
			(count_1 == 23))
			*tmp_ptr_o++ = '-';
		else {
			*tmp_ptr_o++ = "0123456789abcdef"[(*tmp_ptr_i >> 4) & 0x0F];
			*tmp_ptr_o++ = "0123456789abcdef"[*tmp_ptr_i        & 0x0F];
			++tmp_ptr_i;
			++count_1;
		}
	}

	*tmp_ptr_o = '\0';

	return(out_string);
}
// ////////////////////////////////////////////////////////////////////////////

#if 0
// ////////////////////////////////////////////////////////////////////////////
std::string UniqueId::ToStringRadix64() const
{
	std::string out_string;

	return(ToStringRadix64(out_string));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::string &UniqueId::ToStringRadix64(std::string &out_string) const
{
	char buffer[UniqueIdStringLengthRadix64 + 1];

	return(out_string.assign(ToStringRadix64(buffer)));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
char *UniqueId::ToStringRadix64(char *out_string) const
{
	NativeUuidType tmp_id = id_;

	unsigned int  count_1;
	char         *out_ptr = out_string;

	for (count_1 = 0; count_1 < sizeof(tmp_id);
		count_1 += sizeof(long), out_ptr += IntToString_MaxLen_Unsigned_64)
		IntToString(out_ptr,
			*reinterpret_cast<unsigned long *>(
			reinterpret_cast<char *>(&tmp_id) + count_1), 64, "0");

	return(out_string);
}
// ////////////////////////////////////////////////////////////////////////////
#endif // #if 0

// ////////////////////////////////////////////////////////////////////////////
UniqueId UniqueId::FromString(const std::string &uuid_string)
{
	return(FromString(uuid_string.c_str()));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
UniqueId UniqueId::FromString(const char *uuid_string)
{
	if ((uuid_string == NULL) || (!(*uuid_string)))
		throw std::invalid_argument("UUID string is empty.");

	if ((strlen(uuid_string) != UniqueIdStringLength) ||
		(uuid_string[8] != '-') || (uuid_string[13] != '-') ||
		(uuid_string[18] != '-') || (uuid_string[23] != '-')) {
		std::ostringstream error_text;
		error_text << "UUID string '" << uuid_string <<
			"' has an invalid format.";
		throw std::invalid_argument(error_text.str());
	}

	unsigned int count_1;
	for (count_1 = 0; count_1 < UniqueIdStringLength; ++count_1) {
		if ((count_1 != 8) && (count_1 != 13) && (count_1 != 18) &&
			(count_1 != 23)) {
			if (!isxdigit(*uuid_string)) {
				std::ostringstream error_text;
				error_text << "UUID string '" << uuid_string <<
					"' has an invalid format.";
				throw std::invalid_argument(error_text.str());
			}
		}
	}

	UniqueId tmp_uuid(false);

	std::stringstream io_str;

	io_str << uuid_string;
	io_str >> tmp_uuid.id_;

	return(tmp_uuid);
}
// ////////////////////////////////////////////////////////////////////////////

#if 0
#if (!defined(BOOST_CXX_VERSION)) || (BOOST_CXX_VERSION < 201703L)
// ////////////////////////////////////////////////////////////////////////////
constexpr bool UniqueId::operator < (const UniqueId &other) const
{
/*
	return(memcmp(&id_, &other.id_, sizeof(id_)) < 0);
*/
	return(*this < other);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
constexpr bool UniqueId::operator == (const UniqueId &other) const
{
/*
	return(memcmp(&id_, &other.id_, sizeof(id_)) == 0);
*/
	return(*this == other);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
constexpr bool UniqueId::operator != (const UniqueId &other) const
{
/*
	return(memcmp(&id_, &other.id_, sizeof(id_)) != 0);
*/
	return(*this != other);
}
// ////////////////////////////////////////////////////////////////////////////
#endif // #if (!defined(BOOST_CXX_VERSION)) || (BOOST_CXX_VERSION < 201703L)
#endif // #if 0

// ////////////////////////////////////////////////////////////////////////////
bool UniqueId::IsZero() const
{
	const char   *tmp_ptr = reinterpret_cast<const char *>(this);
	unsigned int  count_1 = sizeof(*this);

	while (count_1--) {
		if (*tmp_ptr++)
			return(false);
	}

	return(true);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void UniqueId::swap(UniqueId &other)
{
	NativeUuidType tmp_uuid(id_);

	id_       = other.id_;
	other.id_ = tmp_uuid;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
NativeUuidType UniqueId::GetSystemUUID()
{
	NativeUuidType uuid_data;

	uuid_data = boost::uuids::random_generator()();

	return(uuid_data);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::istream & operator >> (std::istream &i_str, UniqueId &datum)
{
	return(datum.FromStream(i_str));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
std::ostream & operator << (std::ostream &o_str, const UniqueId &datum)
{
/*
	CODE NOTE: Obsoleted. TBR. 
	char buffer[UniqueIdStringLength + 1];

	o_str <<
		datum.ToString(buffer);

   return(o_str);
*/
	return(datum.ToStream(o_str));
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

#include <Utility/EmitterSep.hpp>

#include <deque>
#include <iterator>
#include <list>
#include <set>

//using namespace MLB::Utility;

namespace {

// ////////////////////////////////////////////////////////////////////////////
void TEST_UniqueIdCore()
{
	using namespace MLB::Utility;

	std::cout << EmitterSep('=');
	std::cout << "TEST_UniqueIdCore()\n";
	std::cout << EmitterSep('-');
	UniqueId    request_id;
	UniqueId    request_id_tmp(false);
	std::cout << "UUID Start     : " <<
		request_id <<  '\n';
	std::cout << "UUID ToStream  : ";
	request_id.ToStream() << '\n';
	std::string uuid_as_string(request_id.ToString());
	std::cout << "UUID ToString  : " <<
		uuid_as_string << '\n';
	std::stringstream io_str;
	io_str << request_id;
	UniqueId uuid_from_stream;
	uuid_from_stream.FromStream(io_str);
	std::cout << "UUID FromStream: " <<
		uuid_from_stream <<  '\n';
	std::cout << "UUID FromString: " <<
		UniqueId::FromString(uuid_as_string) <<  '\n';
	std::cout << "UUID Byte Order: " <<
		request_id.ToStringByteOrder() <<  '\n';
/*
	std::cout << "UUID Radix 64  : " <<
		request_id.ToStringRadix64() << std::endl;
*/
	std::cout << EmitterSep('=') << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_BoostUuid()
{
	using namespace MLB::Utility;

	std::cout << EmitterSep('=');
	std::cout << "TEST_BoostUuid()\n";
	std::cout << EmitterSep('-');

	try {
		boost::uuids::uuid uuid_empty = boost::uuids::nil_uuid();
		boost::uuids::uuid uuid_datum_1 = boost::uuids::random_generator()();
		std::cout << "UUID Empty     : " << uuid_empty   << std::endl;
		std::cout << "UUID Datum 1   : " << uuid_datum_1 << std::endl;
		std::string uuid_datum_1_string;
		std::stringstream o_str;
		o_str << uuid_datum_1;
		uuid_datum_1_string = o_str.str();
		boost::uuids::uuid uuid_datum_2;
		o_str >> uuid_datum_2;
		std::cout << "UUID Datum 2   : " << uuid_datum_2 << std::endl;
	}
	catch (const std::exception &except) {
		throw std::runtime_error("The test of the boost::uuid library "
			"failed: " + std::string(except.what()));
	}

	std::cout << EmitterSep('=') << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
template <typename ContainerType>
	void TEST_CreateContainerOfUuid(std::size_t element_count = 5)
{
	using namespace MLB::Utility;

	{
		std::ostringstream o_str;
		o_str << typeid(ContainerType).name();
		std::string cont_name(o_str.str());
		std::string::size_type comma_pos = cont_name.find(',');
		if (comma_pos != std::string::npos)
			cont_name = cont_name.substr(0, comma_pos);
		std::cout << cont_name << ">:\n";
		std::cout << EmitterSep('-', cont_name.size() + 2);
	}

	element_count = (element_count > 0) ? element_count : 5;

	ContainerType dst(CreateUniqueIdContainer<ContainerType>(element_count));

	std::copy(dst.begin(), dst.end(),
		std::ostream_iterator<UniqueId>(std::cout, "\n"));
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_UniqueIdContainer()
{
	using namespace MLB::Utility;

	std::size_t element_count = 5;

	std::cout << EmitterSep('=');
	std::cout << "TEST_UniqueIdContainer()\n";
	std::cout << EmitterSep('-');
	TEST_CreateContainerOfUuid<std::deque<UniqueId>  >(element_count);
	std::cout << EmitterSep('-');
	TEST_CreateContainerOfUuid<std::list<UniqueId>   >(element_count);
	std::cout << EmitterSep('-');
	TEST_CreateContainerOfUuid<std::set<UniqueId>    >(element_count);
	std::cout << EmitterSep('-');
//	TEST_CreateContainerOfUuid<std::slist<UniqueId>  >(element_count);
//	std::cout << EmitterSep('-');
	TEST_CreateContainerOfUuid<std::vector<UniqueId> >(element_count);
	std::cout << EmitterSep('=') << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_UniqueId()
{
	using namespace MLB::Utility;

	TEST_UniqueIdCore();
	TEST_BoostUuid();
	TEST_UniqueIdContainer();
}
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
int main()
{
	int return_code = EXIT_SUCCESS;

	std::cout << "Test routine for class 'UniqueId'" << std::endl;
	std::cout << "---- ------- --- ----- ----------" << std::endl;

	try {
		TEST_UniqueId();
#if 0
		TEST_BoostUuid();
		UniqueId    request_id;
		UniqueId    request_id_tmp(false);
		std::cout << "---------------- --------------------------------------" <<
			std::endl << std::endl;
		std::cout << "UUID Mine      : " <<
			request_id << std::endl;
		std::cout << "UUID Converted : " <<
			UniqueId::FromString(request_id.ToString()) << std::endl;
		std::cout << "UUID Byte Order: " <<
			request_id.ToStringByteOrder() << std::endl;
/*
		std::cout << "UUID Radix 64  : " <<
			request_id.ToStringRadix64() << std::endl;
*/
		std::cout << "---------------- --------------------------------------" <<
			std::endl << std::endl;
#endif // #if 0
	}
	catch (const std::exception &except) {
		std::cout << "\n\nERROR: " << except.what() << std::endl;
		return_code = EXIT_FAILURE;
	}

	return(return_code);
}
// ////////////////////////////////////////////////////////////////////////////

#endif // #ifdef TEST_MAIN

