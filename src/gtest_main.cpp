#include <gtest/gtest.h>

#include <string>
#include <iostream>
#include <chrono>
#include <ctime>
#include <reportportal/gtest/event_listener.hpp>

namespace test {
static const std::string indentation = "  ";

// Use this if the TimeInMillis represents a time from the system epoch time
std::chrono::system_clock::time_point to_time_point(::testing::TimeInMillis gtime)
{
    // TimeInMillis is the time since the system epoch
    std::chrono::milliseconds dur(gtime);

    return std::chrono::system_clock::time_point(dur);
}

// Use this if the TimeInMillis represents a different between to points of time
std::chrono::milliseconds to_duration(::testing::TimeInMillis gtime)
{
    return std::chrono::milliseconds(gtime);
}

static std::string char_to_string(const char* char_array)
{
    return char_array ? char_array : "";
}

static std::string to_string(bool value)
{
    return value ? "true" : "false";
}

static std::string to_string(::testing::TimeInMillis gtime)
{
    const std::time_t time = std::chrono::system_clock::to_time_t(to_time_point(gtime));
    return char_to_string(std::ctime(&time));
}

static std::string to_string(::testing::TestPartResult::Type type)
{
    switch (type) {
        case ::testing::TestPartResult::kSuccess:
            return "Success";
        case ::testing::TestPartResult::kNonFatalFailure:
            return "NonFatalFailure";
        case ::testing::TestPartResult::kFatalFailure:
            return "kFatalFailure";
        case ::testing::TestPartResult::kSkip:
            return "kSkip";
        default:
            throw std::invalid_argument("Got unknown ::testing::TestPartResult::Type value");
            return "";
    }
}

#define APPEND_MEMBER(buffer, level,  VAR) \
    buffer += level + #VAR + ": \"" + VAR + "\"\n"

#define APPEND_MEMBER_NEWLINE(buffer, level,  VAR) \
    buffer += level + #VAR + ":\n" + VAR + "\n"

static std::string to_string(const ::testing::TestProperty& test_property, const std::string& leading_spaces = "")
{
    const std::string key = char_to_string(test_property.key());
    const std::string value = char_to_string(test_property.value());

    const std::string level = leading_spaces + indentation;
    std::string result = leading_spaces + "[TestProperty]\n";
    APPEND_MEMBER(result, level, key);
    APPEND_MEMBER(result, level, value);

    return result;
}

static std::string to_string(const ::testing::TestPartResult& test_part_result, const std::string& leading_spaces = "")
{
    const std::string type = to_string(test_part_result.type());
    const std::string file_name = char_to_string(test_part_result.file_name());
    const std::string line_number = std::to_string(test_part_result.line_number());
    const std::string summary = char_to_string(test_part_result.summary());
    const std::string message = char_to_string(test_part_result.message());

    const std::string skipped = to_string(test_part_result.skipped());
    const std::string passed = to_string(test_part_result.passed());
    const std::string nonfatally_failed = to_string(test_part_result.nonfatally_failed());
    const std::string fatally_failed = to_string(test_part_result.fatally_failed());
    const std::string failed = to_string(test_part_result.failed());

    const std::string level = leading_spaces + indentation;
    std::string result = leading_spaces + "[TestPartResult]\n";
    APPEND_MEMBER(result, level, type);
    APPEND_MEMBER(result, level, file_name);
    APPEND_MEMBER(result, level, line_number);
    APPEND_MEMBER(result, level, summary);
    APPEND_MEMBER(result, level, message);

    APPEND_MEMBER(result, level, skipped);
    APPEND_MEMBER(result, level, passed);
    APPEND_MEMBER(result, level, nonfatally_failed);
    APPEND_MEMBER(result, level, fatally_failed);
    APPEND_MEMBER(result, level, failed);

    return result;
}

static std::string to_string(const ::testing::TestResult& test_result, const std::string& leading_spaces = "")
{
    const std::string total_part_count = std::to_string(test_result.total_part_count());
    const std::string test_property_count = std::to_string(test_result.test_property_count());

    const std::string passed = to_string(test_result.Passed());
    const std::string failed = to_string(test_result.Failed());
    const std::string has_fatal_failure = to_string(test_result.HasFatalFailure());
    const std::string has_nonfatal_failure = to_string(test_result.HasNonfatalFailure());

    const ::testing::TimeInMillis start_timestamp = test_result.start_timestamp();
    const ::testing::TimeInMillis elapsed_time_in_millis = test_result.elapsed_time();

    const std::string start_time = to_string(start_timestamp);
    const std::string elapsed_time = std::to_string(to_duration(elapsed_time_in_millis).count()) + " ms";
    const std::string end_time = to_string(start_timestamp + elapsed_time_in_millis);

    const std::string level = leading_spaces + indentation;
    std::string result = leading_spaces + "[TestResult]\n";
    APPEND_MEMBER(result, level, total_part_count);
    APPEND_MEMBER(result, level, test_property_count);

    APPEND_MEMBER(result, level, passed);
    APPEND_MEMBER(result, level, failed);
    APPEND_MEMBER(result, level, has_fatal_failure);
    APPEND_MEMBER(result, level, has_nonfatal_failure);

    APPEND_MEMBER(result, level, start_time);
    APPEND_MEMBER(result, level, end_time);
    APPEND_MEMBER(result, level, elapsed_time);

    result += level + "part_results:\n";
    for (int i = 0; i < test_result.total_part_count(); ++i) {
        result += to_string(test_result.GetTestPartResult(i), level + indentation);
    }

    result += level + "properties:\n";
    for (int i = 0; i < test_result.test_property_count(); ++i) {
        result += to_string(test_result.GetTestProperty(i), level + indentation);
    }

    return result;
}

static std::string to_string(const ::testing::TestInfo& test_info, const std::string& leading_spaces = "")
{
    const std::string level = leading_spaces + indentation;

    const std::string test_suite_name = char_to_string(test_info.test_suite_name());
    const std::string name = char_to_string(test_info.name());
    const std::string type_param = char_to_string(test_info.type_param());
    const std::string value_param = char_to_string(test_info.value_param());
    const std::string file = char_to_string(test_info.file());
    const std::string line = std::to_string(test_info.line());

    const std::string is_in_another_shard = to_string(test_info.is_in_another_shard());
    const std::string should_run = to_string(test_info.should_run());
    const std::string is_reportable = to_string(test_info.is_reportable());
    const std::string test_result = test_info.result() ? to_string(*test_info.result(), level) : "";

            // const ::testing::TestProperty& GetTestProperty(int i)

    std::string result = leading_spaces + "[TestInfo]\n";
    APPEND_MEMBER(result, level, test_suite_name);
    APPEND_MEMBER(result, level, name);
    APPEND_MEMBER(result, level, type_param);
    APPEND_MEMBER(result, level, value_param);

    APPEND_MEMBER(result, level, file);
    APPEND_MEMBER(result, level, line);
    APPEND_MEMBER(result, level, is_in_another_shard);
    APPEND_MEMBER(result, level, should_run);
    APPEND_MEMBER(result, level, is_reportable);
    APPEND_MEMBER_NEWLINE(result, level, test_result);

    return result;
}

static std::string to_string(const ::testing::TestSuite& test_suite, const std::string& leading_spaces = "")
{
    const std::string name = char_to_string(test_suite.name());
    const std::string type_param = char_to_string(test_suite.type_param());

    const std::string should_run = to_string(test_suite.should_run());
    const std::string successful_test_count = std::to_string(test_suite.successful_test_count());
    const std::string skipped_test_count = std::to_string(test_suite.skipped_test_count());
    const std::string failed_test_count = std::to_string(test_suite.failed_test_count());
    const std::string reportable_disabled_test_count = std::to_string(test_suite.reportable_disabled_test_count());
    const std::string disabled_test_count = std::to_string(test_suite.disabled_test_count());
    const std::string reportable_test_count = std::to_string(test_suite.reportable_test_count());
    const std::string test_to_run_count = std::to_string(test_suite.test_to_run_count());
    const std::string total_test_count = std::to_string(test_suite.total_test_count());

    const std::string passed = to_string(test_suite.Passed());
    const std::string failed = to_string(test_suite.Failed());

    const ::testing::TimeInMillis start_timestamp = test_suite.start_timestamp();
    const ::testing::TimeInMillis elapsed_time_in_millis = test_suite.elapsed_time();

    const std::string start_time = to_string(start_timestamp);
    const std::string elapsed_time = std::to_string(to_duration(elapsed_time_in_millis).count()) + " ms";
    const std::string end_time = to_string(start_timestamp + elapsed_time_in_millis);

    // const ::testing::TestInfo* GetTestInfo(int i)
    // const TestResult& ad_hoc_test_result()

    const std::string level = leading_spaces + indentation;
    std::string result = leading_spaces + "[TestSuite]\n";
    APPEND_MEMBER(result, level, name);
    APPEND_MEMBER(result, level, type_param);
    APPEND_MEMBER(result, level, should_run);
    APPEND_MEMBER(result, level, successful_test_count);

    APPEND_MEMBER(result, level, skipped_test_count);
    APPEND_MEMBER(result, level, failed_test_count);
    APPEND_MEMBER(result, level, reportable_disabled_test_count);
    APPEND_MEMBER(result, level, disabled_test_count);
    APPEND_MEMBER(result, level, reportable_test_count);
    APPEND_MEMBER(result, level, test_to_run_count);
    APPEND_MEMBER(result, level, total_test_count);

    APPEND_MEMBER(result, level, passed);
    APPEND_MEMBER(result, level, failed);

    APPEND_MEMBER(result, level, start_time);
    APPEND_MEMBER(result, level, end_time);
    APPEND_MEMBER(result, level, elapsed_time);

    return result;
}

static std::string to_string(const ::testing::UnitTest& unit_test, const std::string& leading_spaces)
{
    const std::string original_working_dir = char_to_string(unit_test.original_working_dir());
    // const ::testing::TestSuite* current_test_suite()
    // const ::testing::TestInfo* current_test_info()
    const std::string random_seed = std::to_string(unit_test.random_seed());
    const std::string successful_test_suite_count = std::to_string(unit_test.successful_test_suite_count());
    const std::string failed_test_suite_count = std::to_string(unit_test.failed_test_suite_count());
    const std::string total_test_suite_count = std::to_string(unit_test.total_test_suite_count());
    const std::string test_suite_to_run_count = std::to_string(unit_test.test_suite_to_run_count());

    const std::string successful_test_count = std::to_string(unit_test.successful_test_count());
    const std::string skipped_test_count = std::to_string(unit_test.skipped_test_count());
    const std::string failed_test_count = std::to_string(unit_test.failed_test_count());
    const std::string reportable_disabled_test_count = std::to_string(unit_test.reportable_disabled_test_count());
    const std::string disabled_test_count = std::to_string(unit_test.disabled_test_count());
    const std::string reportable_test_count = std::to_string(unit_test.reportable_test_count());
    const std::string total_test_count = std::to_string(unit_test.total_test_count());
    const std::string test_to_run_count = std::to_string(unit_test.test_to_run_count());

    const ::testing::TimeInMillis start_timestamp = unit_test.start_timestamp();
    const ::testing::TimeInMillis elapsed_time_in_millis = unit_test.elapsed_time();

    const std::string start_time = to_string(start_timestamp);
    const std::string elapsed_time = std::to_string(to_duration(elapsed_time_in_millis).count()) + " ms";
    const std::string end_time = to_string(start_timestamp + elapsed_time_in_millis);

    const std::string passed = to_string(unit_test.Passed());
    const std::string failed = to_string(unit_test.Failed());
    // const ::testing::TestSuite* GetTestSuite(int i)
    // const TestResult& ad_hoc_test_result()
    // TestEventListeners& listeners()

    const std::string level = leading_spaces + indentation;
    std::string result = leading_spaces + "[UnitTest]\n";
    APPEND_MEMBER(result, level, original_working_dir);
    APPEND_MEMBER(result, level, random_seed);
    APPEND_MEMBER(result, level, successful_test_suite_count);
    APPEND_MEMBER(result, level, failed_test_suite_count);
    APPEND_MEMBER(result, level, total_test_suite_count);
    APPEND_MEMBER(result, level, test_suite_to_run_count);

    APPEND_MEMBER(result, level, successful_test_count);
    APPEND_MEMBER(result, level, skipped_test_count);

    APPEND_MEMBER(result, level, skipped_test_count);
    APPEND_MEMBER(result, level, failed_test_count);
    APPEND_MEMBER(result, level, reportable_disabled_test_count);
    APPEND_MEMBER(result, level, disabled_test_count);
    APPEND_MEMBER(result, level, reportable_test_count);
    APPEND_MEMBER(result, level, total_test_count);
    APPEND_MEMBER(result, level, test_to_run_count);

    APPEND_MEMBER(result, level, passed);
    APPEND_MEMBER(result, level, failed);

    APPEND_MEMBER(result, level, start_time);
    APPEND_MEMBER(result, level, end_time);
    APPEND_MEMBER(result, level, elapsed_time);

    return result;
}

class test_listener : public ::testing::TestEventListener
{
    public:
        // Fired before any test activity starts.
        virtual void OnTestProgramStart(const ::testing::UnitTest& unit_test)
        {
            // Gets the number of all tests.
            // int unit_test.total_test_count()
            // Gets the number of tests that should run.
            // int unit_test.total_to_run_count()
            // Gets the time of the test program, in ms from the start of the UNIX epoch.
            // ::testing::TimeInMillis unit_test.start_timestamp()

            const std::string level = indentation;
            std::cout << level << "OnTestProgramStart" << std::endl;
            std::cout << to_string(unit_test, level) << std::endl;
        }

