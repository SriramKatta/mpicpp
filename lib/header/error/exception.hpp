#ifndef MPICPP_HEADER_ERROR_EXCEPTION_HPP
#define MPICPP_HEADER_ERROR_EXCEPTION_HPP
#pragma once

#include <string>
#include <exception>


namespace mpicpp
{
    class exception : public std::exception
    {
        std::string error_string;

    public:
        exception(int errorcode);
        exception(const char *msg);
        const char *what() const noexcept override;
    };

    void handle_error(int errorcode);
}

#endif
