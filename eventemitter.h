#ifndef _EVENT_EMITTER_H_
#define _EVENT_EMITTER_H_

#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <list>
#include <algorithm>

template <typename _EventId, typename... _Args>
class EventEmitter
{
public:
    using EventId = _EventId;
    using Handler = std::function<void(_Args...)>;
    using ListenerId = unsigned int;

    EventEmitter() {}
    ~EventEmitter() {}

    ListenerId on(_EventId event_id, Handler cb);
    void off(ListenerId listener_id);
    void exec(_EventId event_id, _Args... data);

private:
    struct ListenerBase
    {
        ListenerBase() {}

        explicit ListenerBase(ListenerId i)
            : id(i) {}

        virtual ~ListenerBase() {}

        ListenerId id;
    };

    struct Listener : public ListenerBase
    {
        Listener() {}

        Listener(ListenerId i, Handler c)
            : ListenerBase(i), cb(c) {}

        Handler cb;
    };

    std::mutex mutex;
    ListenerId last_listener;
    std::multimap<_EventId, std::shared_ptr<ListenerBase>> listeners;

    EventEmitter(const EventEmitter&) = delete;
    const EventEmitter& operator = (const EventEmitter&) = delete;
};

template <typename _EventId, typename... _Args>
typename EventEmitter<_EventId, _Args...>::ListenerId EventEmitter<_EventId, _Args...>::on(_EventId event_id, Handler cb)
{
    if (!cb)
    {
        throw std::invalid_argument("EventEmitter::on: No callbak provided.");
    }

    std::lock_guard<std::mutex> lock(mutex);

    ListenerId listener_id = ++last_listener;
    listeners.insert(
        std::make_pair(event_id, std::make_shared<Listener>(listener_id, cb)));

    return listener_id;
}

template <typename _EventId, typename... _Args>
void EventEmitter<_EventId, _Args...>::off(ListenerId listener_id)
{
    std::lock_guard<std::mutex> lock(mutex);

    auto i = std::find_if(listeners.begin(), listeners.end(), [&](std::pair<const _EventId, std::shared_ptr<ListenerBase>> p) {
        return p.second->id == listener_id;
    });
    if (i != listeners.end())
    {
        listeners.erase(i);
    }
    else
    {
        throw std::invalid_argument("EventEmitter::off: Invalid listener id.");
    }
}

template <typename _EventId, typename... _Args>
void EventEmitter<_EventId, _Args...>::exec(_EventId event_id, _Args... data)
{
    std::list<std::shared_ptr<Listener>> handlers;

    {
        std::lock_guard<std::mutex> lock(mutex);

        auto range = listeners.equal_range(event_id);
        handlers.resize(std::distance(range.first, range.second));
        std::transform(range.first, range.second, handlers.begin(), [](std::pair<const _EventId, std::shared_ptr<ListenerBase>> p) {
            auto l = std::dynamic_pointer_cast<Listener>(p.second);
            if (l)
            {
                return l;
            }
            else
            {
                throw std::logic_error("EventEmitter::exec: Invalid event signature.");
            }
        });
    }

    for (auto& h : handlers)
    {
        h->cb(data...);
    }
}

#endif // _EVENT_EMITTER_H_
