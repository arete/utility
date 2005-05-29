//#include<pstream>

#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <iostream>

class processbuf : public std::streambuf
{
protected:

  int pid;
  int sink[2];
  int source[2];

  /* output */
  virtual int_type overflow (int_type c) {
	if (c != EOF) {
		char cc = c;
		if (write (sink[1], &cc, 1) != 1)
			return EOF;
	}
	return EOF;
  }
  virtual std::streamsize xsputn (const char* s, std::streamsize num) {
	return write (sink[1], s, num);
  }

  /* input */
  char buffer [2];
  virtual int_type underflow () {
	if (gptr() < egptr())
		return *gptr();


	int numPutback = gptr() - eback();
	if (numPutback > 1)
		numPutback = 1;

	buffer[0] = buffer[1];

	int num = read (source[0], &buffer[1], 1);
	if (num <= 0)
		return EOF;

	setg (buffer + 1 - numPutback, buffer + 1, buffer + 1 + num);

	return *gptr();
  }

public:
  processbuf (const char* file, char* const argv[])
  {
    setg (buffer+1, buffer+1, buffer+1);

    pipe (sink);
    pipe (source);
    if ( (pid = fork()) == 0) {
      dup2(sink[0],0); close(sink[0]); close(sink[1]);
      dup2(source[1],1); close(source[0]); close(source[1]);
      execvp(file, argv);
      perror(file);
      _exit(1);
    }
    close(source[1]); close(sink[0]);
  }

  ~processbuf () {
    close(source[0]); close(sink[1]);
    waitpid(pid, NULL, 0);
  }
  
};

class postream : public std::ostream
{
public:
  postream (const char* file, char* const argv[])
  : std::ostream(&buf) , buf(file, argv){
  }

protected:
  processbuf buf;
};

class pistream : public std::istream
{
public:
  pistream (const char* file, char* const argv[])
  : std::istream(&buf) , buf(file, argv) {
  }

protected:
  processbuf buf;
};


class pstream : public std::iostream
{
public:
  pstream (const char* file, char* const argv[])
  : std::iostream(&buf) , buf(file, argv) {
  }

protected:
  processbuf buf;
};


int main ()
{
	pistream cat ("cat", (char*[]){"cat", "/proc/cpuinfo", 0} );
	pstream sed ("sed", (char*[]){"sed", "s/World/C++/", 0} );

	cat >> std::cout.rdbuf();
	sed << "Hello World!" << std::endl;
	sed >> std::cout.rdbuf();
}

