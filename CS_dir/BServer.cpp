#include "BServer.hpp"

BServer::BServer() : _count(0) {}
BServer::BServer(const std::string& ip, const int port) : _ip(ip), _port(port), _count(0),_available(true) {}

BServer::~BServer() { /* do nothing */ }

/* getters */
std::string BServer::get_ip() const{
    return _ip;
}

int BServer::get_port() const{
    return _port;
}

unsigned long int BServer::get_count() const{
    return _count;

}

/* setters */
void BServer::inc_count() {
    _count++;
}
bool BServer::is_available() const{
  return _available;
}
void BServer::available(){
  _available=true;
}
void BServer::unavailable(){
  _available=false;
}
