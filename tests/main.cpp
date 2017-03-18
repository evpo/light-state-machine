#include <iostream>
#include <string>
#include "logger_init.h"
#include "gtest/gtest.h"

GTEST_API_ int main(int argc, char **argv) {
  std::cout << "Running main() from main.cpp\n";
  const std::string log_file("debug.out");
  CppProject::InitLogger(log_file);

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
