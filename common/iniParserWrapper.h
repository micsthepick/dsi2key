#pragma once

#include <string> // std::string
#include "iniparser/src/dictionary.h"

namespace D2K {namespace iniParser {

void dump(dictionary* d, FILE* f);
std::string getstring(dictionary* d, const std::string& key, const std::string& def);
int getint(dictionary* d, const std::string& key, int notfound);
dictionary* load(const std::string& ininame);
void freedict(dictionary* d);

}} // namespace D2K::iniParser