        // Fired before each iteration of tests starts.  There may be more than
        // one iteration if GTEST_FLAG(repeat) is set. iteration is the iteration
        // index, starting from 0.
        virtual void OnTestIterationStart(const ::testing::UnitTest& unit_test, int iteration) {
            // If test is a repeat test should go here.
            //
            // if (GTEST_FLAG(shuffle)) {
            //     // Note: Randomizing tests' orders with a seed of %d."
            //     unit_test.random_seed();
            // }

            const std::string level = indentation;
            std::cout << level << "OnTestIterationStart: iteration = " << iteration << std::endl;
            std::cout << to_string(unit_test, level) << std::endl;
        }

        // Fired before environment set-up for each iteration of tests starts.
        virtual void OnEnvironmentsSetUpStart(const ::testing::UnitTest& unit_test)
        {
            const std::string level = indentation + indentation;
            std::cout << level << "OnEnvironmentsSetUpStart" << std::endl;
            std::cout << to_string(unit_test, level) << std::endl;
        }

        // Fired after environment set-up for each iteration of tests ends.
        virtual void OnEnvironmentsSetUpEnd(const ::testing::UnitTest& unit_test)
        {
            const std::string level = indentation + indentation;
            std::cout << level << "OnEnvironmentsSetUpEnd" << std::endl;
            std::cout << to_string(unit_test, level) << std::endl;
        }

