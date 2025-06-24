// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// MLB Utility Library Module File
// ////////////////////////////////////////////////////////////////////////////
/*
   File Name         :  LogTestSupport.cpp

   File Description  :  Implementation of functions for shared log tests.

   Revision History  :  2005-01-02 --- Creation
                           Michael L. Brock
                        2023-02-25 --- Migration to C++ MlbDev2/Utility.
                           Michael L. Brock

      Copyright Michael L. Brock 2005 - 2023.
      Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
      http://www.boost.org/LICENSE_1_0.txt)

*/
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// Required include files...
// ////////////////////////////////////////////////////////////////////////////

#include <Logger/LogTestSupport.hpp>

#include <Logger/LogManager.hpp>
#include <Logger/LogHandlerFile.hpp>

#include <Utility/AnyToString.hpp>
#include <Utility/Sleep.hpp>
#include <Utility/StringPadLeft.hpp>
#include <Utility/ValueToStringRadix.hpp>

#ifdef _Windows
# pragma warning(push)
# pragma warning(disable:4217 4350 4625 4626 4668)
#endif // #ifdef _Windows

#include <boost/thread/thread.hpp> 

#ifdef _Windows
# pragma warning(pop)
#endif // #ifdef _Windows

#include <iomanip>
#include <iostream>

// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
LogManagerMacroDefinition(MB_LIB_LOCAL)
// ////////////////////////////////////////////////////////////////////////////

