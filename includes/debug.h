// #pragma once
// #include <iostream>
// #include <cstring>
// size_t fwritewithescape(FILE *f, char *data, size_t len)
// {

//    size_t x = len;
//    if (len > strlen(data))
//    {
//       x = strlen(data);
//    }
//    for (int i = 0; i < x; i++)
//    {
//       if (data[i] == '\r')
//       {
//          fwrite("\\r", 1, 2, f);
//       }
//       else if (data[i] == '\n')
//       {
//          fwrite("\\n", 1, 2, f);
//       }
//       else
//       {
//          fwrite(&data[i], 1, 1, f);
//       }
//    }

//    return x;
// }
