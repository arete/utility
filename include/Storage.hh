
#ifndef __STORAGE_HH__
#define __STORAGE_HH__

#include <string>
#include <vector>
#include <map>
#include <iostream>

class BasicStorage {
 public:
  
  virtual ~BasicStorage () {};
  
  virtual std::istream& Read (std::istream& is) = 0;
  virtual std::ostream& Write (std::ostream& os) = 0;

  std::string name;
};

template <typename T> 
class Storage : public BasicStorage {
 
 public:
  
  Storage (const std::string& i_name) {
    name = i_name;
  }

  Storage (const std::string& i_name, const T& i_value) {
    name = i_name;
    value = i_value;
  }
  
  virtual std::istream& Read (std::istream& is) {
    // TODO: error handling
    is >> value;
    return is;
  }
  
  virtual std::ostream& Write (std::ostream& os) {
    os << value;
    return os;
  }
  
  T value;
};

// some bool specialisations ...
std::istream& Storage<bool>::Read (std::istream& is) {
  // TODO: error handling
  is >> value;
  if (is.fail () ) {
    
    is.clear ();
    
    std::string s;
    is >> s;
    
    if (s == "true")
      value = true;
    else if (s == "false")
      value = false;
    else 
      std::cout << "Error: Unable to read boolean value for key \""
		<< name << "\"!";
  }
  return is;
}

class Serializer {

 public:

  void Add (BasicStorage* storage);
  
  std::istream& Read (std::istream& is, bool verbose = true);

  template <typename T>
  T Get (const std::string& name) {
    iterator it = content.find (name);
    
    if (it == content.end () ) {
      std::cout << "Warning: Storage " << name << " not available!"
		<< std::endl;
      return T ();
    }
    
    Storage<T>* s;
    s = dynamic_cast<Storage<T>*> (it->second);
    if (s)
      return s->value;
    else {
      std::cout << "Warning: Type mismatch for storage " << name << "!"
		<< std::endl;
      return T ();
    }
  }
  
 private:
  typedef std::map<std::string, BasicStorage*> container;
  typedef container::iterator iterator;
  
  container content;
};

#endif // __STORAGE__
