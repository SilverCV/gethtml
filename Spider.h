#ifndef SPIDER_H_
#define SPIDER_H
#ifdef WIN32
#include <WS2tcpip.h>
#include <WinSock2.h>
#elif __linux__
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/unistd.h>
#endif
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
class Spider {
 public:
  Spider();
  Spider(std::string&& url);
  virtual ~Spider();
  void SetUrl(std::string const& url);
  std::string getHtml(const std::string& url = "");
  void setFilePath(std::string&& filePath) { m_filepath = filePath; }
  void SaveToFile();

 private:
  bool ParseUrl(bool IsSec);
  void Connect();

 private:
#ifdef WIN32
  SOCKET sockfd;
#elif __linux__
  int sockfd;
#endif
  std::string m_url;
  std::string m_host;
  std::string m_resouce;
  std::string m_ipaddr;
  int port;
  std::string m_filepath;
};

#endif