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

main ()
{
  ofstream lfile ("test.log");

  LogDevice <> logA (lfile);
  LogDevice <> logB;

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


  LogDestination <LogDestinationConfig, LogDeviceConfig> log ("test", logB);
  log.Log () << "logging" << endl;
  log.Warn () << "warning" << endl;
};
