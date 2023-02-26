// This file contains some common helper function there are required by netlib
#pragma once
#include <filesystem>
#include "SocketServer.h"
#ifndef BLOCK_SIZE 

#define BLOCK_SIZE 1024 
#endif
unsigned long getFileSize(std::string filename);

bool writeFileToSocket(shared_ptr<darshan::network::SocketConnection> socket, std::string filename);