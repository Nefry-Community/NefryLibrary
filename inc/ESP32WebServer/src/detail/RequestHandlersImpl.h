#ifndef REQUESTHANDLERSIMPL_H
#define REQUESTHANDLERSIMPL_H

#include "RequestHandler.h"

class FunctionRequestHandler : public RequestHandler {
public:
    FunctionRequestHandler(ESP32WebServer::THandlerFunction fn, ESP32WebServer::THandlerFunction ufn, const char* uri, HTTPMethod method)
    : _fn(fn)
    , _ufn(ufn)
    , _uri(uri)
    , _method(method)
    {
    }

    bool canHandle(HTTPMethod requestMethod, String requestUri) override  {
        if (_method != HTTP_ANY && _method != requestMethod)
            return false;

        if (requestUri != _uri)
            return false;

        return true;
    }

    bool canUpload(String requestUri) override  {
        if (!_ufn || !canHandle(HTTP_POST, requestUri))
            return false;

        return true;
    }

    bool handle(ESP32WebServer& server, HTTPMethod requestMethod, String requestUri) override {
        if (!canHandle(requestMethod, requestUri))
            return false;

        _fn();
        return true;
    }

    void upload(ESP32WebServer& server, String requestUri, HTTPUpload& upload) override {
        if (canUpload(requestUri))
            _ufn();
    }

protected:
    ESP32WebServer::THandlerFunction _fn;
    ESP32WebServer::THandlerFunction _ufn;
    String _uri;
    HTTPMethod _method;
};



#endif //REQUESTHANDLERSIMPL_H
