#ifndef User_h
#define User_h

#include <string>
#include <map>
#include <vector>
#include "Dir.hpp"
class User {
private:
    int _id;
    std::string _passwd;
    std::vector<Dir*> _dirs;

public:
    User();
    User(int id, const std::string& passwd);
    ~User();

    /* getters */
    int get_id() const;
    std::string get_passwd() const;
    std::vector<File *>* get_files(std::string dirname) const;


    bool validate_passwd(const std::string& passwd);
    void add_backup(Dir *dir);

    void remove_dir(Dir *dir);

};
#endif /* User_h */
