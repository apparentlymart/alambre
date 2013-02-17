
#include "gtest/gtest.h"
#include <gmock/gmock.h>
#include "mock/capability/2dgraphics.h"
#include "alambre/capability/2dgraphics.h"
#include <stdint.h>

using ::testing::InSequence;
using ::testing::Return;

TEST(TestGraphics2d, Bitmap2d) {

    Bitmap2d<uint8_t, uint32_t, 2, 2> bitmap;

    bitmap.set_pixel(0, 0, 1024);
    bitmap.set_pixel(0, 1, 65536);
    bitmap.set_pixel(1, 0, 0);
    bitmap.set_pixel(1, 1, 120);

    ASSERT_EQ(1024, bitmap.get_pixel(0, 0));
    ASSERT_EQ(65536, bitmap.get_pixel(0, 1));
    ASSERT_EQ(0, bitmap.get_pixel(1, 0)); // should default to 0
    ASSERT_EQ(120, bitmap.get_pixel(1, 1)); // should default to 0

}

TEST(TestGraphics2d, PaletteBitmap2d) {

    uint32_t palette[] = {
        0x00000000,
        0x00ffffff,
        0x000000ff,
        0x0000ff00,
        0x00ff0000,
        0x0000ffff,
        0x00ffff00,
        0x00ff00ff,
    };
    const unsigned int palette_size = sizeof(palette) / sizeof(uint32_t);
    PaletteBitmap2d<uint8_t, uint32_t, 2, 2, palette_size> bitmap(palette);
    typedef typeof(bitmap) bitmap_type;

    bitmap.set_pixel(0, 0, 0);
    bitmap.set_pixel(0, 1, 1);
    bitmap.set_pixel(1, 0, 5);
    bitmap.set_pixel(1, 1, 7);

    ASSERT_EQ(0, bitmap.get_pixel(0, 0));
    ASSERT_EQ(1, bitmap.get_pixel(0, 1));
    ASSERT_EQ(5, bitmap.get_pixel(1, 0));
    ASSERT_EQ(7, bitmap.get_pixel(1, 1));

    ASSERT_EQ(palette[0], bitmap.render_bitmap.get_pixel(0, 0));
    ASSERT_EQ(palette[1], bitmap.render_bitmap.get_pixel(0, 1));
    ASSERT_EQ(palette[5], bitmap.render_bitmap.get_pixel(1, 0));
    ASSERT_EQ(palette[7], bitmap.render_bitmap.get_pixel(1, 1));

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
