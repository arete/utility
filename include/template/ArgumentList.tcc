
#if GCC_VERSION < 30400 || __INCLUDE_LEVEL__ == 1

template <> bool Argument<bool>::Read () {
  // the ctor garuantees we have one value!
  values[0] = true;
  ++ count;
  return true;
}

template <> bool Argument<bool>::Read (const std::string& arg) {
  std::cout << "Error: No argument allowed for boolean values!" << std::endl;
  return false;
}

#endif

