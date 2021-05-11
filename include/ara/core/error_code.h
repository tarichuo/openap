#pragma once

#include "ara/core/error_domain.h"

namespace ara {
namespace core {

class ErrorCode final {
public:
    template <typename EnumT> 
    constexpr ErrorCode(EnumT e, ErrorDomain::SupportDataType data=ErrorDomain::SupportDataType()) noexcept :
        MakeErrorCode(static_cast<ErrorDomain::CodeType>(e), data) { 
    }

    constexpr ErrorCode(ErrorDomain::CodeType value, 
        const ErrorDomain &domain, 
        ErrorDomain::SupportDataType data=ErrorDomain::SupportDataType()) noexcept :
        _value(value),
        _domain(domain),
        _data(data) {
    }

    constexpr ErrorDomain::CodeType Value() const noexcept {
        return _value;
    }

    constexpr const ErrorDomain& Domain() const noexcept {
        return _domain;
    }

    constexpr ErrorDomain::SupportDataType SupportData() const noexcept {
        return _data;
    }

    StringView Message() const noexcept {
        return _domain.Message(_value);
    }

    void ThrowAsException() const {
        return _domain.ThrowAsException(*this);
    }

private:
    ErrorDomain::CodeType _value;
    ErrorDomain _domain;
    ErrorDomain::SupportDataType _data;
};

constexpr bool operator== (const ErrorCode &lhs, const ErrorCode &rhs) noexcept {
    return ((lhs.Value() == rhs.Value() && lhs.Domain() == rhs.Domain());
}
constexpr bool operator!= (const ErrorCode &lhs, const ErrorCode &rhs) noexcept {
    return ((lhs.Value() != rhs.Value() || lhs.Domain() != rhs.Domain());
}

}
}