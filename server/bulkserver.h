#pragma once

#include "dispatcher.h"
#include <boost/asio.hpp>

//!
//! namespace name reducing
//!

namespace ba = boost::asio;
namespace bi = ba::ip;

//!
//! \brief The Connection class - connection class
//!

class Connection : public std::enable_shared_from_this<Connection>
{
public:
    Connection(bi::tcp::socket socket, const size_t bulkSize);

    void run();

private:
    void readData();

    Dispatcher m_dispatcher;
    ba::streambuf m_streamBuf;
    bi::tcp::socket m_socket;

};

//!
//! \brief The BulkServer class - main server class
//!

class BulkServer
{
public:
    BulkServer(ba::io_context &context, const bi::tcp::endpoint &endPoint, const size_t bulkSize);
    virtual ~BulkServer();

private:
    void accept();

    size_t m_bulkSize;
    bi::tcp::acceptor m_acceptor;
    bi::tcp::socket m_socket;

};
