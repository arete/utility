
#ifndef __ARGUMENTLIST_HH__
#define __ARGUMENTLIST_HH__

#include <iostream>

#include <map>
#include <vector>

class BasicArgument {
 public:
  
  BasicArgument (const std::string& i_sname, const std::string& i_lname,
		 const std::string& i_desc,  int i_min_count, int i_max_count);
  
  virtual ~BasicArgument ();
  
  virtual bool Read () = 0;
  virtual bool Read (const std::string& arg) = 0;
  
  std::string sname;
  std::string lname;
  std::string desc;
  
  bool needs_arg;
  int min_count, max_count;
  int count;
};

template <typename T> 
class Argument : public BasicArgument {
 
 public:
  
  Argument (const std::string& i_sname, const std::string& i_lname,
	    const std::string& i_desc,
	    int i_min_count = 0, int i_max_count = 0) 
    : BasicArgument (i_sname, i_lname, i_desc, min_count, max_count)
  {
    // if bool (or other special option) always containing one value
    if (!needs_arg)
      values.push_back (T () );
  }

  Argument (const std::string& i_sname, const std::string& i_lname,
	    const std::string& i_desc, 
	    const T& i_value, int min_count = 0, int max_count = 0)
    : BasicArgument (i_sname, i_lname, i_desc, min_count, max_count)
  {
    values.push_back (i_value);
    // if bool (or other special option) always containing one value
    if (!needs_arg)
      values.push_back (T () );
  }
  
  bool Read () {
    std::cout << "Error: Option " << lname << " needs an argument!" << std::endl;
    return false;
  }
  
  bool Read (const std::string& arg) {
    if (count >= max_count) {
      std::cout << "Error: Too many arguments for option " << lname
		<< ", only " << max_count << " allowed!" << std::endl;
      return false;
    }
    
    std::stringstream stream (arg);
    // TODO: error handling
    T value;
    stream >> value;
    
    // special case if default was supplied -> overwrite the first ...
    if (count == 0 & values.size () > 0)
      values[0] = value;
    else
      values.push_back (value);
    
    ++count;
    
    return true;
  }
  
  std::vector<T> values;
};

// some bool specialisations ...

bool Argument<bool>::Read () {
  // the ctor garuantees we have one value!
  values[0] = true;
  ++ count;
  return true;
}

bool Argument<bool>::Read (const std::string& arg) {
  std::cout << "Error: No argument allowed for boolean values!" << std::endl;
  return false;
}

class ArgumentList {

 public:

  void Add (BasicArgument* arg);
  bool Read (int argc, char** argv);
  
  void Useage (const std::ostream& os);
  
  template <typename T>
  T Get (const std::string& name, unsigned int i = 0) {
    
    iterator it = long_content.find (name);
    
    if (it == long_content.end () ) {
      it = short_content.find (name);
      if (it == short_content.end () ) {
	std::cout << "Warning: Argument " << name << " not available!"
		  << std::endl;
	return T ();
      }
    }
    
    Argument<T>* s;
    s = dynamic_cast<Argument<T>*> (it->second);
    if (s) {
      if (s->values.size () > i) {
	return s->values [i];
      }
      else
	std::cout << "Error: There is no value " << i << "present for argument "
		  << name << std::endl;
    }
    else
      std::cout << "Error: Type mismatch for argument " << name << "!"
		<< std::endl;
    return T ();
  }
  
 private:
  typedef std::map<std::string, BasicArgument*> container;
  typedef container::iterator iterator;
  
  container short_content;
  container long_content;
};


#endif // __ARGUMENTLIST_HH__
