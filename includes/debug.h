#pragma once
#include<iostream>
#include<cstring>
size_t fwritewithescape(FILE *f,char* data,int len){

//    if(len > strlen(data)){
//     throw "er";
//    }
   //cout << data <<  endl;
   for(int i=0;i<len;i++){
     if(data[i] == '\r'){
       fwrite("\\r",1,2,f);
    }
    else if(data[i] == '\n'){
       fwrite("\\n",1,2,f);
    }
    else{
       fwrite(&data[i],1,1,f);
    }
   }

    return len;

}

template<typename t>
void printVector(std::vector<t> s){

cout <<endl << "printing vector-------";
for (size_t i = 0; i < s.size(); i++)
{
   cout << endl << s.at(i);
}

cout <<"\nvector ends------\n";

};