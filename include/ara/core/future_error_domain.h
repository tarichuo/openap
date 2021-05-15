#pragma once

#include "ara/core/error_code.h"
#include "ara/core/error_domain.h"
#include "ara/core/exception.h"

namespace ara {
namespace core {

enum class future_errc : int32_t {
    broken_promise = 101,
    future_already_retrieved = 102,
    promise_already_satisfied = 103,
    no_state = 104,
};

class FutureException : public Exception {
public:
    explicit FutureException(ErrorCode err) noexcept :
        Exception(std::move(err)) {

    }
};

class FutureErrorDomain final : public ErrorDomain { 
public:
    using Errc = future_errc;
    using Exception = FutureException;
    const ErrorDomain::IdType id = 0x8000'0000'0000'0013;

    constexpr FutureErrorDomain() noexcept : 
        ErrorDomain(id) {
    }

    const char* Name() const noexcept override {
        return "Future";
    }

    const char* Message(ErrorDomain::CodeType errorCode) const noexcept override {
        future_errc error = static_cast<future_errc>(errorCode);

        switch (error) {
        case future_errc::broken_promise:
            return "the asynchronous task abandoned its shared state";

        case future_errc::future_already_retrieved:
            return "the contents of the shared state were already accessed";

        case future_errc::promise_already_satisfied:
            return "attempt to store a value into the shared state twice";

        case future_errc::no_state:
            return "attempt to access Promise or Future without an associated state";
        }
    }

    void ThrowAsException(const ErrorCode &errorCode) const noexcept(false) override {
        throw FutureException(errorCode);
    }
};

namespace internal {
    static const FutureErrorDomain future_error_domain;
}

constexpr const ErrorDomain& GetFutureErrorDomain() noexcept {
    return internal::future_error_domain;
}

constexpr ErrorCode MakeErrorCode(future_errc code, ErrorDomain::SupportDataType data) noexcept {
    return ErrorCode(static_cast<ErrorDomain::CodeType>(code), GetFutureErrorDomain(), data);
}

}
}