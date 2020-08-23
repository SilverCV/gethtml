#include "Spider.h"

Spider::Spider() {
#ifdef WIN32
  WSADATA wsadata;
  if (WSAStartup(MAKEWORD(2, 2), &wsadata) < 0) {
    std::cerr << "fail to initializer the network environment" << std::endl;
    exit(-1);
  }
#endif
  port = 80;
  sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
  m_filepath = "./test.html";
}

Spider::Spider(std::string&& url) : m_url(url) { Spider(); }

Spider::~Spider() {
#ifdef WIN32
  closesocket(sockfd);
  if (WSACleanup() < 0) {
    std::cerr << "fail to clean up the network environment" << std::endl;
    exit(-1);
  }
#elif __linux__
  shutdown(sockfd, SHUT_RDWR);
#endif
}

void Spider::SetUrl(std::string const& url) { m_url = url; }

std::string Spider::getHtml(const std::string& url) {
  if (!url.empty()) {
    m_url = url;
  }
  std::string html;
  if (ParseUrl(false) == false && ParseUrl(true) == false) {
    std::cerr << "fail to parse url" << std::endl;
    return "";
  }
  Connect();

  std::string request = "GET " + m_resouce + " HTTP/1.1\r\nHost:" + m_host +
                        "\r\ncontent-type: text/html; charset=utf-8" +
                        "\r\nConnection: Close\r\n\r\n";

  if (send(sockfd, request.c_str(), request.size(), 0) < 0) {
    std::cerr << "fail to send command" << std::endl;
#ifdef WIN32
    closesocket(sockfd);
#elif __linux__
    shutdown(sockfd, SHUT_RDWR);
#endif
    return "";
  }
  char ch;
  while (recv(sockfd, &ch, 1, 0)) {
    html += ch;
  }
  return html;
}

void Spider::SaveToFile() {
  if (m_filepath.empty()) {
    std::cerr << "please set the file path " << std::endl;
    return;
  }
  std::fstream out(m_filepath, std::ios::out);
  std::string html = getHtml();
  out << html;
  out.close();
}

// get the ipaddress https or http
bool Spider::ParseUrl(bool IsSec) {
  if (m_url.empty()) {
    return false;
  }
  std::string url;
  std::string Https = IsSec ? "https://" : "http://";
  size_t offset = IsSec ? 8 : 7;
  if (std::string::npos == m_url.find(Https)) {
    return false;
  }
  if (IsSec && m_url.length() < 8) {
    return false;
  } else if (!IsSec && m_url.length() < 7) {
    return false;
  }
  size_t len = m_url.length();
  size_t pos = m_url.find("/", offset);
  if (pos == std::string::npos) {
    m_host = m_url.substr(offset);
    m_resouce = "/";
  } else {
    m_host = m_url.substr(offset, len - offset);
    m_resouce = m_url.substr(offset);
  }
  struct hostent* host = NULL;
  host = gethostbyname(m_host.c_str());
  m_ipaddr = inet_ntoa(*((struct in_addr*)host->h_addr_list[0]));
  return true;
}

void Spider::Connect() {
  struct sockaddr_in serverAddr;
  memset(&serverAddr, 0, sizeof(serverAddr));
  inet_pton(AF_INET, m_ipaddr.c_str(), &serverAddr.sin_addr);
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(80);

  if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
    std::cerr << "fail to connect" << std::endl;
    exit(-1);
  }
}
