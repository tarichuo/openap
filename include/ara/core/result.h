#pragma once

#include <variant>
#include "error_code.h"

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
        _var = std::move(other);
    }

    template <typename... Args>
    void EmplaceValue(Args &&... args) {
        _var
    }

    template <typename... Args>
    void EmplaceError (Args &&... args);
    void Swap (Result &other) noexcept(
        std::is_nothrow_move_constructible<T >::value 
        && std::is_nothrow_move_assignable< T >::value 
        && std::is_nothrow_move_constructible< E >::value 
        && std::is_nothrow_move_assignable< E >::value);

    bool HasValue () const noexcept;
    explicit operator bool () const noexcept;
    const T& operator* () const &;
    T&& operator* () &&;
    const T* operator-> () const;
    const T& Value () const &;
    T&& Value () &&;
    const E& Error () const &;
    E&& Error () &&;
    Optional<T> Ok () const &;
    Optional<T> Ok () &&;
    Optional<E> Err () const &;
    Optional<E> Err () &&;
    template <typename U>
    T ValueOr (U &&defaultValue) const &;
    template <typename U>
    T ValueOr (U &&defaultValue) &&;
    template <typename G>
    E ErrorOr (G &&defaultError) const &;
    template <typename G>
    E ErrorOr (G &&defaultError) &&;
    template <typename G>
    bool CheckError (G &&error) const;
    const T& ValueOrThrow () const &noexcept(false);
    T&& ValueOrThrow () &&noexcept(false);
    template <typename F>
    T Resolve (F &&f) const;
    template <typename F>
    auto Bind (F &&f) const -> <see below>;

private:
    std::variant<T, E> _var;
};

}
}