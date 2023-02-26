#include <SocketServer.h>

namespace darshan
{
    namespace network
    {

        SocketConnection::SocketConnection(asio::ip::tcp::socket &s)
        {
            ptr = make_shared<asio::ip::tcp::socket>(move(s));
        };

        SocketConnection::~SocketConnection()
        {
            ptr->close();
        }

        size_t SocketConnection::readUntilCRLF(std::string *s)
        {

            return asio::read_until(getsockref(), asio::dynamic_buffer(*s), "\r\n");
        }

        void SocketConnection::close()
        {
            ptr.get()->close();
        }

        auto SocketConnection::getSocket()
        {
            return std::ref(*ptr.get());
        }

        asio::ip::tcp::socket &SocketConnection::getsockref()
        {

            return *ptr.get();
        }

        size_t SocketConnection::avaliable()
        {
            if (isConnected())
            {
                try
                {
                    return ptr.get()->available();
                }
                catch (std::exception e)
                {
                    return -1;
                }
            }
            return -1;
        }

        bool SocketConnection::isConnected() { return ptr.get()->is_open(); };

        void SocketConnection::wait(wait_type w)
        {
            if (isConnected())
            {
                try
                {
                    ptr.get()->wait((asio::socket_base::wait_type)w);
                }
                catch (std::exception e)
                {
                    cout << "\n error while waiting " << e.what();
                }
            }
        }

        size_t SocketConnection::writeString(std::string s)
        {
            if (isConnected())
            {
                try
                {
                    return ptr->write_some(asio::buffer(s.c_str(), s.length()));
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

        size_t SocketConnection::writeString(std::string s, size_t length)
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
            return -1;
        }

        size_t SocketConnection::readString(std::string *s, size_t length)
        {
            if (isConnected())
            {
                try
                {
                    char *m = (char *)malloc(length + 1);
                    int y = ptr.get()->read_some(asio::buffer(m, length));
                    std::string sf(m, y);
                    s->assign(sf);
                    free(m);
                    sf.clear();
                    // delete& sf;
                    return y;
                }
                catch (std::exception e)
                {
                    cout << "\n error while reading data" << e.what();
                    return -1;
                }
            }
            else
                return -1;
        }

        size_t SocketConnection::readData(char *data, size_t length)
        {

            if (isConnected())
            {
                try
                {
                    return asio::read(getSocket().get(), asio::buffer(data, length));
                }
                catch (std::exception &e)
                {
                    cout << "\n error while reading data" << e.what();
                    return 0;
                }
            }

            return 0;
        }

        size_t SocketConnection::writeData(char *data, size_t length)
        {
            if (isConnected())
            {
                try
                {
                    return ptr.get()->write_some(asio::buffer(data, length));
                }
                catch (std::exception e)
                {
                    cout << "\n error while writing data" << e.what();
                    return -1;
                }
            }

            return -1;
        }

        void SocketConnection::shutdownRead()
        {
            ptr.get()->shutdown(ptr->shutdown_receive);
        }

        void SocketConnection::shutdownWrite()
        {
            ptr.get()->shutdown(ptr->shutdown_send);
        }

        void SocketConnection::shutdownBoth()
        {
            ptr.get()->shutdown(ptr->shutdown_both);
        }

        void SocketConnection::waitRead()
        {
            ptr.get()->wait(ptr->wait_read);
        }

        void SocketConnection::waitWrite()
        {
            ptr.get()->wait(ptr->wait_write);
        }

        SocketServer::SocketServer(char *host, int port) : acceptor(io, asio::ip::tcp::endpoint(asio::ip::address::from_string(host), port))
        {
            acceptor.non_blocking(NON_BLOCK_MODE);
        }

        asio::ip::tcp::socket::native_handle_type SocketServer::getServerSocket() { return acceptor.native_handle(); };

        int SocketServer::getPort() { return acceptor.local_endpoint().port(); }

        std::string SocketServer::getAddress() { return acceptor.local_endpoint().address().to_string(); }

        void SocketServer::onClientConnected(shared_ptr<SocketConnection>){};

        void SocketServer::runServer()
        {

            if (th_context.joinable())
                th_context.join();
        }

        void SocketServer::waitForConnection()
        {

            try
            {
                acceptor.async_accept([this](const asio::error_code &ec, asio::ip::tcp::socket s)
                                      {
                        if (!ec) {
#ifdef DEBUG
                            cout <<"[+] Someone is connected from "<<s.remote_endpoint().address().to_string()<<":"<<to_string(s.remote_endpoint().port())<<endl;
#endif
                            shared_ptr <SocketConnection> spo = make_shared<SocketConnection>(s);
                            std::thread t(&SocketServer::onClientConnected, this, std::move(spo));
                            t.detach();
                        } else {

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

        bool SocketServer::startServer()
        {
            try
            {
                waitForConnection();
                th_context = std::thread([this]()
                                         { io.run(); });
            }
            catch (std::exception &e)
            {
                return false;
            }

            return true;
        }

        void SocketServer::stopServer()
        {
            io.stop();
            if (th_context.joinable())
                th_context.join();
#ifdef DEBUG
            cout << "[+] Server stopped";
#endif
        }

        SocketServer::~SocketServer()
        {
            stopServer();
        }

    }

}
