#include <iostream>
#include <boost/asio.hpp>
#include "async.h"
#include "bulkserver.h"

//!
//! \brief main - app entry point
//!

int main(int argc, char *argv[])
{
//    std::cout << "Home work #10" << std::endl;

    if (argc != 3) {
        std::cerr << "Usage: bulk_server <port> <bulk_size>" << std::endl;
        return 1;
    }

    async::init();

    try {
        ba::io_context context;
        BulkServer server(context, bi::tcp::endpoint(bi::tcp::v4(), std::atoi(argv[1])), std::atoi(argv[2]));

        context.run();
    } catch (std::exception &exc) {
        std::cout << exc.what() << std::endl;
    }

    async::deinit();

    return 0;
}
