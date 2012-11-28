#include "RPC.h"

namespace NsHMIEmulator
{
    log4cplus::Logger RPC::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("testAppMgr"));

    /**
    * \brief Constructor.
    * \param address remote network address or FQDN
    * \param port remote local port
    */
    RPC::RPC(const std::string &address, unsigned short port, const std::string &name)
        :CMessageBrokerController(address, port, name)
    {
    }

    /**
    * \brief Destructor.
    */
    RPC::~RPC()
    {
    }

    /**
    * \brief process request.
    * \param root JSON message.
    */
    void RPC::processRequest(Json::Value &root)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Request arrived");
        if (getControllersName() == getDestinationComponentName(root))
        {
            NsRPC2Communication::RPC2Command * currentCommand = NsRPC2Communication::Marshaller::fromJSON( root );
            if ( !currentCommand )
            {
                LOG4CPLUS_ERROR_EXT( mLogger, "Invalid RPCBus message received." );
                return;
            }
            LOG4CPLUS_INFO_EXT(mLogger, "handle command" );
            messageReceivedFromDeviceCallback( currentCommand );
        }
        else
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "Wrong message destination!");
        }
    }

    /**
    * \brief process notification.
    * \param root JSON message.
    */
    void RPC::processNotification(Json::Value &root)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Notification arrived");
        NsRPC2Communication::RPC2Command * currentCommand = NsRPC2Communication::Marshaller::fromJSON( root );
        if ( !currentCommand )
        {
            LOG4CPLUS_ERROR_EXT( mLogger, "Invalid RPCBus message received." );
            return;
        }
        LOG4CPLUS_INFO_EXT(mLogger, "handle notification" );
        messageReceivedFromDeviceCallback( currentCommand );
    }

    /**
    * \brief process response.
    * \param method method name which has been called.
    * \param root JSON message.
    */
    void RPC::processResponse(std::string method, Json::Value &root)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Response arrived");
        ResponseContainer response;
        response.methodName = method;
        response.response = root;
        NsRPC2Communication::RPC2Command * currentCommand = NsRPC2Communication::Marshaller::fromJSON( response.response );
        if ( !currentCommand )
        {
            LOG4CPLUS_ERROR_EXT( mLogger, "Invalid RPCBus message received." );
            return;
        }
        LOG4CPLUS_INFO_EXT(mLogger, "handle response" );
        messageReceivedFromDeviceCallback( currentCommand );
    }

    /**
     * \brief send a message to a mobile side via applinkcore
     * \param command Pointer to base class of AppLink Json object
     */
    void RPC::sendRPC2MessageToMobileSide(NsRPC2Communication::RPC2Command *command)
    {
        LOG4CPLUS_INFO_EXT(mLogger, " Sending to the mobile side a message " << command->getMethod());
        Json::Value message = NsRPC2Communication::Marshaller::toJSON(command);
        sendJsonMessage(message);
    }
}
