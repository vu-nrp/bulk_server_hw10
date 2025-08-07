#include "bulkserver.h"
#include "dispatcher.h"
#include <boost/asio/ip/tcp.hpp>

//!
//! \brief Connection
//!

Connection::Connection(bi::tcp::socket socket, const size_t bulkSize) :
    m_dispatcher(bulkSize),
    m_socket(std::move(socket))
{
}

void Connection::run()
{
    readData();
}

void Connection::readData()
{
    const auto self = shared_from_this();

    ba::async_read_until(m_socket, m_streamBuf, '\n', [this, self](boost::system::error_code errCode, size_t /*size*/)
    {
        if (!errCode)
            readData();
    });

    std::istream stream(&m_streamBuf);
    m_dispatcher.process(stream);
}

//!
//! \brief BulkServer
//!

BulkServer::BulkServer(ba::io_context &context, const bi::tcp::endpoint &endPoint, const size_t bulkSize) :
    m_bulkSize(bulkSize),
    m_acceptor(context, endPoint),
    m_socket(context)
{
    accept();
}

void BulkServer::accept()
{
    m_acceptor.async_accept(m_socket, [this](boost::system::error_code errCode)
    {
        if (!errCode)
            std::make_shared<Connection>(std::move(m_socket), m_bulkSize)->run();

        accept();
    });
}

BulkServer::~BulkServer()
{
    Dispatcher::disconnectStatic();
}
