#include "User.hpp"
#include <algorithm>
#include <iostream>

User::User() : _id(86500), _passwd("aaaabbbb"){}
User::User(int id, const std::string& passwd) : _id(id), _passwd(passwd){}
User::~User() {}

    /* getters */
bool User::validate_passwd(const std::string& passwd) {
    return _passwd == passwd;
}

void User::add_backup(Dir *dir) {
    _dirs.push_back(dir);
}

std::vector<File *>* User::get_files(std::string dirname) const{
    for(Dir *d:_dirs){
      if(d->getName()==dirname){
        return (d->getFiles());
      }
    }
    return nullptr;
}
void User::remove_dir(Dir *dir) {
    int index = std::find(_dirs.begin(), _dirs.end(),dir) - _dirs.begin();

    if(index < (int)_dirs.size()) {
        _dirs.erase(_dirs.begin() + index);
    } else {
        std::cout << "Directory not found" << std::endl;
        return;
    }
}

int User::get_id() const{
    return _id;
}

std::string User::get_passwd() const{
    return _passwd;
}
