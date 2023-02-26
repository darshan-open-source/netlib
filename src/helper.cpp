#include<helper.h>
unsigned long getFileSize(std::string filename)
{

    return std::filesystem::file_size(filename);
}

bool writeFileToSocket(shared_ptr<darshan::network::SocketConnection> socket, std::string filename)
{

    FILE *f = fopen(filename.c_str(), "r");
    if (f == 0)
    {
        return false;
    }

    char buf[BLOCK_SIZE];
    while (!feof(f))
    {

        size_t x = fread(buf, 1, BLOCK_SIZE, f);
        socket->writeData(buf,x);

        bzero(buf, BLOCK_SIZE);
    }
    fclose(f);
    return true;
}