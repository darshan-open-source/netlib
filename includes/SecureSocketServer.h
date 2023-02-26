#ifndef CORE_SERVER
#define CORE_SERVER
#define ASIO_STANDALONE
#include <iostream>
#include <asio.hpp>
#include <asio/ssl.hpp>
#include <thread>
#include <vector>
#include <memory>
using namespace std;
using namespace asio;
using namespace asio::ip;
#ifndef CERTIFICATE_FILE
#define CERTIFICATE_FILE "/home/darshan/Documents/code/netlib(main)/certificates/certificate.pem"
#endif

#ifndef PRIVATEKEY_FILE
#define PRIVATEKEY_FILE "/home/darshan/Documents/code/netlib(main)/certificates/key.pem"
#endif
typedef enum _WAIT_TYPE
{
    wait_read = socket_base::wait_read,
    wait_write = socket_base::wait_write,
    wait_error = socket_base::wait_error
} wait_type;

namespace darshan
{
    namespace network
    {

        class SocketConnection : public std::enable_shared_from_this<SocketConnection>
        {
        public:
            SocketConnection(asio::ssl::stream<tcp::socket> socket);

            size_t readUntilCRLF(std::string *s);

            void close();

            auto getSocket();

            size_t avalible();

            bool isConnected();
            size_t writeString(std::string s);

            size_t writeString(std::string s, int length);
            size_t readData(char *data, int length);

            size_t writeData(char *data, int length);

            void shutdownRead();

            void shutdownWrite();

            void shutdownBoth();

            void waitRead();

            void waitWrite();
            shared_ptr<asio::ssl::stream<tcp::socket>> ptr;

        private:
            void do_handshake();
        };
        class SocketServer
        {
        public:
            io_context io;
            SocketServer(char *host, int port);

            asio::ip::tcp::socket::native_handle_type getServerSocket();
            int getPort();
            std::string getAddress();
            virtual void onClientConnected(shared_ptr<SocketConnection>);
            void wait();

            void waitForConnection();

            bool start();
            void stop();
            ~SocketServer();

        private:
            tcp::acceptor acceptor;
            std::thread th_context;
            bool NON_BLOCK_MODE = true;
            asio::ssl::context context_;
        };

    }

}
#endif