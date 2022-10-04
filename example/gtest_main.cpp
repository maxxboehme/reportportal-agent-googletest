#include <gtest/gtest.h>
#include <reportportal/gtest/event_listener.hpp>

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    report_portal::service service("http://web.demo.reportportal.io", "DEFAULT_PERSONAL", "default", "1q2w3e");
    listeners.Append(new reportportal::gtest::event_listener(service));
    return RUN_ALL_TESTS();
}
