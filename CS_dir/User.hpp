#ifndef User_h
#define User_h

#include <string>
#include "BServer.hpp"
#include <map>
#include <vector>
class User {
private:
    int _id;
    std::string _passwd;
    std::map<std::string, BServer* > _backups;
    /* adicionar flag em relacao a quando sao alteradas as diretorias */
    std::vector<std::string> _dirnames;
    bool _logged_in;

public:
    User();
    User(int id, const std::string& passwd);
    ~User();

    /* getters */
    int get_id() const;
    std::string get_passwd() const;
    std::vector<std::string> get_dirs() const;
    std::map<std::string, BServer* > get_backups() const;


    bool validate_passwd(const std::string& passwd);
    void add_backup(std::string dirname , BServer* server);
    void login();
    void logout();
    bool is_logged_in() const;
    /* setters */
    BServer* checkDir(std::string name);
    bool checkServer(BServer* b);
    void remove_dir(const std::string& dirname);

};
#endif /* User_h */
