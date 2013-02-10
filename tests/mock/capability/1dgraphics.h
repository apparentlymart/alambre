#ifndef ALAMBRE_MOCK_1DGRAPHICS
#define ALAMBRE_MOCK_1DGRAPHICS

#include <gmock/gmock.h>
#include <alambre/capability/1dgraphics.h>

class MockBuffered1dGraphicsSurface : public IBuffered1dGraphicsSurface<unsigned int, unsigned char> {

 public:
    MOCK_METHOD2(set_pixel, void(unsigned int, unsigned char));
    MOCK_METHOD1(get_pixel, unsigned char(unsigned int));
    MOCK_METHOD0(flip, void());
    MOCK_METHOD0(get_length, unsigned int());
};

template <unsigned int LENGTH>
class FakeBuffered1dGraphicsSurface : public AbstractBuffered1dGraphicsSurface<LENGTH, unsigned char> {

  unsigned char front_buf[LENGTH];

  public:

    void flip() {
        for (unsigned int i = 0; i <= LENGTH; i++) {
            this->front_buf[i] = this->buf[i];
        }
    }

};

#endif
