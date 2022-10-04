#pragma once

#include <gtest/gtest.h>

#include <reportportal/service.hpp>
#include <reportportal/launch.hpp>
#include <reportportal/test_item.hpp>

namespace reportportal
{
namespace gtest
{

class event_listener : public ::testing::TestEventListener
{
    public:
        event_listener(report_portal::service& service);

        // Fired before any test activity starts.
        void OnTestProgramStart(const ::testing::UnitTest& unit_test) override;

        // Fired before each iteration of tests starts.  There may be more than
        // one iteration if GTEST_FLAG(repeat) is set. iteration is the iteration
        // index, starting from 0.
        void OnTestIterationStart(const ::testing::UnitTest& unit_test, int iteration) override;

        // Fired before environment set-up for each iteration of tests starts.
        void OnEnvironmentsSetUpStart(const ::testing::UnitTest& unit_test) override;

        // Fired after environment set-up for each iteration of tests ends.
        void OnEnvironmentsSetUpEnd(const ::testing::UnitTest& unit_test) override;

        // Fired before the test suite starts.
        void OnTestSuiteStart(const ::testing::TestSuite& test_suite) override;

        // Fired before the test starts.
        void OnTestStart(const ::testing::TestInfo& test_info) override;

        // Fired after a failed assertion or a SUCCEED() invocation.
        // If you want to throw an exception from this function to skip to the next
        // TEST, it must be AssertionException defined above, or inherited from it.
        void OnTestPartResult(const ::testing::TestPartResult& test_part_result) override;

        // Fired after the test ends.
        void OnTestEnd(const ::testing::TestInfo& test_info) override;

        // Fired after the test suite ends.
        void OnTestSuiteEnd(const ::testing::TestSuite& test_suite) override;

        // Fired before environment tear-down for each iteration of tests starts.
        void OnEnvironmentsTearDownStart(const ::testing::UnitTest& unit_test) override;

        // Fired after environment tear-down for each iteration of tests ends.
        void OnEnvironmentsTearDownEnd(const ::testing::UnitTest& unit_test) override;

        // Fired after each iteration of tests finishes.
        void OnTestIterationEnd(const ::testing::UnitTest& unit_test, int iteration) override;

        // Fired after all test activities have ended.
        void OnTestProgramEnd(const ::testing::UnitTest& unit_test) override;

    private:
        report_portal::service& _service;
        std::unique_ptr<report_portal::launch> _launch;
        std::vector<std::unique_ptr<report_portal::test_item> > _test_item_stack;
};

}
}
