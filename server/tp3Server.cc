// tp1Server.cpp : Defines the entry point for the console application.
//

#include "TP3Server.h"
#include <fstream>
#include <sstream>

std::string TP3Server::doneToHTML(const std::unordered_map<std::string, std::pair<std::string, bool>> &map, const std::unordered_map<std::string, std::string> &sentmap) {
  std::stringstream ans;

  ans << "<html><body><h1>";
  ans << "<p>Done:</p>";
  doneMutex_.lock();
  for (auto it : map) {
    auto intervalAns = it.second;
    ans << "<p>";
    ans << it.first << " : " << intervalAns.first << " result: " << intervalAns.second << std::endl;
    ans << "</p>";
  }
  doneMutex_.unlock();
  ans << "<p>Processing:</p>";

  intervalMutex_.lock();
  for (auto it : sentmap) {
    if (map.find(it.first) == map.end()) {
      ans << "<p>";
      ans << it.first << ":" << it.second;
      ans << "</p>";
    }
  }
  intervalMutex_.unlock();

  ans << "</h1></body></html>\r\n\n";
  return ans.str();
}

TP3Server::TP3Server(const std::string& port) :Server(port) {}


TP3Server::~TP3Server() {}

void TP3Server::onAccept(std::shared_ptr<ClientSocket> client) {
  printf("Server: Client Accepted\n");
  std::string readLine;
  readLine = client->read();
  if (readLine.find("GET /") != readLine.npos &&
    readLine.find("HTTP/") != readLine.npos) {
    printf("Server:\n\n [\n%s\n]\n\n", readLine.c_str());

    
    bool ans = client->write(
      "HTTP/1.0 200 OK\nServer - localhost:27015\r\nContent - Length : 44\r\nConnection : close\r\nContent - Type : text/html\r\n\n" + doneToHTML(idToDoneIntervalMap_, idToInterval_));

    if (ans) {
      printf("Sent Response\n");
    } else {
      printf("Response Failed\n");
    }

  } else if (readLine.find("ASK ") != readLine.npos) {

    std::string hostName = client->getAddr();

    std::string clientID = readLine.substr(readLine.find("ASK ") + 4);

    intervalMutex_.lock();
    TestInterval myInterval = sent_;
    idToInterval_[clientID] = myInterval.toString();
    sent_.bmin_ += INTERVAL_WIDTH;
    sent_.bmax_ += INTERVAL_WIDTH;
    sent_.pmin_ += INTERVAL_WIDTH;
    sent_.pmax_ += INTERVAL_WIDTH;
    intervalMutex_.unlock();

    std::string limitsMsg = myInterval.toString();
    client->write(limitsMsg);
    std::string confirmationMsg = client->read();

    std::string resultMsg = client->read();
    bool ans;
    if (resultMsg.find("CounterExample") != resultMsg.npos) {
      //got a counter Example
      ans = true;
    } else {
      ans = false;
    }

    doneMutex_.lock();
    idToDoneIntervalMap_[clientID] = std::make_pair(limitsMsg, ans);
    doneMutex_.unlock();

    char resultString[6];
    if (ans) {
      strcpy(resultString, "true");
    } else {
      strcpy(resultString, "false");
    }
    printf("Received Results of ~client[%s]status, Found Counter?: [%s]\n", clientID.c_str(), resultString);
  }
}

bool TP3Server::parseReceived(const std::string &msg, ClientSocket &client, std::string &hostName, std::ofstream& listFile, bool& isReady, bool &dirReceived) {
  return true;
}