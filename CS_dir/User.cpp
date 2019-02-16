#include "User.hpp"
#include <algorithm>
#include <iostream>

User::User() : _id(86500), _passwd("aaaabbbb"), _logged_in(true) {}
User::User(int id, const std::string& passwd) : _id(id), _passwd(passwd), _logged_in(true) {}
User::~User() {}

    /* getters */
bool User::validate_passwd(const std::string& passwd) {
    return _passwd == passwd;
}

void User::add_backup(std::string dirname, BServer* server) {
    _backups.insert(std::make_pair(dirname, server));
    _dirnames.push_back(dirname);
}

std::vector<std::string> User::get_dirs() const{
    return _dirnames;
}
void User::remove_dir(const std::string &dirname) {
    int index = std::find(_dirnames.begin(), _dirnames.end(), dirname) - _dirnames.begin();

    if(index < (int)_dirnames.size()) {
        _dirnames.erase(_dirnames.begin() + index);
    } else {
        std::cout << "Directory not found" << std::endl;
        return;
    }

    _backups.erase(dirname);

}

int User::get_id() const{
    return _id;
}

std::string User::get_passwd() const{
    return _passwd;
}

void User::login() {
    _logged_in = true;
}

void User::logout() {
    _logged_in = false;
}

bool User::is_logged_in() const{
    return _logged_in;
}

std::map<std::string, BServer* > User::get_backups() const{
    return _backups;
}

BServer* User::checkDir(std::string name){
  std::map<std::string,BServer*>::iterator i;
  i = _backups.find(name);
  if(i != _backups.end()){
    return i->second;
  }
  return nullptr;
}
bool User::checkServer(BServer* b){
  for(auto pair:_backups){
    if(pair.second==b){
      return true;
    }
  }
  return false;
}
