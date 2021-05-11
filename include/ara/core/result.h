#pragma once

#include <variant>
#include "ara/core/error_code.h"

namespace ara {
namespace core {

template <typename T, typename E = ErrorCode>
class Result final {
public:
    using value_type = T;
    using error_type = E;

    Result(const T &t) :
        _var(t) {
    }

    Result(T &&t) : 
        _var(std::move(t)) {
    }

    explicit Result(const E &e) :
        _var(e) {
    }

    explicit Result(E &&e) : 
        _var(std::move(e)) {
    }

    Result(const Result &other) : 
        _var(other._var) {
    }

    Result(Result &&other) noexcept(
        std::is_nothrow_move_constructible<T>::value && std::is_nothrow_move_constructible< E >::value) :
        _var(std::move(other._var)) {

    }

    ~Result() {

    }

    static Result FromValue(const T &t) {
        return Result(t);
    }

    static Result FromValue(T &&t) {
        return Result(std::move(t));
    }

    template <typename... Args>
    static Result FromValue(Args &&... args) {
        return Result(T{std::forward<Args>(args)});
    }

    static Result FromError(const E &e) {
        return Result(e);
    }

    static Result FromError(E &&e) {
        return Result(std::move(e));
    }

    template <typename... Args>
    static Result FromError(Args &&... args) {
        return Result(E{std::forward<Args>(args)});
    }

    Result& operator=(const Result &other) {
        _var = other._var;
    }

    Result& operator= (Result &&other) noexcept(
        std::is_nothrow_move_constructible<T>::value 
        && std::is_nothrow_move_assignable<T>::value 
        && std::is_nothrow_move_constructible<E>::value 
        && std::is_nothrow_move_assignable<E>::value) {
        _var = std::move(other._var);
    }

    template <typename... Args>
    void EmplaceValue(Args &&... args) {
        _var = T(std::forward<Args>(args)...);
    }

    template <typename... Args>
    void EmplaceError(Args &&... args) {
        _var = E(std::forward<Args>(args)...);
    }

    void Swap(Result &other) noexcept(
        std::is_nothrow_move_constructible<T >::value 
        && std::is_nothrow_move_assignable< T >::value 
        && std::is_nothrow_move_constructible< E >::value 
        && std::is_nothrow_move_assignable< E >::value) {
        std::swap(_var, other._var);
    }

    bool HasValue() const noexcept {
        return _var.index() == 0;
    }

    explicit operator bool() const noexcept {
        return HasValue();
    }

    const T& operator*() const & {
        return Value();
    }

    T&& operator*() && {
        return Value();
    }

    const T* operator->() const {
        return std::addressof(Value());
    }

    const T& Value() const & {
        return std::get<T>(_var);
    }

    T&& Value() && {
        return std::move(std::get<T>(_var));
    }

    const E& Error() const & {
        return std::get<E>(_var);
    }

    E&& Error() && {
        return std::move(std::get<E>(_var));
    }

    Optional<T> Ok() const & {
        return HasValue() ? Optional<T>(std::in_place_t, Value()) : Optional<T>(std::nullopt);
    }

    Optional<T> Ok() && {
        return HasValue() ? std::move(Optional<T>(std::in_place_t, Value())) : std::move(Optional<T>(std::nullopt));
    }

    Optional<E> Err() const & {
        return HasValue() ? Optional<E>(std::nullopt) : Optional<E>(std::in_place_t, Error());
    }

    Optional<E> Err() && {
        return HasValue() ? std::move(Optional<E>(std::nullopt)) : std::move(Optional<E>(std::in_place_t, Error()));
    }

    template <typename U>
    T ValueOr(U &&defaultValue) const & {
        return HasValue() ? Value() : static_cast<T>(std::forward<U>(defaultValue));
    }

    template <typename U>
    T ValueOr(U &&defaultValue) && {
        return HasValue() ? std::move(Value()) : static_cast<T>(std::forward<U>(defaultValue));
    }

    template <typename G>
    E ErrorOr(G &&defaultError) const & {
        return HasValue() ? static_cast<E>(std::forward<G>(defaultValue)) : Error();
    }

    template <typename G>
    E ErrorOr(G &&defaultError) && {
        return HasValue() ? static_cast<E>(std::forward<G>(defaultValue)) : std::move(Error());
    }

    template <typename G>
    bool CheckError(G &&error) const {
        return HasValue() ? false : (Error() == static_cast<E>(std::forward<G>(error)));
    }

    const T& ValueOrThrow() const & noexcept(false) {
        if (HasValue()) {
            return Value();
        }

        std::get<E>(_var).ThrowAsException();
    }

    T&& ValueOrThrow() && noexcept(false) {
        if (HasValue()) {
            return Value();
        } 

        std::get<E>(_var).ThrowAsException();
    }

    template <typename F>
    T Resolve(F &&f) const {
        if (HasValue()) {
            return Value();
        }

        return std::forward<T>(f)(Error());
    }

    template <typename F>
    auto Bind(F &&f) const -> decltype(f(Value())) {
        return std::forward<F>(f)(Value());
    }

    template <typename F>
    auto Bind(F &&f) const -> Result<decltype(f(Value())), E> {
        if (HasValue()) {
            return Result<decltype(f(Value())), E>(std::forward<F>(f)(Value()));    
        }

        return Result<decltype(f(Value())), E>(Error());
    }

private:
    std::variant<T, E> _var;
};

template <typename E = ErrorCode>
class Result<void, E> final {
private:
    struct dummy {
    };
    using T = dummy;

public:
    using value_type = void;
    using error_type = E;

    Result() :
        _var(T{}) {
    }

    explicit Result(const E &e) :
        _var(e) {
    }

    explicit Result(E &&e) : 
        _var(std::move(e)) {
    }

