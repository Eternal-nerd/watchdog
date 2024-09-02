/*

This program assumes that file changes will be tacked on to the end of the file.
The program initially outputs the contents of the file to the terminal, and
it will output an additional x amount of bytes from the END of the file each
time the file increases in size, like when a new log entry is appended to a
file.  It times out after 120 seconds of no changes to the "watched" file.

*/
#include <chrono>
#include <filesystem>
#include <iostream>
#include <string>
#include <thread>

namespace fs = std::filesystem;

void error(const std::string& message) {
  std::cout << "ERROR: " << message << "\n";
  exit(-1);
}

void printFromOffset(const std::string& filename, long offset) {
  FILE* fp = std::fopen(filename.c_str(), "rb");
  if (fp == nullptr) {
    error("File did not open. ");
  }

  std::fseek(fp, 0, SEEK_END);
  size_t size = ftell(fp) - offset;  // might be wrong?
  std::string ss;
  ss.resize(size);
  std::fseek(fp, offset, SEEK_SET);
  std::fread(&ss[0], 1, size, fp);

  std::cout << ss;

  std::fclose(fp);
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    error("This program requires ONE filepath as an arguement. ");
  }

  std::string filename = argv[1];
  const fs::path filepath = filename;
  if (filepath.empty()) {
    error("Filepath was empty. ");
  }

  if (!fs::exists(filepath)) {
    error("Path does not exist. ");
  }
  if (fs::is_directory(filepath)) {
    error("Path is a directory. ");
  }

  printFromOffset(filename, 0);
  long offset = fs::file_size(filepath);

  // check file every half second
  std::chrono::milliseconds waitTime(500);
  int halfSecondsSinceChange = 0;
  // exit after no change for 2 minutes
  while (halfSecondsSinceChange < 240) {
    if (fs::file_size(filepath) > offset) {
      printFromOffset(filename, offset);
      offset = fs::file_size(filepath);
      halfSecondsSinceChange = 0;
    }
    std::this_thread::sleep_for(waitTime);
    halfSecondsSinceChange += 1;
  }

  std::cout << "Program has timed out after 120 seconds of inactivity. \n";

  return 0;
}
