#pragma once

#include "ara/core/error_domain.h"
#include "ara/core/exception.h"

namespace ara {
namespace core {

enum class CoreErrc : ErrorDomain::CodeType {
    kInvalidArgument = 22,
    kInvalidMetaModelShortname = 137,
    kInvalidMetaModelPath = 138,
};

class CoreException : public Exception {
public:
    explicit CoreException(ErrorCode err) noexcept {

    }


};

class CoreErrorDomain final : public ErrorDomain {
public:
    using Errc = CoreErrc;
    using Exception = CoreException;

    constexpr CoreErrorDomain() noexcept {

    }

    const char* Name () const noexcept override {
        return "core";
    }

    const char* Message (ErrorDomain::CodeType errorCode) const noexcept override {
        switch (errorCode) {
        case static_cast<ErrorDomain::CodeType>(CoreErrc::kInvalidArgument):
            return "an invalid argument was passed to a function";

        case static_cast<ErrorDomain::CodeType>(CoreErrc::kInvalidMetaModelShortname):
            return "given string is not a valid model element shortname";

        case static_cast<ErrorDomain::CodeType>(CoreErrc::kInvalidMetaModelPath):
            return "missing or invalid path to model elemen";

        }
    }
};

}
}