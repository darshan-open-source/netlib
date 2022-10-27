# 🚀 Netlib

Netlib is a free and open source header only c++ library for backend development inspired by Django

## Features

- Full SSL/TLS support

- Dynamic URL support

- Middlewares Feature

- High Performance  template engine

- Database support

- Multithreaded

- Super Fast

- Highly Portable

- Cross Plateform

- Open Source

## Third Party Dependency

- OpenSSL

- Asio

- libmysqlclient

- Curl

- rapidJSON

- inja Template engine

## Simple Usage

- First generate settings.h File

  settings.h file contains important setting of your website

```c++
#ifndef SETTINGS_INCLUDED
#define SETTINGS_INCLUDED

#define DEBUG    // for trace execution in development mode
#define USE_SSL  // for HTTPS

#define SESSION_DIRECTORY "/home/darshan/Documents/temp_test/temp_sessions/"
#define UPLOADED_FILES_DIRECTORY "/home/darshan/Documents/code/netlib(main)/temp_files/"
#define DEFAULT_STATIC_DIRECTORY "/home/darshan/Documents/code/netlib(main)/static/"
#define BUILTIN_STATIC_DIRECTORY "/home/darshan/Documents/code/netlib(main)/default_statics/"
#endif
```

- generate cpp file for your website

```c++
#include <iostream>
#include "includes/webserver.h"
#include "includes/public_directory.h"
using namespace std;

RESPONSE admin(httpRequest req, httpResponse res, void *)
{

    return res.HttpResponse("hello from admin");
}

int main()
{

    webserver s("127.0.0.1", 8085);

    s.onRequest("/", "index.html");
    s.onRequest("/admin", admin);

    s.start();
    s.wait();
}
```

## Build and Run The project

Build code with CMake and make

```bash
  cmake .
```

```bash
  make
```

Now executable is generated in present Directory

To run the the the code just simply execute this

```
./<executable name>
```
## Exmaples

For more exmaples go to [exmaples](https://github.com/darshan-open-source/netlib/tree/main/examples)

## Documentation

Documentation is yet not generated

we will provide Documentation as soon as possible

## IMPORTANT NOTES

This library is in development and developer  is continuously updating the code

This library may contain  vulnerbliityA

if anyone found any type of bug or want to suggest any feature contact us at darshanopensource@gmail.com

## Authors

- [@darshan-crypto](https://www.github.com/darshan-crypto)

## Support

For support, email darshanopensource@gmail.com

## License

[MIT](https://choosealicense.com/licenses/mit/)
