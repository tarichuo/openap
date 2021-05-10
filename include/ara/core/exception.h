#pragma once

#include "error_code.h"
#include <exception>

namespace ara {
namespace core {

class Exception : public std::exception {
public:
    explicit Exception(ErrorCode err) noexcept :
        _errorcode(err) {
    }

    const char* what() const noexcept override {
        return std::exception::what();
    }

    const ErrorCode& Error() const noexcept {
        return _errorcode;
    }


protected:
    Exception& operator=(Exception const &other) {
        _errorcode = other._errorcode;
    }


private:
    ErrorCode _errorcode;
};



}
}