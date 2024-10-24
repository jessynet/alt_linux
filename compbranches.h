#ifndef COMPBRANCHES_H
#define COMPBRANCHES_H

#include <vector>
#include <string>
#include <set>
#include <nlohmann/json.hpp>

using std::vector;
using std::string;
using std::set;
using json = nlohmann::json;

void create_json(vector<vector<string>> p1, vector<vector<string>> p2, vector<vector<string>> pm1, const char* b1, const char* b2);
void pckg_vers_more(set<string> s1, set<string> s2, vector<vector<string>> p1, vector<vector<string>> p2, vector<vector<string>>& vers_more);
void pckg_only_this_branch(set<string> s1, set<string> s2, vector<vector<string>> p1, vector<vector<string>> p2, vector<vector<string>>& only_p1);
void get_arch_packages(json info, int count, set<string> s, vector<vector<string>>& p);
void get_all_arch(json info, set<string>& s, int count_pack);
void compare_branches(const char* path_temp_1, const char* path_temp_2);
void get_branch_info(vector<string> cmd, int fd);

#endif 