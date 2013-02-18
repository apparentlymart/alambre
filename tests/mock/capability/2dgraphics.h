#ifndef ALAMBRE_MOCK_2DGRAPHICS
#define ALAMBRE_MOCK_2DGRAPHICS

#include <gmock/gmock.h>
#include <alambre/capability/2dgraphics.h>

/*class MockBuffered2dGraphicsSurface : public IBuffered2dGraphicsSurface<unsigned int, unsigned char> {

 public:
    MOCK_METHOD3(set_pixel, void(unsigned int, unsigned int, unsigned char));
    MOCK_METHOD2(get_pixel, unsigned char(unsigned int, unsigned int));
    MOCK_METHOD4(flip, void(unsigned int, unsigned int, unsigned int, unsigned int));
    MOCK_METHOD0(get_width, unsigned int());
    MOCK_METHOD0(get_height, unsigned int());
};

template <unsigned int WIDTH, unsigned int HEIGHT>
class FakeBuffered2dGraphicsSurface : public AbstractBuffered2dGraphicsSurface<WIDTH, HEIGHT, unsigned char> {

  unsigned char front_buf[HEIGHT][WIDTH];

  public:

    void flip(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
        for (unsigned int y = y1; y <= y2; y++) {
            for (unsigned int x = x1; x <= x2; x++) {
                this->front_buf[y][x] = this->buf[y][x];
            }
        }
    }

    };*/

#endif
