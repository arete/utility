#include "Logger.hh"

using namespace Utility;
using namespace std;

class stdoutLogDestinationConfig : public LogDestinationConfig
{
public:
  static const bool echo_log_stdout = true;
  static const bool log_to_file = false;
};

class bothLogDestinationsConfig : public LogDestinationConfig
{
public:
  static const bool echo_log_stdout = true;
};


typedef stdoutLogDestinationConfig SC;
typedef bothLogDestinationsConfig BC;
typedef LogDestinationConfig LC;


ofstream lfile ("test.log");
LogDevice <> logA (lfile);
LogDevice <> logB;

LogDestination <LogDestinationConfig, LogDeviceConfig> logDst ("test", logB);

class ObjWLog
{
public:
  ObjWLog ()
    : m_logger (logDst, this)
  { }

  ObjectLogger <LogDestinationConfig, LogDeviceConfig, WL_Verbose, ObjWLog> m_logger;
};


main ()
{

  logA.SplitLog <LC> () << "This should be file only" << endl;
  logA.SplitLog <SC> () << "This should be stdout only" << endl;
  logA.SplitLog <BC> () << "This should be both file and stdout" << endl;
  logA.SplitLog <BC> () << "There should be no timestamp..." << endl;
  logA.SplitLog <BC> () << "... here neither" << endl;
  logB.SplitLog <LC> () << "but here..." << endl;
  logB.SplitLog <BC> () << "stdout only" << endl;
  logB.SplitLog <SC> () << "stdout only" << endl;



  logA.NoLogFile ();
  logB.SwitchLogFile (lfile);

  logA.SplitLog <LC> () << "file (not realy)" << endl;
  logA.SplitLog <SC> () << "stdout" << endl;
  logA.SplitLog <BC> () << "both (means stdout)" << endl;
  logB.SplitLog <LC> () << "file" << endl;
  logB.SplitLog <BC> () << "both" << endl;
  logB.SplitLog <SC> () << "stdout" << endl;


 
  logDst.Log () << "logging" << endl;
  logDst.Warn () << "warning" << endl;

  Logger <LogDestinationConfig, LogDeviceConfig, WL_Verbose> logger (logDst);
  
  Q_LOG(logger) << "logger logging" << endl;
  Q_WARN(logger) << "logger warning" << endl;

  ObjWLog ol;
  Q_LOG(ol.m_logger) << "some logging" << endl;
  Q_WARN(ol.m_logger) << "some warning" << endl;
 
};
