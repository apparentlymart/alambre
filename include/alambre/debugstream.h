#ifndef ALAMBRE_DEBUGSTREAM
#define ALAMBRE_DEBUGSTREAM

#include <alambre/events.h>
#include <stdio.h>

class DebugStream {};
constexpr DebugStream debug_stream;

template<typename DUMMY>
constexpr inline bool debug_stream_enabled() {
    return false;
}

class DebugStreamEvent : public Event<const char> {};


template <typename DUMMY=void>
inline const DebugStream operator<<(const DebugStream stream, const char * msg) {
    if (debug_stream_enabled<DUMMY>()) {
        emit_event<DebugStreamEvent>(msg);
    }
    return stream;
}

template <typename DUMMY=void>
const DebugStream operator<<(const DebugStream stream, int value) {
    if (debug_stream_enabled<DUMMY>()) {
        char buf[20];
        snprintf(buf, sizeof(buf), "%i", value);
        emit_event<DebugStreamEvent>(buf);
    }
    return stream;
}

template <typename DUMMY=void>
const DebugStream operator<<(const DebugStream stream, unsigned int value) {
    if (debug_stream_enabled<DUMMY>()) {
        char buf[20];
        snprintf(buf, sizeof(buf), "%u", value);
        emit_event<DebugStreamEvent>(buf);
    }
    return stream;
}

template <typename DUMMY=void>
const DebugStream operator<<(const DebugStream stream, short value) {
    if (debug_stream_enabled<DUMMY>()) {
        char buf[20];
        snprintf(buf, sizeof(buf), "%hi", value);
        emit_event<DebugStreamEvent>(buf);
    }
    return stream;
}

template <typename DUMMY=void>
const DebugStream operator<<(const DebugStream stream, unsigned short value) {
    if (debug_stream_enabled<DUMMY>()) {
        char buf[20];
        snprintf(buf, sizeof(buf), "%hu", value);
        emit_event<DebugStreamEvent>(buf);
    }
    return stream;
}

template <typename DUMMY=void>
const DebugStream operator<<(const DebugStream stream, long value) {
    if (debug_stream_enabled<DUMMY>()) {
        char buf[20];
        snprintf(buf, sizeof(buf), "%liL", value);
        emit_event<DebugStreamEvent>(buf);
    }
    return stream;
}

template <typename DUMMY=void>
const DebugStream operator<<(const DebugStream stream, unsigned long value) {
    if (debug_stream_enabled<DUMMY>()) {
        char buf[20];
        snprintf(buf, sizeof(buf), "%luL", value);
        emit_event<DebugStreamEvent>(buf);
    }
    return stream;
}

template <typename DUMMY=void>
const DebugStream operator<<(const DebugStream stream, char value) {
    if (debug_stream_enabled<DUMMY>()) {
        char buf[2] = { value, 0 };
        emit_event<DebugStreamEvent>(buf);
    }
    return stream;
}

template <typename DUMMY=void>
const DebugStream operator<<(const DebugStream stream, float value) {
    if (debug_stream_enabled<DUMMY>()) {
        char buf[20];
        snprintf(buf, sizeof(buf), "%g", value);
        emit_event<DebugStreamEvent>(buf);
    }
    return stream;
}

template <typename DUMMY=void>
const DebugStream operator<<(const DebugStream stream, double value) {
    if (debug_stream_enabled<DUMMY>()) {
        char buf[20];
        snprintf(buf, sizeof(buf), "%g", value);
        emit_event<DebugStreamEvent>(buf);
    }
    return stream;
}

template <typename DUMMY=void>
const DebugStream operator<<(const DebugStream stream, bool value) {
    if (debug_stream_enabled<DUMMY>()) {
        if (value) {
            emit_event<DebugStreamEvent>("true");
        }
        else {
            emit_event<DebugStreamEvent>("false");
        }
    }
    return stream;
}

// This (overloading with the type of nullptr) crashes GCC <= 4.7.0. :(
// http://gcc.gnu.org/bugzilla/show_bug.cgi?id=51530
/*
template <typename DUMMY=void>
const DebugStream operator<<(const DebugStream stream, decltype(nullptr) value) {
    if (debug_stream_enabled<DUMMY>()) {
        emit_event<DebugStreamEvent>("nullptr");
    }
    return stream;
}*/

template <typename REFERENT_TYPE>
const DebugStream operator<<(const DebugStream stream, REFERENT_TYPE *value) {
    if (debug_stream_enabled<void>()) {
        char buf[20];
        snprintf(buf, sizeof(buf), "%p", value);
        emit_event<DebugStreamEvent>(buf);
    }
    return stream;
}

template <typename REFERENT_TYPE>
const DebugStream operator<<(const DebugStream stream, REFERENT_TYPE value) {
    if (debug_stream_enabled<void>()) {
        char buf[50];
        snprintf(buf, sizeof(buf), "<unknown at %p>", &value);
        emit_event<DebugStreamEvent>(buf);
    }
    return stream;
}

#define ENABLE_DEBUG_STREAM \
    template<> \
    constexpr inline bool debug_stream_enabled<void>() {\
        return true;\
    };\
    template<>\
    void event_handler<DebugStreamEvent>(const char * msg)

#endif

