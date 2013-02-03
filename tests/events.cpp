
#include "tenacious/events.h"
#include "gtest/gtest.h"

class Emitter {

  public:

    int id;

    Emitter(int id) {
        this->id = id;
    }

    class FrobEvent : public Event<Emitter> {};
    class WangleEvent : public Event<Emitter> {};

    void frob() {
        emit_event<FrobEvent>(this);
    }

    void wangle() {
        emit_event<WangleEvent>(this);
    }
};

// In our event model handlers attach to event types rather than to instances,
// and it's then up to the handler to use some properties of the emitter
// to distinguish them. But we make two instances here just to make sure
// we can indeed pass through the right instances.
Emitter emitter0(0);
Emitter emitter1(1);
int frob_count[2] = {0, 0};
int wangle_count[2] = {0, 0};

template<>
inline void event_handler<Emitter::FrobEvent>(Emitter *emitter) {
    frob_count[emitter->id]++;
}
template<>
void event_handler<typename Emitter::WangleEvent>(Emitter * emitter) {
    wangle_count[emitter->id]++;
}

TEST(TestEvents, Setup) {
    ::testing::StaticAssertTypeEq<Emitter, Emitter::FrobEvent::emitter_type>;
    ::testing::StaticAssertTypeEq<Emitter, Emitter::WangleEvent::emitter_type>;
}

TEST(TestEvents, Emitting) {

    emitter0.frob();
    emitter0.frob();
    emitter1.frob();
    emitter0.wangle();
    emitter0.wangle();
    emitter0.wangle();
    emitter1.wangle();
    emitter1.wangle();
    emitter1.wangle();
    emitter1.wangle();

    ASSERT_EQ(2, frob_count[0]);
    ASSERT_EQ(1, frob_count[1]);
    ASSERT_EQ(3, wangle_count[0]);
    ASSERT_EQ(4, wangle_count[1]);

}
