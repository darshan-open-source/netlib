// bug free
#ifndef CORE_SERVER
#define CORE_SERVER

#ifndef ASIO_STANDALONE
#define ASIO_STANDALONE
#endif

#include<iostream>
#include<asio.hpp>
#include<thread>
#include<vector>
#include<memory>

using namespace std;
using namespace asio;
using namespace asio::ip;
typedef enum _WAIT_TYPE {
    wait_read = socket_base::wait_read,
    wait_write = socket_base::wait_write,
    wait_error = socket_base::wait_error
} wait_type;

typedef std::function<
void(asio::ip::tcp::socket
sockets,
const error_code &ec,
void *m
)>
acceptorcallback;
namespace darshan {
    namespace network {

        class SocketConnection {
        public:

            SocketConnection(tcp::socket &s) {
                ptr = make_shared<tcp::socket>(move(s));
            };

            ~SocketConnection() {

            }


            size_t readUntilCRLF(std::string *s) {

                return read_until(getsockref(), asio::dynamic_buffer(*s), "\r\n");
            }

            void close() {
                ptr.get()->close();
            }

            auto getSocket() {
                return std::ref(*ptr.get());
            }

            tcp::socket &getsockref() {

                return *ptr.get();
            }


            size_t avalible() {
                if (isConnected()) {
                    try {
                        return ptr.get()->available();
                    }
                    catch (std::exception e) {
                        return -1;
                    }
                }
                return -1;
            }

            bool isConnected() { return ptr.get()->is_open(); };

            void wait(wait_type w) {
                if (isConnected()) {
                    try {
                        ptr.get()->wait((socket_base::wait_type) w);
                    }
                    catch (std::exception e) {
                        cout << "\n error while waiting " << e.what();

                    }
                }
            }

            size_t writeString(std::string s) {
                if (isConnected()) {
                    try {


                        return ptr->write_some(asio::buffer(s.c_str(), s.length()));
                    }
                    catch (std::exception &e) {
                        cout << "\n error while writing  data" << e.what();
                        return -1;
                    }
                } else return -1;
            }

            size_t writeString(std::string s, int length) {
                if (isConnected()) {
                    try {
                        return ptr.get()->write_some(asio::buffer(s.c_str(), length));
                    }
                    catch (std::exception e) {
                        cout << "\n error while writing data" << e.what();
                        return -1;
                    }
                }
            }

            size_t readString(std::string *s, int length) {
                if (isConnected()) {
                    try {
                        char *m = (char *) malloc(length + 1);
                        int y = ptr.get()->read_some(asio::buffer(m, length));
                        std::string sf(m, y);
                        s->assign(sf);
                        free(m);
                        sf.clear();
                        //delete& sf;
                        return y;
                    }
                    catch (std::exception e) {
                        cout << "\n error while reading data" << e.what();
                        return -1;
                    }

                } else return -1;

            }

            size_t readData(char *data, int length) {

                if (isConnected()) {
                    try {
                        return asio::read(getSocket().get(), buffer(data, length));

                    }
                    catch (std::exception &e) {
                        cout << "\n error while reading data" << e.what();
                        return 0;
                    }
                } else

                    return 0;
            }

            size_t writeData(char *data, int length) {
                if (isConnected()) {
                    try {
                        return ptr.get()->write_some(buffer(data, length));
                    }
                    catch (std::exception e) {
                        cout << "\n error while writing data" << e.what();
                        return -1;
                    }
                } else
                    return -1;
            }

            void shutdownRead() {
                ptr.get()->shutdown(ptr->shutdown_receive);
            }

            void shutdownWrite() {
                ptr.get()->shutdown(ptr->shutdown_send);
            }

            void shutdownBoth() {
                ptr.get()->shutdown(ptr->shutdown_both);
            }

            void waitRead() {
                ptr.get()->wait(ptr->wait_read);
            }

            void waitWrite() {
                ptr.get()->wait(ptr->wait_write);
            }

            shared_ptr <tcp::socket> ptr;



        };

        class SocketServer {
        public:

            io_context io;

            SocketServer(char *host, int port) : acceptor(io, tcp::endpoint(address::from_string(host), port)) {
                acceptor.non_blocking(NON_BLOCK_MODE);
            }

            asio::ip::tcp::socket::native_handle_type getServerSocket() { return acceptor.native_handle(); };

            int getPort() { return acceptor.local_endpoint().port(); }

            std::string getAddress() { return acceptor.local_endpoint().address().to_string(); }

            virtual void onClientConnected(shared_ptr <SocketConnection>) {};

            void wait() {

                if (th_context.joinable())th_context.join();
            }

            void waitForConnection() {

                try {
                    acceptor.async_accept([this](const asio::error_code &ec, tcp::socket s) {
                        if (!ec) {
                            #ifdef DEBUG
                            cout <<"[+] Someone is connected from "<<s.remote_endpoint().address().to_string()<<":"<<to_string(s.remote_endpoint().port())<<endl;
                            #endif
                            shared_ptr <SocketConnection> spo = make_unique<SocketConnection>(s);
                            std::thread t(&SocketServer::onClientConnected, this, std::move(spo));
                            t.detach();
                        } else {

                            printf("\n asynchronus accept error %s", ec.message().c_str());
                        }

                        waitForConnection();
                    });
                }
                catch (std::exception e) {
                    cout << endl << "error at connection accepting" << e.what();
                }

            }

            bool start() {
                try {
                    waitForConnection();
                    th_context = std::thread([this]() { io.run(); });

                }
                catch (std::exception &e) {
                    printf("\n server start error : %s", e.what());
                    cout << endl;
                    return false;
                }
               #ifdef DEBUG
               cout <<"[+] Server started at ";
               #ifdef USE_SSL
                cout << "https://";
               #endif
               #ifndef USE_SSL
                cout << "http://";
               #endif
                cout << getAddress()<<":"<<getPort()<<endl;
               #endif
                return true;
            }

            void stop() {
                io.stop();
                if (th_context.joinable())th_context.join();
                #ifdef DEBUG
                cout <<"[+] Server stopped";
                #endif

            }

            ~SocketServer() {
                stop();
            }

        private:
            tcp::acceptor acceptor;
            std::thread th_context;
            bool NON_BLOCK_MODE = true;
        };


    }

}
#endif