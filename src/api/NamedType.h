//
// Created by cefour on 12.05.22.
//

#ifndef DATACONTAINER_NAMEDTYPE_H
#define DATACONTAINER_NAMEDTYPE_H
#include <memory>

template <typename T, typename Parameter, template <typename> class... Skills>
class NamedType : public Parameter, public Skills<NamedType<T, Parameter, Skills...>>... {
public:
    using PARAMETER = Parameter;
public:
    explicit NamedType(T const& value) : value_(value) {}
    explicit NamedType(T&& value) : value_(std::move(value)) {}
    T& get() { return value_; }
    T const& get() const {return value_; }

    using ref = NamedType<T&, Parameter, Skills...>;
    operator ref()
    {
        return ref(value_);
    }
private:
    T value_;
};
#endif //DATACONTAINER_NAMEDTYPE_H
