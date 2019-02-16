#include "../Server/TCPServer.hpp"
#include "BServer.hpp"
#include "../Server/UDPServer.hpp"
#include "User.hpp"
#include <string>

void add_bserver(BServer* server);
void update_bserver_db();
void load_bservers();
bool compare_count(const BServer* first, const BServer* second);
void write_user();	
void read_user(const std::string& fname);
BServer* get_bserver(const std::string& ip, int port);

std::ostream& operator<< ( std::ostream& stream, const BServer& server );
std::istream& operator>> ( std::istream& stream, BServer& server );
std::ostream& operator<< ( std::ostream& stream, const User& usr );
std::istream& operator>> ( std::istream& stream, User& usr );