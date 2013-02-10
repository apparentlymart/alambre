
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
