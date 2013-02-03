#ifndef TENACIOUS_EVENTS
#define TENACIOUS_EVENTS

template<class EVENT_TYPE>
inline void event_handler(typename EVENT_TYPE::emitter_type *emitter) {
    // do nothing by default
}

template <class EVENT_TYPE>
inline void emit_event(typename EVENT_TYPE::emitter_type *obj) {
    event_handler<EVENT_TYPE>(obj);
}

template <class EMITTER_TYPE>
class Event {
  public:
    typedef EMITTER_TYPE emitter_type;
};

#endif
