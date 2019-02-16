#ifndef File_h
#define File_h

#include <string>
#include <vector>

class File{
private:
    std::string _name;
    std::string _date;
    std::string _hour;
    std::string _size;


public:
    File(std::string name,std::string date,std::string hour,std::string size):_name(name),_date(date),_hour(hour),_size(size){};
    ~File(){}
    bool checkDifferences(std::string date,std::string hour){
      return (_date==date && _hour==hour);
    }
    std::string getFileName(){
      return _name;
    }
    std::string getFileDate(){
      return _date;
    }
    std::string getFileHour(){
      return _hour;
    }
    std::string getFileSize(){
      return _size;
    }
};
#endif /* User_h */
