#ifndef ALAMBRE_EVENTS
#define ALAMBRE_EVENTS

template<class EVENT_TYPE>
inline void event_handler(typename EVENT_TYPE::emitter_type *emitter) {
    // do nothing by default
}

template<class EVENT_TYPE>
inline void event_handler(typename EVENT_TYPE::emitter_type *emitter, typename EVENT_TYPE::param_type param) {
    // do nothing by default
}

template <class EVENT_TYPE>
inline void emit_event(typename EVENT_TYPE::emitter_type *obj) {
    event_handler<EVENT_TYPE>(obj);
}
template <class EVENT_TYPE>
inline void emit_event(typename EVENT_TYPE::emitter_type *obj, typename EVENT_TYPE::param_type param) {
    event_handler<EVENT_TYPE>(obj, param);
}

template <class EMITTER_TYPE, class PARAM_TYPE=void>
class Event {
  public:
    typedef EMITTER_TYPE emitter_type;
    typedef PARAM_TYPE param_type;
};

#endif
