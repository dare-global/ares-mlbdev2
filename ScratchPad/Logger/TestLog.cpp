#include <Logger/LogManager.hpp>
#include <Logger/LogHandlerFile.hpp>

#include <Utility/DemangleCppName.hpp>

#include <iostream>

#define EMPTY_DEFINE

//LogManagerMacroDefinition(API_LOGGER)
//LogManagerMacroDefinition(API_UTILITY)
//LogManagerMacroDefinition(MB_LIB_LOCAL)
LogManagerMacroDefinition(MB_LIB_PUBLIC)

template <typename DatumType>
	std::string GetName(const DatumType &datum)
{
	return(MLB::Utility::DemangleCppName(typeid(datum).name()));
}

struct SomeClass
{
	SomeClass()
		:some_data_("Gotta get some data!")
	{
	}

	std::ostream &ToStream(std::ostream &o_str = std::cout) const
	{
//		std::cout << "SRC=" << &o_str << ": >>>";
		o_str << '[' << GetName(o_str) << "]: ADDR : " << &o_str << " some_data_=[" << some_data_ << "]\n";

		return(o_str);
	}

	std::string some_data_;
};

int main()
{
   using namespace MLB::Utility;

   int return_code = EXIT_SUCCESS; 

   try {
      // Create a LogHandlerFile...
      LogHandlerPtr my_log_handler(
         new LogHandlerXFile("Log_Test.SomeClass.001.log"));
		MyLogManager.HandlerInstall(my_log_handler);		
//      TEST_TestControl(my_log_handler, 0, 0, 0, 0);
		SomeClass some_class;
		LogInfo << '[' << GetName(LogInfo) << "]: START=" << &LogInfo << "\n";
		LogInfo << "[*PARTIAL-1*]";
		some_class.ToStream(LogInfo);
		some_class.ToStream();
		some_class.ToStream(std::cout);
		some_class.ToStream(LogInfo);
		LogInfo << '[' << GetName(LogInfo) << "]: END  =" << &LogInfo << "\n";
   }
   catch (const std::exception &except) {
      std::cerr << std::endl << std::endl << "ERROR: " << except.what() <<
         std::endl; 
      return_code = EXIT_FAILURE;
   }

   return(return_code);
}

