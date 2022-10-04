#include <gtest/gtest.h>
#include <reportportal/gtest/event_listener.hpp>

namespace reportportal
{
namespace gtest
{

static std::string char_to_string(const char* char_array)
{
    return char_array ? char_array : "";
}

event_listener::event_listener(report_portal::service& service)
  : _service(service)
{}

// Fired before any test activity starts.
void event_listener::OnTestProgramStart(const ::testing::UnitTest& unit_test) {
    _launch = std::make_unique<report_portal::launch>(_service, "Google Test Launch");
    _launch->set_description("This is a test launch for google tests.");

    _launch->start(std::chrono::high_resolution_clock::now());

    std::unique_ptr<report_portal::test_item> suite = std::make_unique<report_portal::test_item>(*_launch, "Google Test Suite");
    suite->start(std::chrono::high_resolution_clock::now());
    _test_item_stack.push_back(std::move(suite));
}

// Fired before each iteration of tests starts.  There may be more than
// one iteration if GTEST_FLAG(repeat) is set. iteration is the iteration
// index, starting from 0.
void event_listener::OnTestIterationStart(const ::testing::UnitTest& unit_test, int iteration) {
}

// Fired before environment set-up for each iteration of tests starts.
void event_listener::OnEnvironmentsSetUpStart(const ::testing::UnitTest& unit_test) {
}

// Fired after environment set-up for each iteration of tests ends.
void event_listener::OnEnvironmentsSetUpEnd(const ::testing::UnitTest& unit_test) {
}

// Fired before the test suite starts.
void event_listener::OnTestSuiteStart(const ::testing::TestSuite& test_suite) {
    std::unique_ptr<report_portal::test_item>& suite = _test_item_stack.back();

    const std::string name = char_to_string(test_suite.name());
    const std::string type_param = char_to_string(test_suite.type_param());

    std::unique_ptr<report_portal::test_item> test_suite_item = std::make_unique<report_portal::test_item>(*suite, name, report_portal::test_item_type::suite);
    test_suite_item->set_description("type_param = " + type_param);

    test_suite_item->start(std::chrono::high_resolution_clock::now());
    _test_item_stack.push_back(std::move(test_suite_item));
}

// Fired before the test starts.
void event_listener::OnTestStart(const ::testing::TestInfo& test_info) {
    std::unique_ptr<report_portal::test_item>& test_suite = _test_item_stack.back();

    const std::string name = char_to_string(test_info.name());
    const std::string type_param = char_to_string(test_info.type_param());
    const std::string value_param = char_to_string(test_info.value_param());
    const std::string file = char_to_string(test_info.file());
    const std::string line = std::to_string(test_info.line());

    std::unique_ptr<report_portal::test_item> test = std::make_unique<report_portal::test_item>(*test_suite, name, report_portal::test_item_type::step);
    test->set_description(
        "type_param = " + type_param + "\n"
        "value_param = " + value_param + "\n"
        "file = " + file + "\n"
        "line = " + line + "\n");

    test->start(std::chrono::high_resolution_clock::now());
    _test_item_stack.push_back(std::move(test));
}

// Fired after a failed assertion or a SUCCEED() invocation.
// If you want to throw an exception from this function to skip to the next
// TEST, it must be AssertionException defined above, or inherited from it.
void event_listener::OnTestPartResult(const ::testing::TestPartResult& test_part_result) {
}

// Fired after the test ends.
void event_listener::OnTestEnd(const ::testing::TestInfo& test_info) {
    std::unique_ptr<report_portal::test_item>& test = _test_item_stack.back();

    report_portal::test_item_status status = report_portal::test_item_status::skipped;
    const ::testing::TestResult* test_result = test_info.result();
    if (test_result) {
        if (test_result->Passed()) {
            status = report_portal::test_item_status::passed;
        } else if (test_result->Skipped()) {
            status = report_portal::test_item_status::skipped;
        } else {
            status = report_portal::test_item_status::failed;
        }

        for (int i = 0; i < test_result->total_part_count(); ++i) {
            const ::testing::TestPartResult& test_part_result = test_result->GetTestPartResult(i);

            const std::string file_name = char_to_string(test_part_result.file_name());
            const std::string line_number = std::to_string(test_part_result.line_number());
            const std::string summary = char_to_string(test_part_result.summary());
            const std::string message = char_to_string(test_part_result.message());

            const std::string detailed_log =
                "file = " + file_name + "\n"
                "line = " + line_number + "\n" + summary;
            test->log(std::chrono::high_resolution_clock::now(), report_portal::log_level::error, detailed_log);
        }
    }

    test->end(std::chrono::high_resolution_clock::now(), status);
    _test_item_stack.pop_back();
}

// Fired after the test suite ends.
void event_listener::OnTestSuiteEnd(const ::testing::TestSuite& test_suite) {
    std::unique_ptr<report_portal::test_item>& test_suite_item = _test_item_stack.back();

    test_suite_item->end(std::chrono::high_resolution_clock::now());
    _test_item_stack.pop_back();
}

// Fired before environment tear-down for each iteration of tests starts.
void event_listener::OnEnvironmentsTearDownStart(const ::testing::UnitTest& unit_test) {
}

// Fired after environment tear-down for each iteration of tests ends.
void event_listener::OnEnvironmentsTearDownEnd(const ::testing::UnitTest& unit_test) {
}

// Fired after each iteration of tests finishes.
void event_listener::OnTestIterationEnd(const ::testing::UnitTest& unit_test, int iteration) {
}

// Fired after all test activities have ended.
void event_listener::OnTestProgramEnd(const ::testing::UnitTest& unit_test) {
    std::unique_ptr<report_portal::test_item>& suite = _test_item_stack.back();
    suite->end(std::chrono::high_resolution_clock::now());
    _test_item_stack.pop_back();

    _launch->end(std::chrono::high_resolution_clock::now());
    _launch.reset();
}
}
}
