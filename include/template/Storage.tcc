
#if GCC_VERSION < 30400 || __INCLUDE_LEVEL__ == 1

template <> std::istream& Storage<bool>::Read (std::istream& is) {
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

#endif

