
#include "gtest/gtest.h"
#include <gmock/gmock.h>
#include "mock/capability/1dgraphics.h"
#include "alambre/capability/1dgraphics.h"

using ::testing::InSequence;
using ::testing::Return;

TEST(TestGraphics1d, Bitmap1d) {

    Bitmap1d<unsigned char, unsigned int, 4> bitmap;

    bitmap.set_pixel(0, 1);
    bitmap.set_pixel(1, 0);
    bitmap.set_pixel(2, 3);
    bitmap.set_pixel(3, 256);

    ASSERT_EQ(1, bitmap.get_pixel(0));
    ASSERT_EQ(0, bitmap.get_pixel(1));
    ASSERT_EQ(3, bitmap.get_pixel(2));
    ASSERT_EQ(256, bitmap.get_pixel(3));

}

TEST(TestGraphics1d, PaletteBitmap1d) {

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
    PaletteBitmap1d<uint8_t, uint32_t, 4, palette_size> bitmap(palette);
    typedef typeof(bitmap) bitmap_type;

    bitmap.set_pixel(0, 0);
    bitmap.set_pixel(1, 1);
    bitmap.set_pixel(2, 5);
    bitmap.set_pixel(3, 7);

    ASSERT_EQ(0, bitmap.get_pixel(0));
    ASSERT_EQ(1, bitmap.get_pixel(1));
    ASSERT_EQ(5, bitmap.get_pixel(2));
    ASSERT_EQ(7, bitmap.get_pixel(3));

    ASSERT_EQ(palette[0], bitmap.render_bitmap.get_pixel(0));
    ASSERT_EQ(palette[1], bitmap.render_bitmap.get_pixel(1));
    ASSERT_EQ(palette[5], bitmap.render_bitmap.get_pixel(2));
    ASSERT_EQ(palette[7], bitmap.render_bitmap.get_pixel(3));

}

TEST(TestGraphics1d, AdapterAs2d) {
    InSequence dummy;

    MockBitmap1d bitmap1d;

    // Call to 2d.get_height in turn calls 1d.get_length.
    // We'll make the 1D surface 13 pixels long and make the
    // 2D surface 3 pixels wide, which should result in a 3x4 2D surface
    // because we round down to the nearest full row.
    EXPECT_CALL(bitmap1d, get_length()).WillOnce(Return(13));

    // Setting pixels
    EXPECT_CALL(bitmap1d, set_pixel(2, 1));
    EXPECT_CALL(bitmap1d, set_pixel(4, 2));
    EXPECT_CALL(bitmap1d, set_pixel(6, 3));
    EXPECT_CALL(bitmap1d, set_pixel(11, 4));

    // Getting pixels
    EXPECT_CALL(bitmap1d, get_pixel(0)).WillOnce(Return(0));
    EXPECT_CALL(bitmap1d, get_pixel(2)).WillOnce(Return(1));
    EXPECT_CALL(bitmap1d, get_pixel(3)).WillOnce(Return(2));
    EXPECT_CALL(bitmap1d, get_pixel(11)).WillOnce(Return(4));

    RowSlicedMutableBitmap1dAsBitmap2dAdapter<typeof(bitmap1d), 3> bitmap2d(&bitmap1d);

    ASSERT_EQ(3, bitmap2d.get_width());
    ASSERT_EQ(4, bitmap2d.get_height());

    bitmap2d.set_pixel(2, 0, 1);
    bitmap2d.set_pixel(1, 1, 2);
    bitmap2d.set_pixel(0, 2, 3);
    bitmap2d.set_pixel(2, 3, 4);

    ASSERT_EQ(0, bitmap2d.get_pixel(0, 0));
    ASSERT_EQ(1, bitmap2d.get_pixel(2, 0));
    ASSERT_EQ(2, bitmap2d.get_pixel(0, 1));
    ASSERT_EQ(4, bitmap2d.get_pixel(2, 3));

}