        // Fired before the test suite starts.
        virtual void OnTestSuiteStart(const ::testing::TestSuite& test_suite) {
            // Gets the name of the ::testing::TestSuite.
            // const char* name()
            // Returns the name of the parameter type, or NULL if this is not a
            // type-parameterized test suite.
            // const char* type_param()
            // Returns true if any test in this test suite should run.
            // bool should_run()
            // Gets the time of the test suite start
            // ::testing::TimeInMillis start_timestamp()

            if (test_suite.type_param()) {
                // printf(", where %s = %s", kTypeParamLabel, test_suite.type_aram())
            }


            const std::string level = indentation + indentation + indentation;
            std::cout << level << "OnTestSuiteStart" << std::endl;
            std::cout << to_string(test_suite, level) << std::endl;
        }

        // Fired before the test starts.
        virtual void OnTestStart(const ::testing::TestInfo& test_info)
        {
            // Returns the test name
            // const char* name()
            // Returns the name of the parameter type
            // const char* type_param()
            // Returns the test representation of the value parameter
            // const char* value_param()
            // Returns the file name where this test is defined
            // const char* file()
            // Returns the line where this test is defined.
            // int line()
            // const TestResult* result()

            // TestResult
            // Gets the time of the test case start
            // ::testing::TimeInMillis start_timestamd()

            const std::string level = indentation + indentation + indentation + indentation;
            std::cout << level << "OnTestStart" << std::endl;
            std::cout << to_string(test_info, level) << std::endl;
        }

