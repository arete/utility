
// -*- c++ -*-

#ifndef UTILITY__LOGGER_TMPL__
#error "This file is included by Logger.hh"
#error "Include that file instead."
#endif


namespace Utility
{

  inline bool BasicLogDevice::TimeStamp ()
  {
    if (last_device != this)
      {
	last_device = this;
	return true;
      }

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
    //    if (LDEVC::print_log_prelude)
    //     PrintPrelude (*log_prelude);
  };

  template <typename LDEVC>
  void LogDevice<LDEVC>::NoLogFile ()
  {
    if (log_file)
      DeallocateSplitStreams ();
    log_file = &logfile;
  }


  template <typename LDEVC> template <typename LDESC>
  std::ostream& LogDevice<LDEVC>::SplitLog ()
  {
    if (LDESC::echo_log_std_out || (!log_file))
      {
	if (LDESC::log_to_file && log_file)
	  return *split_stream_cout;
	else
	  return cout;
      }
    else if (LDESC::log_to_file)
      return *log_file;
  }

  template <typename LDESC>
  std::ostream& SplitWarn ();



  template <typename LDEVC>
  void LogDevice<LDEVC>::AllocateSplitStreams ()
  {
    split_buffer_cout = new SplitStreamBuffer (cout, *log_file);
    split_buffer_cerr = new SplitStreamBuffer (cerr, *log_file);
    split_stream_cout = new std::ostream (split_buffer_cout);
    split_stream_cerr = new std::ostream (split_buffer_cerr);
  }

  
  template <typename LDEVC>
  void LogDevice<LDEVC>::DeallocateSplitStreams ()
  {
    delete split_stream_cout;
    delete split_stream_cerr;
    delete split_buffer_cout;
    delete split_buffer_cerr;
  }

} // end namespace Utility
