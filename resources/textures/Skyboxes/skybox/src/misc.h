#ifndef MISC_H
#define MISC_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

char* loadFile(const char *filename);
std::string pad(int x);
std::string number_format(int x);

#endif