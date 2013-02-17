
#include "gtest/gtest.h"
#include <gmock/gmock.h>
#include "mock/capability/2dgraphics.h"
#include "alambre/capability/2dgraphics.h"

using ::testing::InSequence;
using ::testing::Return;

TEST(TestGraphics2d, Init) {

    // Compile-time test to make sure we can wire up the Graphics2d
    // template class to our dummy surface implementation.
    // If this module compiles then this test should always pass.

    FakeBuffered2dGraphicsSurface<32,16> surface;
    Graphics2d<typeof(surface)> graphics(&surface);

    // Do something to the object just so the compiler won't optimize it away.
    graphics.set_pixel(0, 0, 1);

}

#ifdef DISABLEDFORNOW
TEST(TestGraphics2d, PixelsAndFlip) {
    InSequence dummy;

    MockBuffered2dGraphicsSurface surface;

    EXPECT_CALL(surface, get_width()).WillOnce(Return(20));
    EXPECT_CALL(surface, get_height()).WillOnce(Return(10));

    EXPECT_CALL(surface, set_pixel(1, 2, '\x3'));
    EXPECT_CALL(surface, flip(1, 2, 1, '\x2'));
    EXPECT_CALL(surface, get_width()).WillOnce(Return(20));
    EXPECT_CALL(surface, get_height()).WillOnce(Return(10));

    EXPECT_CALL(surface, set_pixel(2, 2, '\x5'));
    EXPECT_CALL(surface, set_pixel(2, 3, '\x5'));
    EXPECT_CALL(surface, flip(2, 2, 2, 3));
    EXPECT_CALL(surface, get_width()).WillOnce(Return(20));
    EXPECT_CALL(surface, get_height()).WillOnce(Return(10));

    Graphics2d<typeof(surface)> graphics(&surface);
    graphics.set_pixel(1, 2, 3);
    graphics.flip();

    graphics.set_pixel(2, 2, 5);
    graphics.set_pixel(2, 3, 5);
    graphics.flip();

}
#endif
