#ifndef _TP1_CLIENT_H_
#define _TP1_CLIENT_H_

#include "../networking/Client.h"
#include <chrono>
#include <string>
#include <vector>
#include <set>

class TP3Client :
	public Client {
	std::chrono::high_resolution_clock::time_point begin_;
  std::string id_;

  static std::vector<int> factor(int n);

public:
  explicit TP3Client(const std::string &id);
	~TP3Client();

	virtual void run() override;
	virtual bool parseReceived(std::string message) override;
};

#endif