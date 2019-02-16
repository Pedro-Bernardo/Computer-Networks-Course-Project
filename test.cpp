#include <string>
#include <iostream>
#include "CS_dir/User.hpp"
#include "CS_dir/BServer.hpp"
#include <time.h>
#include <sstream>
#include <sys/stat.h>
using namespace std;

int main() {
  struct stat file;
	struct tm *t;
  char buffer[80];
  string request;
  stat("cmd_arguments.cpp",&file);
  t = gmtime(&(file.st_mtime));
  strftime (buffer,80,"%d.%m.%Y %X",t);
  request="cmd_arguments.cpp ";
  request+=buffer;
  request+=" "+to_string(file.st_size)+"\n";
  std::cout<<request<<std::endl;
  return 0;

}
