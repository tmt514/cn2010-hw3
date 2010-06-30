#include"log.h"
#include<cstdio>
#include<cstdarg>
LogWriter::LogWriter() {
  fp.clear();
  fp.push_back(stdout);
}
LogWriter::~LogWriter() {
  for (int i = 1; i < fp.size(); ++i)
    fclose(fp[i]);
}
LogWriter& LogWriter::operator+=(const char name[]) {
  FILE *f = fopen(name, "w");
  if (f != NULL)
    fp.push_back(f);
  return *this;
}
int LogWriter::printf(const char format[], ...) {
  va_list args;
  for (int i = 0; i < fp.size(); ++i) {
    va_start(args, format);
    vfprintf(fp[i], format, args);
    va_end(args);
    fflush(fp[i]);
  }
}

