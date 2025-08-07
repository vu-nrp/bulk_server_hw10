#include <string>
#include "async.h"
#include "dispatcher.h"
#include "commandparser.h"

async::Handle Dispatcher::staticBlockConnectionHandle {0};
std::string Dispatcher::staticBlockBuffer;
size_t Dispatcher::currStaticBlockSize {0};

Dispatcher::Dispatcher(const size_t bulkSize) :
    m_bulkSize(bulkSize)
{
    if (!Dispatcher::staticBlockConnectionHandle)
        Dispatcher::staticBlockConnectionHandle = async::connect(m_bulkSize);
}

Dispatcher::~Dispatcher()
{
    if (m_dynamicBlockConnection)
        async::disconnect(m_dynamicBlockConnection);
}

void Dispatcher::disconnectStatic()
{
    if (Dispatcher::staticBlockConnectionHandle)
        async::disconnect(Dispatcher::staticBlockConnectionHandle);
}

void Dispatcher::process(std::istream &stream)
{
    std::string line;
    if (std::getline(stream, line)) {
        if (line == CommandParser::DynBlockBeg) {
            m_braces.push(line.front());
            m_dynamicBlock += (line + "\n");
        } else if ((line == CommandParser::DynBlockEnd) && !m_braces.empty() && (m_braces.top() == *CommandParser::DynBlockBeg.c_str())) {
            m_braces.pop();
            m_dynamicBlock += (line + "\n");
            if (m_braces.empty() || (stream.peek() == -1)) {
                if (!m_dynamicBlockConnection) {
                    m_dynamicBlockConnection = async::connect(m_bulkSize);
                }
                async::receive(m_dynamicBlockConnection, m_dynamicBlock.c_str(), m_dynamicBlock.size());
            }
        } else if (!m_braces.empty()) {
            m_dynamicBlock += (line + "\n");
        } else {
            Dispatcher::staticBlockBuffer += (line + "\n");
            ++Dispatcher::currStaticBlockSize;
            if ((Dispatcher::currStaticBlockSize == m_bulkSize) || (stream.peek() == -1)) {
                async::receive(Dispatcher::staticBlockConnectionHandle, Dispatcher::staticBlockBuffer.c_str(), Dispatcher::staticBlockBuffer.size());
                Dispatcher::currStaticBlockSize = 0;
                Dispatcher::staticBlockBuffer.clear();
            }
        }
    }
}
