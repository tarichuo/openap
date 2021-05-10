#pragma once

#include <cstdint>

namespace ara {
namespace core {

class ErrorDomain {
public:
    using IdType = std::uint64_t;
    using CodeType = std::int32_t;
    using SupportDataType = std::uint64_t;

    ErrorDomain(const ErrorDomain &) = delete;
    ErrorDomain(const ErrorDomain &&) = delete;
    ErrorDomain& operator=(const ErrorDomain &) = delete;
    ErrorDomain& operator=(ErrorDomain &&) = delete;

    constexpr bool operator==(const ErrorDomain &other) const noexcept {
        return (_id == other._id)
    }

    constexpr IdType Id() const noexcept {
        return _id;
    }

    virtual const char* Name() const noexcept = 0;
    virtual const char* Message(CodeType errorCode) const noexcept = 0;
    virtual void ThrowAsException (const ErrorCode &errorCode) const noexcept(false) = 0;

protected:
    explicit constexpr ErrorDomain(IdType id) noexcept : _id(id){

    };
    ~ErrorDomain() = default;

private:
    IdType _id;
};
}
}