namespace MLB {

namespace Utility {

namespace {

// ////////////////////////////////////////////////////////////////////////////
const char         *MultiLineTestList[] = {
	"Line 1\nLine 2\n\nLine 4",
	"\nLine 2\n\n\nLine 5\n\n",
	"\n\nLine 3\n\n\nLine 6\n\n"
};
const unsigned int  MultiLineTestCount  =
	sizeof(MultiLineTestList) / sizeof(MultiLineTestList[0]);
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
std::string TEST_GetLogFileName(const std::string &base_name)
{
/*
YYYY-MM-DD hh:mm:ss.nnnnnnnnn 
012345678901234567890123456789 
          1         2 
*/
	std::string dt(TimeSpec::Now().ToString());
	std::string log_file_name("TEST_LOG." +
		std::string((base_name.empty()) ? "TestLogFile" : base_name) + "." +
		std::string(Logger_IMPL_CURRENT) + "." +
		std::string(Logger_VER_CURRENT) + "." +
		dt.substr( 0, 4) + dt.substr( 5, 2) + dt.substr( 8, 2) + "_" +
		dt.substr(11, 2) + dt.substr(14, 2) + dt.substr(17, 2) + "_" +
		dt.substr(20) + ".log");

	return(log_file_name);
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_MultiLineOperation()
{
	unsigned int count_1;

	std::cout << std::setfill('*') << std::setw(79) << "" <<
		std::setfill(' ') << std::endl;
	std::cout << std::setfill('*') << std::setw(79) << "" <<
		std::setfill(' ') << std::endl;

	for (count_1 = 0; count_1 < MultiLineTestCount; ++count_1) {
		std::string test_string(MultiLineTestList[count_1]);
		std::cout << std::setfill('=') << std::setw(79) << "" <<
			std::setfill(' ') << std::endl;
		std::string tmp_string(test_string);
		std::string::size_type eol_pos = 0;
		while ((eol_pos = tmp_string.find('\n', eol_pos)) != std::string::npos)
			tmp_string[eol_pos] = '|';
		std::cout << tmp_string << std::endl;
		std::cout << std::setfill('-') << std::setw(79) << "" <<
			std::setfill(' ') << std::endl;
		LogEmitControl emit_control(Default, LogFlag_Mask, LogFlag_Mask,
			TimeSpec(), LogLevel_Info, LogFlag_Info, test_string);
		emit_control.UpdateTime();
		unsigned int  line_length;
		const char   *line_ptr;
		while ((line_ptr = emit_control.GetNextLine(line_length)) != NULL) {
			std::cout << emit_control.GetLeaderPtr();
			std::cout.write(line_ptr, static_cast<std::streamsize>(line_length));
			std::cout << std::endl;
		}
		std::cout << std::setfill('=') << std::setw(79) << "" <<
			std::setfill(' ') << std::endl << std::endl;
	}
}
// ////////////////////////////////////////////////////////////////////////////

namespace {

// ////////////////////////////////////////////////////////////////////////////
static int     TEST_IsolationCharacter = 0;

static LogLock TEST_IsolationLock;
// ////////////////////////////////////////////////////////////////////////////

} // Anonymous namespace

// ////////////////////////////////////////////////////////////////////////////
void TEST_IsolationThreadProc()
{
	int this_value;

	{
		LogLockScoped my_lock(TEST_IsolationLock);
		this_value = TEST_IsolationCharacter++;
	}

	char this_char  = static_cast<char>(this_value) + 'A';

	LogInfo << "[" << std::setw(10) << CurrentThreadId() << "]";
	SleepMilliSecs(static_cast<unsigned long>(rand() % 100));
	LogInfo << "[" << this_char << "]";
	SleepMilliSecs(static_cast<unsigned long>(rand() % 100));
	LogInfo << TimeVal::Now();
	SleepMilliSecs(static_cast<unsigned long>(rand() % 100));
	LogInfo << "]";
	SleepMilliSecs(static_cast<unsigned long>(rand() % 100));
	LogInfo << "]: Character = '";
	SleepMilliSecs(static_cast<unsigned long>(rand() % 100));
	LogInfo << this_char;
	SleepMilliSecs(static_cast<unsigned long>(rand() % 100));
	LogInfo << "' (" << std::setw(2) << this_value << "): ";
	SleepMilliSecs(static_cast<unsigned long>(rand() % 100));
	LogInfo << std::setfill(this_char);
	SleepMilliSecs(static_cast<unsigned long>(rand() % 100));

	//	Delay threads managing the letter 'A' for an extended period...
	if (this_char == 'A')
		SleepMilliSecs(static_cast<unsigned long>(5000));

	LogInfo << std::setw(this_value);
	SleepMilliSecs(static_cast<unsigned long>(rand() % 100));
	LogInfo << "";
	LogInfo << std::setfill(' ');
	SleepMilliSecs(static_cast<unsigned long>(rand() % 100));
	LogInfo << "--->DONE";
	SleepMilliSecs(static_cast<unsigned long>(rand() % 100));
	LogInfo << std::endl;

	SleepMilliSecs(static_cast<unsigned long>(rand() % 1000));

	if (this_char == 'A') {
		LogInfo << "[" << std::setw(10) << CurrentThreadId() << "][" <<
			this_char << "] *** Character 'A' slept for an additional " <<
			5000 << " milliseconds ***\n";
	}
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
class TEST_IsolationThread : public boost::thread {
public:
	TEST_IsolationThread() :
		 boost::thread(TEST_IsolationThreadProc) {
	}

private:
	TEST_IsolationThread(const TEST_IsolationThread &) = delete;
	TEST_IsolationThread & operator = (const TEST_IsolationThread &) = delete;
};
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_Isolation()
{
	int                 count_1;
	boost::thread_group thr_group;

	for (count_1 = 0; count_1 < 26; ++count_1) {
//		thr_group.create_thread(&TEST_IsolationThreadProc);
		TEST_IsolationThread *this_thread = new TEST_IsolationThread();
		thr_group.add_thread(this_thread);
	}

	thr_group.join_all();
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_NoEOL()
{
	LogInfo << "Line without explicit 'std::endl'";
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_StressLines(std::size_t stress_count = 10000,
	std::size_t stress_length = 200)
{
	int         line_count  = (stress_count)  ? stress_count  : 10000;
	std::size_t line_length = (stress_length) ? stress_length :   200;
	std::string test_string(line_length, 'X');

	MLB::Utility::TimeSpec hr_timer;

	hr_timer.SetToNow();

	for (int count_1 = 0; count_1 < line_count; ++count_1)
		LogDetail << std::setw(10) << count_1 << ": " << test_string << std::endl;

	hr_timer = TimeSpec::Now().GetDifference(hr_timer);

	LogInfo << "Time required to write " << line_count << " log lines of "
		"length " << line_length << ": " << std::fixed << std::setw(16) <<
		std::setprecision(9) << hr_timer.GetDouble() <<
		" seconds." << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_StressSize(std::size_t stress_count = 10,
	std::size_t stress_length = 2000000)
{
	int         line_count  = (stress_count)  ? stress_count  :      10;
	std::size_t line_length = (stress_length) ? stress_length : 2000000;
	std::string test_string(line_length, 'Y');

	MLB::Utility::TimeSpec hr_timer;

	hr_timer.SetToNow();

	for (int count_1 = 0; count_1 < line_count; ++count_1)
		LogDetail << "REALLY BIG STRING: [" << test_string << "]" << std::endl;

	hr_timer = TimeSpec::Now().GetDifference(hr_timer);

	LogInfo << "Time required to write " << line_count << " log lines of "
		"length " << line_length << ": " << std::fixed << std::setw(16) <<
		std::setprecision(9) << hr_timer.GetDouble() <<
		" seconds." << std::endl;
}
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
void TEST_TestControl(LogHandlerPtr my_log_handler,
	std::size_t stress_count_1, std::size_t stress_length_1,
	std::size_t stress_count_2, std::size_t stress_length_2)
{
	try {
		TEST_MultiLineOperation();
		LogLevelPair old_level_console = MyLogManager.GetLogLevelConsole();
		LogLevelPair old_levels_file   = MyLogManager.GetLogLevelFile();
		MyLogManager.SetLogLevelConsoleAll();
		MyLogManager.SetLogLevelFileAll();
/*
		//	Create a LogHandlerFile...
		LogHandlerPtr my_log_handler(
			new LogHandlerXFile("TestLogXFile.VERSION_OLD_001.log"));
*/
		//	... And install the handler...
		MyLogManager.HandlerInstall(my_log_handler);
		//	Some simple tests...
LogLiteral << std::string("LITERAL #1: std::string(hello, world)") << std::endl;
LogLiteral << std::string("LITERAL #2: std::string(hello, world)") << std::endl;
LogInfo    << std::string("std::string(hello, world)") << std::endl;
		LogInfo << "hello, world" << std::endl;
		LogInfo << 666 << std::endl;
		LogInfo << "Last line" << std::endl;
		LogInfo << std::endl;
		//	Write 100 formatted INFO lines...
		unsigned int count_1 = 0;
		while (count_1 < 100)
			LogInfo << std::setw(8) << ++count_1 << " " << std::setfill('*') <<
				std::setw(10) << "" << std::setfill(' ') << 666 << "=0x" <<
				std::hex << count_1 << std::dec << std::endl;
		//	Write 100 formatted ERROR lines...
		count_1 = 0;
		while (count_1 < 100)
			LogError << std::setw(8) << ++count_1 << " " << std::setfill('*') <<
				std::setw(10) << "" << std::setfill(' ') << std::endl;
		//	Perform the threading test...
		TEST_Isolation();
		//	This last line doesn't provide line termination... But should be
		//	emitted at the time the thread stream is destroyed...
		TEST_NoEOL();
		//	Write some separator strings...
		LogInfo << ">>>>> Following is a call to LogSeparator():" <<
			std::endl;
		LogInfo.LogSeparator();
		LogInfo << ">>>>> Following is a call to LogSeparator('='):" <<
			std::endl;
		LogInfo.LogSeparator('=');
		LogInfo << ">>>>> Following is a call to LogSeparator('=', 10):" <<
			std::endl;
		LogInfo.LogSeparator('=', 10);
		//	Emit a line for each of the log severity levels...
		LogInfo << ">>>>> Following we use LogToLevel() for each of the log "
			"levels: " << std::endl;
		LogLevel min_level = LogLevel_Minimum;
		LogLevel max_level = LogLevel_Maximum;
		unsigned int level_flags = 0;
		while (min_level <= max_level) {
			level_flags = 1U << static_cast<unsigned int>(min_level);
			LogInfo.LogToLevel(min_level, "Logging to level " +
				PadLeft(AnyToString(min_level), 2) + " (flag equivalent = " +
				ValueToStringHex(level_flags) + ").");
			min_level    =
				static_cast<LogLevel>(static_cast<unsigned int>(min_level) + 1);
		}
		if (stress_count_1 || stress_count_2) {
			MyLogManager.SetLogLevelConsole(LogLevel::LogLevel_Info,
				LogLevel::LogLevel_Info);
			MyLogManager.SetLogLevelFile(LogLevel::LogLevel_Info,
				LogLevel::LogLevel_Info);
			if (stress_count_1)
				TEST_StressLines(stress_count_1, stress_length_1);
			if (stress_count_2)
				TEST_StressSize(stress_count_2, stress_length_2);
			MyLogManager.SetLogLevelConsole(old_level_console.first,
				old_level_console.second);
			MyLogManager.SetLogLevelFile(old_levels_file.first,
				old_levels_file.second);
		}
	}
	catch (const std::exception &except) {
		throw std::runtime_error("Failure in TEST_TestControl(): " +
			std::string(except.what()));
	}
}
// ////////////////////////////////////////////////////////////////////////////

} // namespace Utility

} // namespace MLB