        // Fired after a failed assertion or a SUCCEED() invocation.
        // If you want to throw an exception from this function to skip to the next
        // TEST, it must be AssertionException defined above, or inherited from it.
        virtual void OnTestPartResult(const ::testing::TestPartResult& test_part_result)
        {
        }

        // Fired after the test ends.
        virtual void OnTestEnd(const ::testing::TestInfo& test_info)
        {
            // Returns the result of the test
            // const TestResult* result()

            // TestResult
            // Returns true if and only if the test passed (i.e. no test part failed)
            // bool Passed()
            // Returns true if the test was skipped
            // bool Skipped()
            // Returns true if the test failed
            // bool Failed()
            // Returns the elapsed time, in milliseconds
            // ::testing::TimeInMillis elapsed_time()
            // Gets the number of all test parts. This is the sum of the number of successful
            // test parts and the number of failed test parts.
            // int total_part_count()
            // Returns the number of the test properties
            // test_property_count()
            // Returns the i-th test part result among all the results. i can rage from 0
            // to total_part_count() - 1. If i is not in that range, aborts
            // const ::testing::TestPartResult& GetTestPartResult(int i)
            // Returns the i-th test property.
            // const ::testing::TestProperty& GetTestProperty(int i)

            // ::testing::TestPartResult
            // Gets the outcome of the test part
            // Type type()
            // Gets the name of the source file where the test part took place, NULL if it's unknown.
            // const char* file_name()
            // Gets the line in the source file where the test part took place, or -1 if it's unknown.
            // int line_number()
            // Gets the summary of the failure message.
            // const char* summary()
            // Gets the message associated with the test part
            // const char* message
            // Returns true if test skipped
            // bool skipped()
            // Returns ture if and only if the test part passed.
            // bool passed()
            // Returns true if and only if the test part failed.
            // bool failed()

            // ::testing::TestProperty "RecordProperty(key, value) to log additional information
            // Gets the user supplied key
            // const char* key()
            // Gets the user supplied value
            // const char* value()


//            if (test_info.result()->Passed()) {
//            } else if (test_info.result()->Skipped()) {
//            } else {
//            }
//
//            if (test_info.result()->Failed()) {
//                PrintFullTestCommentIfPresent(test_info);
//            }
//
//            internal::StreamableToString(test_info.result()->elapsed_time())

            const std::string level = indentation + indentation + indentation + indentation;
            std::cout << level << "OnTestEnd" << std::endl;
            std::cout << to_string(test_info, level) << std::endl;
        }