    Result(const Result &other) : 
        _var(other._var) {
    }

    Result(Result &&other) noexcept(
        std::is_nothrow_move_constructible<T>::value && std::is_nothrow_move_constructible< E >::value) :
        _var(std::move(other._var)) {

    }

    ~Result() {

    }

    static Result FromValue() {
        return Result();
    }

    static Result FromError(const E &e) {
        return Result(e);
    }

    static Result FromError(E &&e) {
        return Result(std::move(e));
    }

    template <typename... Args>
    static Result FromError(Args &&... args) {
        return Result(E{std::forward<Args>(args)});
    }

    Result& operator=(const Result &other) {
        _var = other._var;
    }

    Result& operator= (Result &&other) noexcept(
        std::is_nothrow_move_constructible<T>::value 
        && std::is_nothrow_move_assignable<T>::value 
        && std::is_nothrow_move_constructible<E>::value 
        && std::is_nothrow_move_assignable<E>::value) {
        _var = std::move(other._var);
    }

    template <typename... Args>
    void EmplaceValue(Args &&... args) {
        _var = T(std::forward<Args>(args)...);
    }

    template <typename... Args>
    void EmplaceError(Args &&... args) {
        _var = E(std::forward<Args>(args)...);
    }

    void Swap(Result &other) noexcept(
        std::is_nothrow_move_constructible<T >::value 
        && std::is_nothrow_move_assignable< T >::value 
        && std::is_nothrow_move_constructible< E >::value 
        && std::is_nothrow_move_assignable< E >::value) {
        std::swap(_var, other._var);
    }

    bool HasValue() const noexcept {
        return _var.index() == 0;
    }

    explicit operator bool() const noexcept {
        return HasValue();
    }

    const T& operator*() const & {
        return Value();
    }

    void operator*() && {
    }

    void Value() const {

    }

    const E& Error() const & {
        return std::get<E>(_var);
    }

    E&& Error() && {
        return std::move(std::get<E>(_var));
    }

    Optional<E> Err() const & {
        return HasValue() ? Optional<E>(std::nullopt) : Optional<E>(std::in_place_t, Error());
    }

    Optional<E> Err() && {
        return HasValue() ? std::move(Optional<E>(std::nullopt)) : std::move(Optional<E>(std::in_place_t, Error()));
    }

    template <typename U>
    void ValueOr(U &&defaultValue) const & {
    }

    template <typename G>
    E ErrorOr(G &&defaultError) const & {
        return HasValue() ? static_cast<E>(std::forward<G>(defaultValue)) : Error();
    }

    template <typename G>
    E ErrorOr(G &&defaultError) && {
        return HasValue() ? static_cast<E>(std::forward<G>(defaultValue)) : std::move(Error());
    }

    template <typename G>
    bool CheckError(G &&error) const {
        return HasValue() ? false : (Error() == static_cast<E>(std::forward<G>(error)));
    }

    void ValueOrThrow() const & noexcept(false) {
        if (HasValue()) {
            // do nothing
        }

        std::get<E>(_var).ThrowAsException();
    }

    template <typename F>
    void Resolve(F &&f) const {
        if (HasValue()) {
            // do nothing
        }

        return std::forward<T>(f)(Error());
    }

    template <typename F>
    auto Bind(F &&f) const -> decltype(f()) {
        Value()
        return std::forward<F>(f)();
    }

    template <typename F>
    auto Bind(F &&f) const -> Result<decltype(f()), E> {
        if (HasValue()) {
            return Result<decltype(f()), E>(std::forward<F>(f)());    
        }

        return Result<decltype(f()), E>(Error());
    }

private:
    std::variant<T, E> _var;
};

template <typename T, typename E>
bool operator==(const Result<T, E> &lhs, const Result<T, E> &rhs) {
    if (lhs.HasValue() && rhs.HasValue()) {
        if (lhs.Value() == rhs.Value()) {
            return true;
        }
    }

    return false;
}

template <typename T, typename E>
bool operator!=(const Result<T, E> &lhs, const Result<T, E> &rhs) {
    return !(lhs == rhs);
}

template <typename T, typename E>
bool operator==(const Result<T, E> &lhs, const T &rhs) {
    if (lhs.HasValue()) {
        if (lhs.Value() == rhs) {
            return true;
        }
    }

    return false;
}

template <typename T, typename E>
bool operator== (const T &lhs, const Result<T, E> &rhs) {
    if (rhs.HasValue()) {
        if (rhs.Value() == lhs) {
            return true;
        }
    }

    return false;
}

template <typename T, typename E>
bool operator!= (const Result<T, E> &lhs, const T &rhs) {
    return !(lhs == rhs);
}

template <typename T, typename E>
bool operator!= (const T &lhs, const Result<T, E> &rhs) {
    return !(lhs == rhs);
}

template <typename T, typename E>
bool operator==(const Result<T, E> &lhs, const E &rhs) {
    if (!lhs.HasValue()) {
        if (lhs.Error() == rhs) {
            return true;
        }
    }

    return false;
}

template <typename T, typename E>
bool operator==(const E &lhs, const Result<T, E> &rhs) {
    if (!rhs.HasValue()) {
        if (rhs.Error() == lhs) {
            return true;
        }
    }

    return false;  
}

template <typename T, typename E>
bool operator!=(const Result<T, E> &lhs, const E &rhs) {
    return !(lhs == rhs);
}

template <typename T, typename E>
bool operator!=(const E &lhs, const Result<T, E> &rhs) {
    return !(lhs == rhs);
}

template <typename T, typename E>
void swap(Result<T, E> &lhs, Result<T, E> &rhs) noexcept(noexcept(lhs.Swap(rhs))) {
    lhs.Swap(rhs);
}

}
}