
#include "Storage.hh"
#include <sstream>

void Serializer::Add (BasicStorage* storage) {
  content [storage->name] = storage;
}

std::istream& Serializer::Read (std::istream& is, bool verbose) {
  char seperator = '=';
  
  std::vector<char> comments;
  comments.push_back (';');
  comments.push_back ('#');
    
  // TODO: error handling
  std::string line;
  while (std::getline (is, line)) {
      
    // strip comments
    for (std::vector<char>::iterator comment = comments.begin ();
	 comment != comments.end ();
	 ++comment)
      {
	std::string::size_type it = line.find (*comment);
	if (it != std::string::npos) {
	  line.erase (it, line.size () );
	}
      }
      
    // s.th. left to parse?
    if (line.size() > 0)
      {
	std::stringstream streamline (line);
	std::string key, sep, value;
	  
	streamline >> key >> sep;
	if (sep.size() == 1 && sep[0] == seperator) {
	    
	  std::map<std::string, BasicStorage*>::iterator it
	    = content.find (key);

	  if (it != content.end () ) {
	    it->second->Read(streamline);
	  }
	  else {
	    if (verbose)
	      std::cout << "Key " << key << " not registered!" << std::endl;
	  }
	}
	else {
	  if (verbose)
	    std::cout << "Parse error - no key found in line!" << std::endl;
	}
      }
  } // end while
    
    /* debug
       std::map<std::string, BasicStorage*>::iterator it;
       for (it = content.begin (); it != content.end(); ++it) {
       std::cout << "-> " << it->second->name << ": ";
       it->second->Write(std::cout) << std::endl;
       } */
  return is;
}