        // Fired after the test suite ends.
        virtual void OnTestSuiteEnd(const ::testing::TestSuite& test_suite)
        {
            // Gets the elapsed time, in milliseconds.
            // ::testing::TimeInMillis elapsed_time()
            //

            const std::string level = indentation + indentation + indentation;
            std::cout << level << "OnTestSuiteEnd" << std::endl;
            std::cout << to_string(test_suite, level) << std::endl;
        }

        // Fired before environment tear-down for each iteration of tests starts.
        virtual void OnEnvironmentsTearDownStart(const ::testing::UnitTest& unit_test)
        {
            const std::string level = indentation + indentation;
            std::cout << level << "OnEnvironmentsTearDownStart" << std::endl;
            std::cout << to_string(unit_test, level) << std::endl;
        }

        // Fired after environment tear-down for each iteration of tests ends.
        virtual void OnEnvironmentsTearDownEnd(const ::testing::UnitTest& unit_test)
        {
            const std::string level = indentation + indentation;
            std::cout << level << "OnEnvironmentsTearDownEnd" << std::endl;
            std::cout << to_string(unit_test, level) << std::endl;
        }

        // Fired after each iteration of tests finishes.
        virtual void OnTestIterationEnd(const ::testing::UnitTest& unit_test, int iteration)
        {

            const std::string level = indentation;
            std::cout << level << "OnTestIterationEnd: iteration = " << iteration << std::endl;
            std::cout << to_string(unit_test, level) << std::endl;
        }

        // Fired after all test activities have ended.
        virtual void OnTestProgramEnd(const ::testing::UnitTest& unit_test) {
            // Gets the elapsed time, in milliseconds.
            // ::testing::TimeInMillis unit_test.elapsed_time()

            const std::string level = "";
            std::cout << level << "OnTestProgramEnd" << std::endl;
            std::cout << to_string(unit_test, level) << std::endl;
        }
};

}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    // delete listeners.Release(listeners.default_result_printer());
    report_portal::service service("http://web.demo.reportportal.io", "DEFAULT_PERSONAL", "default", "1q2w3e");
    listeners.Append(new reportportal::gtest::event_listener(service));
    return RUN_ALL_TESTS();
}
