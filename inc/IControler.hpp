#pragma once
#include <IControler.hpp>

#include <CommonMessages.hpp>


class IControler
{
public:
    IControler() { }

    virtual ~IControler() { }

    virtual void receiveMessages() =0;

    virtual void sendResponses() =0;

    virtual void handleInitializations() =0;

    virtual void handleMessages() =0;

    // TODO make private and rewrite the test
    virtual bool registerNode(Message msg) =0;

    virtual void replyWithResetRequest(Header hdr) =0;

    virtual void replyWithAck(Header hdr) =0;

};

