#ifndef _TP3_SERVER_H_
#define _TP3_SERVER_H_

#include "../networking/Server.h"
#include <mutex>
#include <unordered_map>

static const int INTERVAL_WIDTH = 8;

struct TestInterval {
  int bmin_ = 2, bmax_ = bmin_ + INTERVAL_WIDTH, pmin_ = 3, pmax_ = pmin_ + INTERVAL_WIDTH;
  std::string toString()const {
    return  std::to_string(bmin_) + ";" +
      std::to_string(bmax_) + ";" +
      std::to_string(pmin_) + ";" +
      std::to_string(pmax_) + ";";
  }
  void load(std::string msg) {
    auto pos = msg.find(";");
    auto token = msg.substr(0, pos);
    msg = msg.substr((pos + 1));
    bmin_ = std::stoi(token);

    pos = msg.find(";");
    token = msg.substr(0, pos);
    msg = msg.substr((pos + 1));
    bmax_ = std::stoi(token);

    pos = msg.find(";");
    token = msg.substr(0, pos);
    msg = msg.substr((pos + 1));
    pmin_ = std::stoi(token);

    pos = msg.find(";");
    token = msg.substr(0, pos);
    msg = msg.substr((pos + 1));
    pmax_ = std::stoi(token);

  }
  //bool operator <(TestInterval& t) {
  //  return (bmax_ * 1000 + bmin_ * 100 + pmax_ * 10 + pmin_)
  //    <
  //    (t.bmax_ * 1000 + t.bmin_ * 100 + t.pmax_ * 10 + t.pmin_);
  //}
  //bool operator ==(TestInterval& t) {
  //  return (bmax_ * 1000 + bmin_ * 100 + pmax_ * 10 + pmin_)
  //    ==
  //    (t.bmax_ * 1000 + t.bmin_ * 100 + t.pmax_ * 10 + t.pmin_);
  //}
};

class TP3Server :
  public Server {
  bool isReceiving_;
  
  std::mutex intervalMutex_;
  std::mutex doneMutex_;

  TestInterval sent_;

  std::unordered_map<std::string, std::string> idToInterval_;
  std::unordered_map<std::string, std::pair<std::string, bool>> idToDoneIntervalMap_;

  std::string doneToHTML(const std::unordered_map<std::string, std::pair<std::string, bool>> &map, const std::unordered_map<std::string, std::string> &sentmap);

  public:
  explicit TP3Server(const std::string& port);
  virtual ~TP3Server();

  virtual bool parseReceived(const std::string &msg, ClientSocket &client, std::string &hostName, std::ofstream& listFile, bool& isReady, bool &dirReceived);
  virtual void onAccept(std::shared_ptr<ClientSocket> client) override;
};

#endif