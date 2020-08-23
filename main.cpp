#include "Spider.h"
int main(int argc, char* argv[]) {
  std::string url;
  std::cout << "请输入网址" << std::endl;
  std::cin >> url;
  std::cin.ignore();
  Spider spider;
  spider.SetUrl(url);
  spider.SaveToFile();
  return 0;
}