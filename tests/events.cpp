
#include "tenacious/events.h"
#include "gtest/gtest.h"
#include <stdio.h>

class Emitter {

  public:

    int id;

    Emitter(int id) {
        this->id = id;
    }

    class FrobEvent : public Event<Emitter> {};
    class WangleEvent : public Event<Emitter> {};
    class UnhandledEvent : public Event<Emitter> {};
    class ParamEvent : public Event<Emitter, int> {};
    class UnhandledParamEvent : public Event<Emitter, int> {};

    void frob() {
        emit_event<FrobEvent>(this);
    }

    void wangle() {
        emit_event<WangleEvent>(this);
    }

    void unhandled() {
        emit_event<UnhandledEvent>(this);
    }

    void param(int param) {
        emit_event<ParamEvent>(this, param);
    }

    void unhandled_param(int param) {
        emit_event<UnhandledParamEvent>(this, param);
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
int param_event_param_sum = 0;

template<>
inline void event_handler<Emitter::FrobEvent>(Emitter *emitter) {
    frob_count[emitter->id]++;
}
template<>
void event_handler<typename Emitter::WangleEvent>(Emitter * emitter) {
    wangle_count[emitter->id]++;
}
template<>
void event_handler<Emitter::ParamEvent>(Emitter * emitter, int param) {
    param_event_param_sum += param;
}

TEST(TestEvents, Setup) {
    ::testing::StaticAssertTypeEq<Emitter, Emitter::FrobEvent::emitter_type>;
    ::testing::StaticAssertTypeEq<void, Emitter::FrobEvent::param_type>;
    ::testing::StaticAssertTypeEq<Emitter, Emitter::WangleEvent::emitter_type>;
    ::testing::StaticAssertTypeEq<void, Emitter::WangleEvent::param_type>;
    ::testing::StaticAssertTypeEq<Emitter, Emitter::ParamEvent::emitter_type>;
    ::testing::StaticAssertTypeEq<int, Emitter::ParamEvent::param_type>;
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
    emitter0.param(5);
    emitter1.param(4);

    ASSERT_EQ(2, frob_count[0]);
    ASSERT_EQ(1, frob_count[1]);
    ASSERT_EQ(3, wangle_count[0]);
    ASSERT_EQ(4, wangle_count[1]);
    ASSERT_EQ(9, param_event_param_sum);

    // These two should be no-ops at runtime, but they're included
    // here to make sure we can compile them.
    emitter0.unhandled();
    emitter0.unhandled_param(5);
}
