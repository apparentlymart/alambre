#ifndef ALAMBRE_MOCK_1DGRAPHICS
#define ALAMBRE_MOCK_1DGRAPHICS

#include <gmock/gmock.h>
#include <alambre/capability/1dgraphics.h>

class MockBitmap1d : public IMutableBitmap1d<unsigned int, unsigned char> {

 public:
    MOCK_METHOD2(set_pixel, void(unsigned int, unsigned char));
    MOCK_METHOD1(get_pixel, unsigned char(unsigned int));
    MOCK_METHOD0(get_length, unsigned int());
};

#endif
