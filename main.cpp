#include <fstream>
#include <iostream>
#include <sstream>

#include "Piroto.hpp"

auto measureAverageTime(const char* benchmarkName,
                        int times,
                        const std::function<void(void)>& compute) -> void {
  std::chrono::system_clock::time_point start, end;
  double ms;
  double totalTime = 0.0;

  for (int i = 0; i < times; i++) {
    start = std::chrono::system_clock::now();
    compute();
    end = std::chrono::system_clock::now();
    ms = std::chrono::duration_cast<std::chrono::duration<double>>(end - start)
             .count() *
         1000.0;
    totalTime += ms;
  }
  totalTime /= (double)times;
  printf("%s took %lf ms\n", benchmarkName, totalTime);
}

struct Record {
  string name;
  int count;
  Record(const string& name, int count) : name(name), count(count) {}
};

Piroto* readData(const string& fileName) {
  Piroto* piroto = new Piroto();

  std::ifstream file{fileName};
  if (!file)
    throw "could not open file";
  string line, value;
  int j = 0;
  while (file) {
    getline(file, line);
    stringstream ssLine(line);
    getline(ssLine, value, ',');
    string name = value;
    int count;
    // string lineProcessed = name;
    while (getline(ssLine, value, ',')) {
      count = atoi(value.c_str());
      Record* record = new Record(name, count);
      piroto->insert(name, count, record);
      // lineProcessed += ',' + std::to_string(count);
    }
    printf("terminó la línea %d\n", j);
    // if (lineProcessed.compare(line) != 0)
    //   cout << lineProcessed << endl << line << endl << endl;
    j++;
  }
  printf("de que terminó todo a ver\n");
  return piroto;
}

int main() {
  string fileName = "./dataset/dataset.txt";
  Piroto* triesito = readData(fileName);

  string command;
  while (1) {
    std::cin >> command;
    if (command == "exact") {
      std::cout << "\033[4;33mexact query\033[0m"
                << "\n";
      measureAverageTime("Piroto y 63'000", 10, [&triesito] {
        triesito->exactSearch("Piroto", 63'000);
      });
      measureAverageTime("Carla y 60'000", 10, [&triesito] {
        triesito->exactSearch("Carla", 60'000);
      });
      measureAverageTime("Macarena y 58'386", 10, [&triesito] {
        triesito->exactSearch("Macarena", 58'386);
      });
      measureAverageTime("Maca y 67'316", 10, [&triesito] {
        triesito->exactSearch("Maca", 67'316);
      });
      measureAverageTime("Gabriella y 43'059", 10, [&triesito] {
        triesito->exactSearch("Gabriella", 43'059);
      });
      measureAverageTime("Gabriel y 2'257", 10, [&triesito] {
        triesito->exactSearch("Gabriel", 2'257);
      });
      measureAverageTime("Emma y 47'553", 10, [&triesito] {
        triesito->exactSearch("Emma", 47'553);
      });
      measureAverageTime("Christopher y 73'341", 10, [&triesito] {
        triesito->exactSearch("Christopher", 73'341);
      });
    } else if (command == "range") {
      std::cout << "\033[4;36mrange query\033[0m"
                << "\n";
      measureAverageTime("Piroto y 60'100 - 64'900", 10, [&triesito] {
        auto res = triesito->rangeSearch("Piroto", LowerBound::GTE, 60'100,
                                         UpperBound::LTE, 64'900);
      });
      measureAverageTime("Carla y 59'000 - 63'800", 10, [&triesito] {
        auto res = triesito->rangeSearch("Carla", LowerBound::GTE, 59'000,
                                         UpperBound::LTE, 63'900);
      });
      measureAverageTime("Macarena y 25'000 - 55'000", 10, [&triesito] {
        auto res = triesito->rangeSearch("Macarena", LowerBound::GTE, 25'000,
                                         UpperBound::LTE, 55'000);
      });
      measureAverageTime("Maca y 25'000 - 55'000", 10, [&triesito] {
        auto res = triesito->rangeSearch("Maca", LowerBound::GTE, 25'000,
                                         UpperBound::LTE, 55'000);
      });
      measureAverageTime("Gabriella y 25'000 - 55'000", 10, [&triesito] {
        auto res = triesito->rangeSearch("Gabriella", LowerBound::GTE, 25'000,
                                         UpperBound::LTE, 55'000);
      });
      measureAverageTime("Gabriel y 25'000 - 55'000", 10, [&triesito] {
        auto res = triesito->rangeSearch("Gabriel", LowerBound::GTE, 25'000,
                                         UpperBound::LTE, 55'000);
      });
      measureAverageTime("Christopher y 25'000 - 55'000", 10, [&triesito] {
        auto res = triesito->rangeSearch("Christopher", LowerBound::GTE, 25'000,
                                         UpperBound::LTE, 55'000);
      });
      measureAverageTime("Emma y 25'000 - 55'000", 10, [&triesito] {
        auto res = triesito->rangeSearch("Emma", LowerBound::GTE, 25'000,
                                         UpperBound::LTE, 55'000);
      });
    } else if (command == "q") {
      break;
    }
  }

  delete triesito;

  return 0;
}
