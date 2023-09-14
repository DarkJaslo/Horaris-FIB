#include "HTTPSGetter.hh"
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "../lib/httplib.h"

#include <fstream>
#include <iostream>
using namespace jaslo;

bool HTTPSGetter::get(const std::string& serverUrl, const std::string& path, const std::string& outputFilename)
{
  httplib::Client cli(serverUrl);

  if(auto res = cli.Get(path))
  {
    if(res->status == 200)
    {
      std::fstream file;
      file.open(outputFilename,std::ios::out | std::ios::trunc);
      if(file.is_open())
      {
        replaceQuotations(res->body);
        file << res->body;
        file.close();
        return true;
      }
      else{
        std::cerr << "File " << outputFilename << " is not open" << std::endl;
      }
    }
    else{
      std::cerr << "GET request failed with code " << res->status << std::endl;
    }
  }
  else{
    auto err = res.error();
    std::cerr << "HTTP error: " << httplib::to_string(err) << std::endl;
  }

  return false;
}

void HTTPSGetter::replaceQuotations(std::string& str)
{
  size_t found = str.find("&quot;");

  while(found != std::string::npos)
  {
    str.replace(found,6,"\"");
    found = str.find("&quot;",found+1);
  }
}