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
        

            SocketConnection(asio::ssl::stream<tcp::socket> socket)
            {
                ptr = make_shared<asio::ssl::stream<tcp::socket>>(std::move(socket));
                do_handshake();
            };

            size_t readUntilCRLF(std::string *s)
            {

                return read_until(*(ptr.get()), asio::dynamic_buffer(*s), "\r\n");
            }

            void close()
            {
                ptr->lowest_layer().close();
            }
            // auto getSocket()
            // {
            //     return std::ref(*ptr.get());
            // }
            
            auto getSocket()
            {
        
                return std::ref(ptr.get()->lowest_layer());

            }

            size_t avalible()
            {
                if (isConnected())
                {
                    try
                    {
                        return ptr.get()->lowest_layer().available();
                    }
                    catch (std::exception e)
                    {
                        cout << "\n error while readind data" << e.what();
                        return -1;
                    }
                }
                return -1;
            }
            bool isConnected() { return ptr.get()->lowest_layer().is_open(); };
            // void wait(wait_type w)
            // {
            //     if (isConnected())
            //     {
            //         try
            //         {
            //             ptr.get()->lowest_layer().wait((socket_base::wait_type)w);
            //         }
            //         catch (std::exception e)
            //         {
            //             cout << "\n error while waiting " << e.what();
            //         }
            //     }
            // }
            size_t writeString(std::string s)
            {
                if (isConnected())
                {
                    
                    try
                    {
                        return ptr.get()->write_some(asio::buffer(s.c_str(), s.length()));
                    }
                    catch (std::exception &e)
                    {
                        cout << "\n error while writing  data" << e.what();
                        return -1;
                    }
                }
                else
                    return -1;
            }
            size_t writeString(std::string s, int length)
            {
                if (isConnected())
                {
                    try
                    {
                        return ptr.get()->write_some(asio::buffer(s.c_str(), length));
                    }
                    catch (std::exception e)
                    {
                        cout << "\n error while writing data" << e.what();
                        return -1;
                    }
                }
            }
            // size_t readString(std::string *s, int length)
            // {
            //     if (isConnected())
            //     {
            //         try
            //         {
            //             char *m = (char *)malloc(length + 1);
            //             int y = ptr.get()->read_some(asio::buffer(m, length));
            //             std::string sf(m, y);
            //             s->assign(sf);
            //             free(m);
            //             sf.clear();
            //             // delete& sf;
            //             return y;
            //         }
            //         catch (std::exception e)
            //         {
            //             cout << "\n error while reading data" << e.what();
            //             return -1;
            //         }
            //     }
            //     else
            //         return -1;
            // }
            size_t readData(char *data, int length)
            {

                if (isConnected())
                {
                    try
                    {

                        error_code ec;
                        size_t t = asio::read(*(ptr.get()), buffer(data, length), ec);
                        if(!ec)return t;
                        return -1;
                    }
                    catch (std::exception &e)
                    {
                        return -1;
                    }
                }
                else
                {
                    return -1;
                }
            }
            size_t writeData(char *data, int length)
            {
                if (isConnected())
                {
                    try
                    {
                        return ptr.get()->write_some(buffer(data, length));
                    }
                    catch (std::exception e)
                    {
                        return -1;
                    }
                }
                else
                    return -1;
            }
            void shutdownRead()
            {
                ptr->lowest_layer().shutdown(ptr->lowest_layer().shutdown_receive);
            }
            void shutdownWrite()
            {
                ptr->lowest_layer().shutdown(ptr->lowest_layer().shutdown_send);
            }
            void shutdownBoth()
            {
                ptr->lowest_layer().shutdown(ptr->lowest_layer().shutdown_both);
            }
            void waitRead()
            {
                ptr.get()->lowest_layer().wait(ptr->lowest_layer().wait_read);
            }
            void waitWrite()
            {
                ptr->lowest_layer().wait(ptr->lowest_layer().wait_write);
            }
            shared_ptr<asio::ssl::stream<tcp::socket>> ptr;

        private:
            void do_handshake()
            {

                // auto self(shared_from_this());
                //  socket_.async_handshake(asio::ssl::stream_base::server,
                //                          [this, self](const std::error_code &error) {

                //                         });
               try{
                ptr.get()->handshake(asio::ssl::stream_base::server);
               }
               catch(std::exception &e){
                cout << "error occured at handshak";
               }
            }
            char data[2000];

            vector<char> v;
        };
        class SocketServer
        {
        public:
            io_context io;
            SocketServer(char *host, int port) : acceptor(io, tcp::endpoint(address::from_string(host), port)), context_(asio::ssl::context::sslv23)
            {
                acceptor.non_blocking(NON_BLOCK_MODE);
                // context_.sect_options(
                //     asio::ssl::ontext::default_workarounds | asio::ssl::context::no_sslv2);
                //   context_.set_password_callback(std::bind(&server::get_password, this));
                context_.use_certificate_chain_file(CERTIFICATE_FILE);
                context_.use_private_key_file(PRIVATEKEY_FILE, asio::ssl::context::pem);
            }

            asio::ip::tcp::socket::native_handle_type getServerSocket() { return acceptor.native_handle(); };
            int getPort() { return acceptor.local_endpoint().port(); }
            std::string getAddress() { return acceptor.local_endpoint().address().to_string(); }
            virtual void onClientConnected(shared_ptr<SocketConnection>){};
            void wait()
            {

                if (th_context.joinable())
                    th_context.join();
            }
            void waitForConnection()
            {
                // cout << "\n waiting for connection";
                try
                {
                    acceptor.async_accept([this](const asio::error_code &ec, tcp::socket s)
                                          {
						if (!ec) {
						
							// shared_ptr<SocketConnection> spo = make_unique<SocketConnection>(s);
                     shared_ptr<SocketConnection> spo = std::make_shared<SocketConnection>(
                asio::ssl::stream<tcp::socket>(
                  std::move(s), context_));

							std::thread t(&SocketServer::onClientConnected, this, std::move(spo));

							t.detach();



                            // cout << "calld 2"<<endl;
						}
						else {

							printf("\n asynchronus accept error %s", ec.message().c_str());
						}

						waitForConnection(); });
                }
                catch (std::exception e)
                {
                    cout << endl
                         << "error at connection accepting" << e.what();
                }
            }
            bool start()
            {
                try
                {
                    waitForConnection();
                    th_context = std::thread([this]()
                                             { io.run(); });
                }
                catch (std::exception &e)
                {
                    printf("\n server start error : %s", e.what());
                    cout << endl;
                    return false;
                }
                return true;
            }
            void stop()
            {
                io.stop();
                if (th_context.joinable())
                    th_context.join();
            }
            ~SocketServer()
            {
                stop();
            }

        private:
            tcp::acceptor acceptor;
            std::thread th_context;
            bool NON_BLOCK_MODE = true;
            asio::ssl::context context_;
        };

    }

}
#endif