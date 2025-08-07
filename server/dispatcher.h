#pragma once

#include <stack>
#include <istream>
#include "types.h"

//!
//! \brief The Dispatcher class - dispatch incoming data
//!

class Dispatcher
{
public:
    explicit Dispatcher(const size_t bulkSize);
    virtual ~Dispatcher();

    static void disconnectStatic();
    void process(std::istream &stream);

private:
    static async::Handle staticBlockConnectionHandle;
    static std::string staticBlockBuffer;
    static size_t currStaticBlockSize;

    std::stack<char> m_braces;
    std::string m_dynamicBlock;
    async::Handle m_dynamicBlockConnection;
    size_t m_bulkSize;

};
