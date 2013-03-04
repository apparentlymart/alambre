
#include "alambre/debugstream.h"
#include "gtest/gtest.h"
#include <string.h>

char latest_log_line[256];

ENABLE_DEBUG_STREAM {
    latest_log_line[255] = '\0';
    strncpy(latest_log_line, msg, 255);
}

TEST(TestDebugStream, Types) {

    debug_stream << "hello";
    ASSERT_STREQ("hello", latest_log_line);

    debug_stream << (int)5;
    ASSERT_STREQ("5", latest_log_line);

    debug_stream << (int)-5;
    ASSERT_STREQ("-5", latest_log_line);

    debug_stream << (unsigned int)5;
    ASSERT_STREQ("5", latest_log_line);

    debug_stream << (long)5;
    ASSERT_STREQ("5L", latest_log_line);

    debug_stream << (unsigned long)5;
    ASSERT_STREQ("5L", latest_log_line);

    debug_stream << (short)5;
    ASSERT_STREQ("5", latest_log_line);

    debug_stream << (unsigned short)5;
    ASSERT_STREQ("5", latest_log_line);

    debug_stream << (float)1.5;
    ASSERT_STREQ("1.5", latest_log_line);

    debug_stream << (double)1.5;
    ASSERT_STREQ("1.5", latest_log_line);

    debug_stream << 'a';
    ASSERT_STREQ("a", latest_log_line);

    debug_stream << true;
    ASSERT_STREQ("true", latest_log_line);

    debug_stream << false;
    ASSERT_STREQ("false", latest_log_line);

    // Can't do this right now because implementing this overload
    // makes GCC <= 4.7.0 crash during compile.
    /*debug_stream << nullptr;
    ASSERT_STREQ("nullptr", latest_log_line);*/

}

