//
// Created by cefour on 12.05.22.
//

#ifndef DATACONTAINER_NAMED_TYPE_H
#define DATACONTAINER_NAMED_TYPE_H
#include <memory>

template <typename T, typename Parameter, template <typename> class... Skills>
class named_type : public Parameter, public Skills<named_type<T, Parameter, Skills...>>... {
public:
    using PARAMETER = Parameter;
    using TYPE = T;
public:
    explicit named_type(){ }
    explicit named_type(T const& value) : value_(value) {}
    explicit named_type(T&& value) : value_(std::move(value)) {}
    T& get() { return value_; }
    T const& get() const {return value_; }

    using ref = named_type<T&, Parameter, Skills...>;
    operator ref()
    {
        return ref(value_);
    }
private:
    T value_;
};
#endif //DATACONTAINER_NAMED_TYPE_H
