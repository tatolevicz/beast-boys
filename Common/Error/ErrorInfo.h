//
// Created by Arthur Motelevicz on 28/07/24.
//

#ifndef BEAST_BOYS_ERRORINFO_H
#define BEAST_BOYS_ERRORINFO_H

#include <string>

namespace bb
{
struct ErrorInfo
  {
  int errorCode;
  std::string errorMessage;
  std::string file;
  int line;

  ErrorInfo(int code, const std::string &message, const std::string &file, int line)
      : errorCode(code), errorMessage(message), file(file), line(line) {}
};
}


#endif //BEAST_BOYS_ERRORINFO_H
