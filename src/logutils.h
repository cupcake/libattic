#ifndef LOGUTILS_H_
#define LOGUTILS_H_
#pragma once

#include "event.h"
#include "response.h"

namespace attic { namespace log {

static void LogHttpResponse(const std::string& error_ident, const Response& resp) {
    std::ostringstream error;
    error << error_ident << " ";
    error << "Non 200 repsonse" << std::endl;
    error << "Code : \n" << resp.code << std::endl;
    error << "Headers : \n" << resp.header.asString() << std::endl;
    error << "Body : \n" << resp.body << std::endl;
    event::RaiseEvent(event::Event::ERROR_NOTIFY, error.str(), NULL);
}


}}//namespace
#endif

