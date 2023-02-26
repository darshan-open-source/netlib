// bug free
#ifndef CORE_SERVER
#define CORE_SERVER

#ifndef ASIO_STANDALONE
#define ASIO_STANDALONE
#endif

#include <iostream>
#include <asio.hpp>
#include <thread>
#include <vector>
#include <memory>

using namespace std;

typedef enum _WAIT_TYPE
{
    wait_read = asio::socket_base::wait_read,
    wait_write = asio::socket_base::wait_write,
    wait_error = asio::socket_base::wait_error
} wait_type;

typedef std::function<
    void(asio::ip::tcp::socket
             sockets,
         const error_code &ec,
         void *m)>
    acceptorcallback;
namespace darshan
{
    namespace network
    {

        class SocketConnection
        {
        public:
            SocketConnection(asio::ip::tcp::socket &s);

            ~SocketConnection();

            size_t readUntilCRLF(std::string *s);

            void close();
            auto getSocket();

            asio::ip::tcp::socket &getsockref();

            size_t avaliable();

            bool isConnected();

            void wait(wait_type w);

            size_t writeString(std::string s);

            size_t writeString(std::string s, size_t length);

            size_t readString(std::string *s, size_t length);

            size_t readData(char *data, size_t length);

            size_t writeData(char *data, size_t length);
            void shutdownRead();
            void shutdownWrite();
            void shutdownBoth();
            void waitRead();

            void waitWrite();

            shared_ptr<asio::ip::tcp::socket> ptr;
        };

        class SocketServer
        {
        public:
            asio::io_context io;

            SocketServer(char *host, int port);

            asio::ip::tcp::socket::native_handle_type getServerSocket();

            int getPort();

            std::string getAddress();
            virtual void onClientConnected(shared_ptr<SocketConnection>);

            void runServer();

            bool startServer();
            void stopServer();
            ~SocketServer();

        private:
            void waitForConnection();
            asio::ip::tcp::acceptor acceptor;
            std::thread th_context;
            bool NON_BLOCK_MODE = true;
        };

    }

}
#endif