
#include "gtest/gtest.h"
#include <gmock/gmock.h>
#include "mock/capability/1dgraphics.h"
#include "alambre/capability/1dgraphics.h"

using ::testing::InSequence;
using ::testing::Return;

TEST(TestGraphics1d, Init) {

    // Compile-time test to make sure we can build our dummy
    // implementation, which in turn requires that we can build
    // the interface class itself.
    // If this module compiles then this test should always pass.

    FakeBuffered1dGraphicsSurface<32> surface;

    // Do something to the object just so the compiler won't optimize it away.
    surface.set_pixel(0, 1);

}

TEST(TestGraphics1d, AdapterFrom2d) {
    InSequence dummy;

    MockBuffered1dGraphicsSurface surface_1d;

    // Call to 2d.get_height in turn calls 1d.get_length.
    // We'll make the 1D surface 13 pixels long and make the
    // 2D surface 3 pixels wide, which should result in a 3x4 2D surface
    // because we round down to the nearest full row.
    EXPECT_CALL(surface_1d, get_length()).WillOnce(Return(13));

    // Setting pixels
    EXPECT_CALL(surface_1d, set_pixel(2, 1));
    EXPECT_CALL(surface_1d, set_pixel(4, 2));
    EXPECT_CALL(surface_1d, set_pixel(6, 3));
    EXPECT_CALL(surface_1d, set_pixel(11, 4));

    // Getting pixels
    EXPECT_CALL(surface_1d, get_pixel(0)).WillOnce(Return(0));
    EXPECT_CALL(surface_1d, get_pixel(2)).WillOnce(Return(1));
    EXPECT_CALL(surface_1d, get_pixel(3)).WillOnce(Return(2));
    EXPECT_CALL(surface_1d, get_pixel(11)).WillOnce(Return(4));

    RowSlicedBuffered2dTo1dGraphicsSurfaceAdapter<typeof(surface_1d), 3> surface_2d(&surface_1d);

    ASSERT_EQ(3, surface_2d.get_width());
    ASSERT_EQ(4, surface_2d.get_height());

    surface_2d.set_pixel(2, 0, 1);
    surface_2d.set_pixel(1, 1, 2);
    surface_2d.set_pixel(0, 2, 3);
    surface_2d.set_pixel(2, 3, 4);

    ASSERT_EQ(0, surface_2d.get_pixel(0, 0));
    ASSERT_EQ(1, surface_2d.get_pixel(2, 0));
    ASSERT_EQ(2, surface_2d.get_pixel(0, 1));
    ASSERT_EQ(4, surface_2d.get_pixel(2, 3));

}
