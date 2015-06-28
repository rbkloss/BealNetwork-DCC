#include "tp3Client.h"
#ifndef __linux__
#else
#include <dirent.h>
#endif
#include <chrono>
#include <string>

#include "../server/TP3Server.h"
#include <algorithm>

extern bool experiments;


std::vector<int> TP3Client::factor(int n) {
  std::vector<int> ans;
  if (n % 2 == 0)ans.push_back(2);
  // Print the number of 2s that divide n
  while (n % 2 == 0) {
    n = n / 2;
    if (n == 0) {
      break;
    }
  }

  // n must be odd at this point.  So we can skip one element (Note i = i +2)
  int len = static_cast<int>(sqrt(n));
  for (int i = 3; i <= len; i = i + 2) {
    // While i divides n, print i and divide n
    if (n%i == 0)ans.push_back(i);
    while (n%i == 0) {
      n = n / i;
      if (n == 0) {
        break;
      }
    }
  }

  // This condition is to handle the case whien n is a prime number
  // greater than 2
  if (n > 2)
    ans.push_back(n);
  std::sort(ans.begin(), ans.end());
  return ans;
}

TP3Client::TP3Client(const std::string &id) {
  id_ = id;
}


TP3Client::~TP3Client() {}

bool TP3Client::parseReceived(std::string message) {
  return true;
}

void TP3Client::run() {
  sendMessage("ASK " + id_);
  begin_ = std::chrono::high_resolution_clock::now();

  std::string intervalMsg = socket_.read();
  TestInterval myInterval;
  myInterval.load(intervalMsg);
  socket_.write(myInterval.toString());
  std::string resultMsg("Results: ");
  for (int a = myInterval.bmin_; a < myInterval.bmax_; ++a) {
    for (int b = myInterval.bmin_; b < myInterval.bmax_; ++b) {
      for (int c = myInterval.bmin_; c < myInterval.bmax_; ++c) {
        for (int x = myInterval.pmin_; x < myInterval.pmax_; ++x) {
          for (int y = myInterval.pmin_; y < myInterval.pmax_; ++y) {
            for (int z = myInterval.pmin_; z < myInterval.pmax_; ++z) {
              printf("Calculating\n\n");
              double A = pow(a, x);
              double B = pow(b, y);
              double C = pow(c, z);
              if ((A + B) == (C)) {

                auto aFactors = factor(static_cast<int>(A));
                auto bFactors = factor(static_cast<int>(B));
                auto cFactors = factor(static_cast<int>(C));
                std::vector<int> aux, intersec;
                aux.resize(max(aFactors.size(), bFactors.size()));

                auto it = std::set_intersection(
                  aFactors.begin(), aFactors.end(),
                  bFactors.begin(), bFactors.end(),
                  aux.begin());
                aux.resize(it - aux.begin());
                intersec.resize(max(aux.size(), cFactors.size()));
                it = std::set_intersection(aux.begin(),
                  aux.end(), cFactors.begin(), cFactors.end(),
                  intersec.begin());
                intersec.resize(it - intersec.begin());
                if (intersec.empty()) {
                  resultMsg = "CounterExample " +
                    std::to_string(a) + " " +
                    std::to_string(x) + " " +
                    std::to_string(b) + " " +
                    std::to_string(y) + " " +
                    std::to_string(c) + " " +
                    std::to_string(z) + " ";//counterExample
                }
              }
            }
          }
        }
      }
    }
  }
  socket_.write(resultMsg);
  printf("Client: Peace Out\n\n");



}