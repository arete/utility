
// -*- c++ -*-

#ifndef UTILITY__LOGGER_TMPL__
#error "This file is included by Logger.hh"
#error "Include that file instead."
#endif


namespace Utility
{

  inline bool BasicLogDevice::TimeStamp (direction d)
  {
    if (last_device != this)
      {
	last_device = this;
	last_direction = d;
	return true;
      }

    if (last_direction != d)
      {
	last_direction = d;
	return true;
      };

    return false;
  }


  template <typename LDEVC>
  LogDevice<LDEVC>::LogDevice (std::ofstream& logfile)
  {
    log_file = 0;
    SwitchLogFile (logfile);
  };


  template <typename LDEVC>
  LogDevice<LDEVC>::LogDevice ()
  {
    log_file = 0;
    NoLogFile ();
  };


  template <typename LDEVC>
  LogDevice<LDEVC>::~LogDevice ()
  {
    if (log_file)
      DeallocateSplitStreams ();  
  };


  template <typename LDEVC>
  void LogDevice<LDEVC>::SwitchLogFile (std::ofstream& logfile)
  {
    if (log_file)
      DeallocateSplitStreams ();
    log_file = &logfile;
    AllocateSplitStreams ();
    if (LDEVC::print_log_prelude)
      PrintPrelude ();
  };

  template <typename LDEVC>
  void LogDevice<LDEVC>::NoLogFile ()
  {
    if (log_file)
      DeallocateSplitStreams ();
    log_file = 0;
  }


  template <typename LDEVC>
  BasicLogDevice::direction LogDevice<LDEVC>::DetermineDirection (const bool to_std, const bool to_file)
  {
    if (to_std || (!log_file))
      {
	if (to_file && log_file)
	  return both;
	else // file only or no logging at all
	  return std;
      }
    else if (to_file)
      return file;

    return std; // no-log-at-all case : return std for savety.
  }


  template <typename LDEVC> template <typename LDESC>
  std::ostream& LogDevice<LDEVC>::SplitLog ()
  {
    direction d = DetermineDirection (LDESC::echo_log_stdout, LDESC::log_to_file);
    std::ostream* r_stream = 0;
    switch (d)
      {
      case std:
	r_stream = &wrap_cout;
	break;
      case both:
	r_stream = split_stream_cout;
	break;
      case file:
	r_stream = log_file;
      }

    if (LDEVC::do_timestamping)
      if (TimeStamp (d))
	PrintTimeStamp (r_stream);
    return *r_stream;
  }

  template <typename LDEVC> template <typename LDESC>
  std::ostream& LogDevice<LDEVC>::SplitWarn ()
  {
    direction d = DetermineDirection (LDESC::echo_warn_stderr, LDESC::warn_to_file);
    std::ostream* r_stream = 0;
    switch (d)
      {
      case std:
	r_stream = &wrap_cerr;
	break;
      case both:
	r_stream = split_stream_cerr;
	break;
      case file:
	r_stream = log_file;
      }

    if (LDEVC::do_timestamping)
      if (TimeStamp (d))
	PrintTimeStamp (r_stream);
    return *r_stream;
  }


  template <typename LDESC, typename LDEVC>
  LogDestination<LDESC, LDEVC>::LogDestination (std::string i_context, LogDevice <LDEVC>& i_device)
    : device (i_device)
  {
    context = i_context;
  }

  template <typename LDESC, typename LDEVC>
  std::ostream& LogDestination<LDESC, LDEVC>::Log ()
  {    
    std::ostream& r_stream = device.LogDevice<>::SplitLog<LDESC> ();
    if (LDESC::context_in_log)
      r_stream << context << ": ";
    return r_stream;
  }

  template <typename LDESC, typename LDEVC>
  std::ostream& LogDestination<LDESC, LDEVC>::Warn ()
  {    
    std::ostream& r_stream = device.LogDevice<>::SplitWarn<LDESC> ();
    if (LDESC::context_in_warn)
      r_stream << context << " warning : ";
    return r_stream;
  }


  template <typename LDESC, typename LDEVC, typename WL_TRAITS>
  Logger<LDESC, LDEVC, WL_TRAITS>::Logger(Destination& i_destination)
    : destination (i_destination)
  {
  }


  template <typename LDESC, typename LDEVC, typename WL_TRAITS>
  std::ostream& Logger<LDESC, LDEVC, WL_TRAITS>::Log (const char* pretty_function_name)
  {
    std::ostream& r_stream = destination.Log ();
    if (LDESC::function_names_in_log)
      r_stream << pretty_function_name << " ";
    return r_stream;
  }

  template <typename LDESC, typename LDEVC, typename WL_TRAITS>
  std::ostream& Logger<LDESC, LDEVC, WL_TRAITS>::Warn (const char* pretty_function_name)
  {
    std::ostream& r_stream = destination.Warn ();
    if (LDESC::function_names_in_warn)
      r_stream << pretty_function_name << " : "; 
    return r_stream;
  }

  template <typename LDESC, typename LDEVC, typename WL_TRAITS, typename OBJ>
  ObjectLogger<LDESC, LDEVC, WL_TRAITS, OBJ>::ObjectLogger(Destination& i_destination, OBJ* i_parent)
    : Logger<LDESC, LDEVC, WL_TRAITS> (i_destination)
  {
    parent = i_parent;
    Q_LOG((*this)) << "new " << FilteredTypeName <OBJ> () << std::endl;
  }

  template <typename LDESC, typename LDEVC, typename WL_TRAITS, typename OBJ>
  ObjectLogger<LDESC, LDEVC, WL_TRAITS, OBJ>::~ObjectLogger()
  {
    Q_LOG((*this)) << "deleted" << std::endl;
  }


  template <typename LDESC, typename LDEVC, typename WL_TRAITS, typename OBJ>
  std::ostream& ObjectLogger<LDESC, LDEVC, WL_TRAITS, OBJ>::Log (const char* pretty_function_name)
  {
    std::ostream& r_stream = Logger<LDESC, LDEVC, WL_TRAITS>::Log (pretty_function_name);
    r_stream << parent << " ";
    return r_stream;
  }

  template <typename LDESC, typename LDEVC, typename WL_TRAITS, typename OBJ>
  std::ostream& ObjectLogger<LDESC, LDEVC, WL_TRAITS, OBJ>::Warn (const char* pretty_function_name)
  {
    std::ostream& r_stream = Logger<LDESC, LDEVC, WL_TRAITS>::Warn (pretty_function_name);
    r_stream << parent << " ";
    return r_stream;
  }




} // end namespace Utility
