#ifndef HTTPSGetter_hh
#define HTTPSGetter_hh

#include <string>

namespace jaslo{

class HTTPSGetter{

public:

  //Gets path from serverUrl and outputs it to outputFilename
  static bool get(const std::string& serverUrl, const std::string& path, const std::string& outputFilename);

private:

  //Replaces every instance of '&quot;' in str with '"'
  static void replaceQuotations(std::string& str);

};


}; //Namespace end
#endif
