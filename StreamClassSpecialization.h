#ifndef STREAM_CLASS_SPECIALIZATION_H
#define STREAM_CLASS_SPECIALIZATION_H

#include <functional>

#define OPERATOR_OVERRIDE(method) \
    template<typename F> \
    decltype(auto) method (F&& f) \
        { return Super :: method (std::forward<F>(f)); } \
    template<typename R> \
    decltype(auto) method (R (T::*member)()) \
        { return Super :: method (std::mem_fn(member)); } \
    template<typename R> \
    decltype(auto) method (R (T::*member)() const) \
        { return Super :: method (std::mem_fn(member)); }

template<typename T>
class StreamImpl<T, StreamTag::Class> 
        : public StreamImpl<T, StreamTag::Common> {

private:
    using Super = StreamImpl<T, StreamTag::Common>;

public:
    using ElementType = T;
    using iterator = typename StreamProvider<T>::Iterator;

    StreamImpl() : Super() {}

    template<typename Iterator>
    StreamImpl(Iterator begin, Iterator end) : Super(begin, end) {}

    template<typename Container>
    StreamImpl(const Container& cont) : Super(cont) {}

    OPERATOR_OVERRIDE(filter);
    OPERATOR_OVERRIDE(take_while);
    OPERATOR_OVERRIDE(drop_while);
    OPERATOR_OVERRIDE(peek);
    OPERATOR_OVERRIDE(map);
    OPERATOR_OVERRIDE(flat_map);

    OPERATOR_OVERRIDE(any);
    OPERATOR_OVERRIDE(all);
    OPERATOR_OVERRIDE(none);
    OPERATOR_OVERRIDE(for_each);

    template<typename, StreamTag>
    friend class StreamImpl;

    friend class MakeStream;

    template<typename, typename, typename>
    friend class FlatMappedStreamProvider;

private:
    StreamImpl(StreamProviderPtr<T> source) : Super(std::move(source)) {}
};

#undef OPERATOR_OVERRIDE

#endif