#pragma once

#include <cstdint>
#include "ara/core/error_code.h"
#include "ara/core/result.h"

namespace ara {
namespace core {

enum class future_status : uint8_t { 
    ready,
    timeout,
};

template <typename T, typename E = ErrorCode>
class Future final {
public:
    Future() noexcept = default;
    Future(const Future &) = delete;
    Future(Future &&other) noexcept;
    ~Future();
    Future& operator=(const Future &) = delete;
    Future& operator=(Future &&other) noexcept;
    T get();
    Result<T, E> GetResult() noexcept;
    bool valid() const noexcept;
    void wait() const;
    template <typename Rep, typename Period>
    future_status wait_for(const std::chrono::duration<Rep, Period> &timeoutDuration) const {

    }
    template <typename Clock, typename Duration>
    future_status wait_until(const std::chrono::time_point<Clock, Duration> &deadline) const {

    }

private:
    template <typename T2, typename E2>
    struct is_future<Future<T2, E2>> : std::true_type {
    }

    template <typename U>
    struct is_future : std::false_type {

    }

    template <typename F>
    using ReturnNewFuture = std::enable_if<is_future<std::result_of<std::decay<F>::type(Future<T,E>)>::type>::value, void>::type; 

    template <typename F>
    using ReturnOldFuture = std::enable_if<!is_future<std::result_of<std::decay<F>::type(Future<T,E>)>::type>::value, void>::type; 

public:
    template <typename ReturnNewFuture>
    auto then(F &&func) -> decltype(func()) {
        
    }

    template <typename ReturnOldFuture>
    auto then(F &&func) -> Future<U, E>
};

}
}