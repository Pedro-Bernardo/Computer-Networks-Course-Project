#ifndef BServer_h
#define BServer_h

#include <string>

class BServer {
private:
    std::string         _ip;
    int                 _port;
    unsigned long int   _count;
    bool                _available;
public:
    BServer();
    BServer(const std::string& ip, const int _port);
    ~BServer();

    /* getters */
    std::string get_ip() const;
    int get_port()const;
    unsigned long int get_count()const;
    bool is_available()const;
    friend std::istream& operator>> ( std::istream& stream, BServer& server );
    void available();
    void unavailable();
    /* setters */
    void inc_count();

};
#endif /* BServer_h */
