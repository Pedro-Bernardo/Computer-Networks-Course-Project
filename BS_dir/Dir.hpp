#ifndef Dir_h
#define Dir_h

#include <string>
#include <vector>
#include "File.hpp"

class Dir {
private:
    std::string _name;
    std::vector<File*> _files;

public:
    Dir(std::string name):_name(name){};
    ~Dir(){
      for(File *f:_files){
        delete f;
      }
      _files.clear();
    }
    void addFile(std::string name,std::string date,std::string hour,std::string size){
      _files.push_back(new File(name,date,hour,size));
    }
    File* getFile(std::string name){
      for(File *f:_files){
        if(f->getFileName()==name){
          return f;
        }
      }
      return nullptr;
    }
    std::vector<File*>* getFiles(){
      return &_files;
    }
    std::string getName() const{
      return _name;
    }
    bool operator==(const Dir& d2) const{
      return (this->_name==d2._name);
    }

};


#endif /* Dir_h */
