#ifndef UTILITY__LOGGER_HH__
#define UTILITY__LOGGER_HH__

#include "SplitStreamBuffer.hh"
#include <fstream>
#include <string>

namespace Utility
{
  class LogDeviceConfig
  {
  public:
    static const bool disabled = false;                   // no logging on this device at all
    static const bool thread_save = false;                // not supported yet...
    static const bool do_timestamping = true;             // print nummeric timestamps after writing to a different logdevice
    static const bool print_log_prelude = true;           // print time and date when creating/switching logfile
    //TODO:
    static const bool output_logfile_acces_errors = true; // print message to stderr on write errors
    static const bool redirect_to_stdout_on_error = true; // redirect all content to stdout after printing error message
  };


  class LogDestinationConfig
  {
  public:
    static const bool echo_log_stdout = false;            // log output will be branched to stdout
    static const bool log_to_file = true;                 // log output will be branched into file
    static const bool function_names_in_log = false;      // log messages will be prefixed with pretty function name
    static const bool context_in_log = false;             // log messages will be prefixed with context string

    static const bool echo_warn_stderr = true;            // warn options in analogy to log options
    static const bool warn_to_file = true;
    static const bool function_names_in_warn = true;
    static const bool context_in_warn = true;
  };


  class BasicLogDevice
  {
  protected:
    typedef u_int64_t timestamp_t;
    typedef enum {std,file,both} direction;

    static timestamp_t current;
    static direction last_direction;
    static BasicLogDevice* last_device;
    

    BasicLogDevice ();

    bool TimeStamp (direction d);
    void PrintPrelude ();
    void PrintTimeStamp (std::ostream* str);

    void AllocateSplitStreams ();
    void DeallocateSplitStreams ();

    SplitStreamBuffer* split_buffer_cout;
    SplitStreamBuffer* split_buffer_cerr;
    
    std::ostream* split_stream_cout;
    std::ostream* split_stream_cerr;

    std::ofstream* log_file;
  };


  template <typename LDEVC = LogDeviceConfig>
  class LogDevice : public BasicLogDevice
  {
  public:
    typedef LDEVC Config;

    LogDevice (std::ofstream& logfile);
    LogDevice ();
    ~LogDevice ();

    void SwitchLogFile (std::ofstream& logfile);
    void NoLogFile ();

    template <typename LDESC>
    std::ostream& SplitLog ();

    template <typename LDESC>
    std::ostream& SplitWarn ();

  protected:
    direction DetermineDirection (const bool to_std, const bool to_file);
  };


  template <typename LDESC, typename LDEVC>
  class LogDestination
  {
  public:
    typedef LogDevice <LDEVC> Device;
    typedef typename LogDevice <LDEVC>::Config DeviceConfig;

    LogDestination (std::string i_context, LogDevice <LDEVC>& i_device);

    inline bool DoLog ()  {return (!LDEVC::disabled) && (LDESC::echo_log_stdout || LDESC::log_to_file);}
    inline bool DoWarn () {return (!LDEVC::disabled) && (LDESC::echo_warn_stderr || LDESC::warn_to_file);}

    std::ostream& Log ();

    std::ostream& Warn ();

  protected:
    std::string context;
    Device& device;
  };


  template <typename LDESC, typename LDEVC, typename WL_TRAITS>
  class Logger
  {
  public:
    Logger (LogDestination <LDESC, LDEVC>& i_destination);
  
    inline bool DoLog () {return destination.DoLog () && WL_TRAITS::log; }
    inline bool DoWarn () {return destination.DoLog () && WL_TRAITS::warn; }

    std::ostream& Log (const char* pretty_function_name);
    std::ostream& Warn (const char* pretty_function_name);
  };


  template <typename LDESC, typename LDEVC, typename WL_TRAITS, typename OBJ>
  class ObjectLogger : public Logger <LDESC, LDEVC, WL_TRAITS>
  { 
  public:
    ObjectLogger (LogDestination <LDESC, LDEVC>& i_destination, OBJ* parent);

    std::ostream& Log (const char* pretty_function_name);
    std::ostream& Warn (const char* pretty_function_name);
  };


  // we export cout and cerr to avoid <iostream> inclusion
  extern std::ostream& wrap_cout;
  extern std::ostream& wrap_cerr;

} // end namespace Utility


// now we do some nasty preprocessor stuff...

#ifdef Q_LOG
#  ifndef UTILITY__CUSTOM_Q_LOG__
#    warning "the Q_LOG macro is already defined"
#  endif
#else
#  define Q_LOG(logger)    if (logger.DoLog ()) logger.Log (__PRETTY_FUNCTION__)
#endif

#ifdef Q_WARN
#  ifndef UTILITY__CUSTOM_Q_WARN__
#    warning "the Q_WARN macro is already defined"
#  endif
#else
#  define Q_WARN(logger)   if (logger.DoWarn ()) logger.Warn (__PRETTY_FUNCION__)
#endif

#define UTILITY__LOGGER_TMPL__
#include "template/Logger.tcc"
#undef UTILITY__LOGGER_TMPL__

#endif // UTILITY__LOGGER_HH__

