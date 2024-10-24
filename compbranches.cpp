#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <fstream>
#include <malloc.h>
#include <vector>
#include <map>
#include <set>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "compbranches.h"

#include <nlohmann/json.hpp>
#include "filesystem.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::cerr;
using std::ifstream;
using std::ofstream;
using std::ios;
using std::set;
using std::map;

using json = nlohmann::json;
namespace fs = ghc::filesystem;
using vvs = vector<vector<string>>;

void create_json(vvs p1, vvs p2, vvs pm1, const char* b1, const char* b2)
{
    json result;

    string tmp;

    result["first_branch"] = b1;

    for(int j = 0; j < p1.size(); j++)
    {
        vector <string> tmp_json;
        tmp = p1[j][0].substr(1, p1[j][0].size() - 2);
        for(int i = 1; i < p1.size(); i++)
            tmp_json.push_back(p1[j][i].substr(1, p1[j][i].size() - 2));
        result["first_branch_packages_only"][tmp] = tmp_json;   
    }

    for(int k = 0; k < pm1.size(); k++)
    {
        vector<string> tmp_json;
        tmp = pm1[k][0].substr(1, pm1[k][0].size() - 2);
        for(int s = 1; s < pm1[k].size(); s++)
            tmp_json.push_back(pm1[k][s].substr(1, pm1[k][s].size() - 2));
        result["first_branch_packages_version_more_second_branch_packages"][tmp] = tmp_json;
    }

    result["second_branch"] = b2;
    for(int p = 0; p < p2.size(); p++)
    {
        vector<string> tmp_json;
        tmp = p2[p][0].substr(1, p2[p][0].size() - 2);
        for(int c = 1; c < p2[p].size(); c++)
            tmp_json.push_back(p2[p][c].substr(1, p2[p][c].size() - 2));
        result["second_branch_packages_only"][tmp] = tmp_json;
    } 

    ofstream out("output.json");

    if(out.is_open())
    {
        out << result;

    }

    out.close();

}

void pckg_vers_more(set<string> s1, set<string> s2, vvs p1, vvs p2, vvs& vers_more)
{
 
    int ind;
    bool exists_arch = false;

    for(int i = 0; i < s1.size(); i++)
    {
        map<string, string> name_vers_b1, name_vers_b2;
        for(int p = 1; p < p1[i].size(); p+=2) name_vers_b1[p1[i][p]] = p1[i][p + 1]; 

        for(int k = 0; k < s2.size(); k++)
            if(p2[k][0] == p1[i][0]) 
            {
                exists_arch = true;
                ind = k;
                for(int s = 1; s < p2[k].size(); s+=2) name_vers_b2[p2[k][s]] = p2[k][s + 1]; 
                break;
            }

        if(!exists_arch) continue; //Если нет во второй ветке такой архитектуры 

        vector<string> tmp_pck;
        tmp_pck.push_back(p1[i][0]);

        for(const auto& [name1, version1] : name_vers_b1)
        {
            if(name_vers_b2.count(name1)) //Если во второй ветке есть такой пакет (есть такой ключ)
            {
                if(strverscmp(version1.data(), name_vers_b2[name1].data()) > 0)  //>0 если первая версия-строка идет позже второй версии-строки
                    tmp_pck.push_back(name1);  

            }
        
        }

        exists_arch = false;
        vers_more.push_back(tmp_pck);
       
    }

}


void pckg_only_this_branch(set<string> s1, set<string> s2, vvs p1, vvs p2, vvs& only_p1)
{
    int ind = 0; //Индекс такой же архитектуры во второй ветке
    for(int i = 0; i < s1.size(); i++)
    {
        //Если архитектура есть в обеих ветках
        if(s2.count(p1[i][0])) //Если элемент есть во множестве ,то вернется 1, если нет - 0
        {
            for(int j = 0; j < s2.size(); j++)
            {
                if(p2[j][0] == p1[i][0]) 
                {
                    ind = j;
                    break;
                }
            }
        
            set<string> tmp_name_b1, tmp_name_b2;
            vector<string> pckg;
            pckg.push_back(p1[i][0]);
            for(int p = 1; p < p1[i].size(); p+=2) tmp_name_b1.insert(p1[i][p]);
            for(int k = 1; k < p2[ind].size(); k+=2) tmp_name_b2.insert(p2[ind][k]);
            for(auto pc1 : tmp_name_b1)
                if(!tmp_name_b2.count(pc1)) //Пакет не найден во второй ветке
                    pckg.push_back(pc1);

            only_p1.push_back(pckg);

        }

        else //Во второй ветке нет такой же архитектуры -> добавляем все пакеты
        {
            vector<string> pckg;
            pckg.push_back(p1[i][0]);

            for(int p = 1; p < p1[i].size(); p+=2)
                pckg.push_back(p1[i][p]);

            only_p1.push_back(pckg);

        } 

    }

}

void get_arch_packages(json info, int count, set<string> s, vvs& p)
{
    int k = 0;
    for(auto i : s)
    {
        vector<string> packg_vers;
        packg_vers.push_back(i); //Первым элементом будет архитектура

        for(int j = 0; j < count; j++)
        {
            if(info[j]["arch"].dump() == i)
            {
                //Добавление имение пакета и его версии
                string name = info[j]["name"].dump();
                string vers = info[j]["version"].dump();
                packg_vers.push_back(name);
                packg_vers.push_back(vers);
                //Значение будет представлять из себя вектор такого вида: ИМЯ ПАКЕТА1 ВЕРСИЯ ПАКЕТА1 ИМЯ ПАКЕТА2 ВЕРСИЯ ПАКЕТА2..... 
            
            }
               
        }

        p.push_back(packg_vers);

        k++;
    }
}

void get_all_arch(json info, set<string>& s, int count_pack)
{
    for(int i = 0; i < count_pack; i++)
    {
        string arch_name = info[i]["arch"].dump();
        s.insert(arch_name);

    }

}

void compare_branches(const char* path_temp_1, const char* path_temp_2)
{

    json info_branch_1, info_branch_2;

    ifstream branch_1, branch_2;

    string f1,f2;

    int count_pack_br1 {0}, count_pack_br2 {0};
    for(const auto& filename : fs::directory_iterator("/tmp"))
    {
        string fname = fs::path(filename).filename();
        //Завершающий нулевой символ не входит в длину строки
        //Функция strlen видит начало Си-строки  и начинает сначала считать количество символов (байтов, отводимых под каждый символ), этот процесс выполняется до тех пор, пока не будет достигнут завершающий нулевой символ
        if(fname.substr(0, strlen(path_temp_1)) == path_temp_1)
        {
            f1 = fs::path(filename);
            branch_1.open(f1);
        } 
        else if(fname.substr(0, strlen(path_temp_2)) == path_temp_2)
        {   f2 = fs::path(filename);
            branch_2.open(f2);
        } 
    }

    branch_1 >> info_branch_1;
    branch_2 >> info_branch_2;

    count_pack_br1 = info_branch_1["length"];
    count_pack_br2 = info_branch_2["length"];

    //множество - контейнер, который хранит только уникальные значения, множество упорядочивает элементы
    set <string> arch_b1, arch_b2;

    vvs arch_packages_b1, arch_packages_b2, json_packages_b1, json_packages_b2, json_more_ver_b1;

    //Получение множества архитектур веток
    get_all_arch(info_branch_1["packages"], arch_b1, count_pack_br1);
    get_all_arch(info_branch_2["packages"],arch_b2, count_pack_br2);


    //Получение пакетов в каждой архитектуре для обеих веток
    get_arch_packages(info_branch_1["packages"], count_pack_br1, arch_b1, arch_packages_b1);
    get_arch_packages(info_branch_2["packages"], count_pack_br2, arch_b2, arch_packages_b2);


    //Получение пакетов только на одной ветке
    pckg_only_this_branch(arch_b1, arch_b2, arch_packages_b1, arch_packages_b2, json_packages_b1);
    pckg_only_this_branch(arch_b2, arch_b1, arch_packages_b2, arch_packages_b1, json_packages_b2);


    //Получение пакетов из первой ветки, которые есть во второй и у которых больше версия
    pckg_vers_more(arch_b1, arch_b2, arch_packages_b1, arch_packages_b2, json_more_ver_b1);


    create_json(json_packages_b1, json_packages_b2, json_more_ver_b1, path_temp_1, path_temp_2);

    branch_1.close();
    branch_2.close();

    remove(f1.data());
    remove(f2.data());

}

void get_branch_info(vector<string> cmd, int fd)
{

    pid_t pid = fork();

    switch(pid){
        case -1:
        {
            cerr << "Не удалось разветвить процесс\n";
            exit(1);
        }
        case 0: //дочерний процесс
        {
            dup2(fd, STDOUT_FILENO); //Дублирование fd как 1, то есть как стандартный поток вывода

            char* args[cmd.size() + 1]; //массив строк

            for(int i = 0; i < cmd.size(); i++)
            {
                args[i] = new char[strlen(cmd[i].data())];
                strcpy(args[i], cmd[i].data()); //char *strcpy(char *str1, const char *str2)

            }

            args[cmd.size()] = NULL;
            
            char* command_name = args[0];
            //int execvp(const char* command, char* argv[]);
            /*command - команда, исполняемый двоичный файл, который является частью PATH переменной среды (например,ls)
            Второй аргумент ( argv) представляет собой список аргументов для command. Это массив char* строк.
            В argv содержится полная команда вместе с ее аргументами.(Например, "ls", "-l", NULL)
            Этот массив ДОЛЖЕН быть NULL завершен, т.е. последний элемент argv должен быть NULL указателем.*/

            execvp(command_name, args);
            cerr << "Не удалось выполнить команду exec\n";
            _exit(42);
            break;

        }
        default: //родительский процесс
        {
            int status;
            do{
                waitpid(pid, &status, 0);
            } while(!WIFEXITED(status));
            //WIFEXITED(status) возвращает истинное значение, если потомок нормально завершился, то есть вызвал exit или _exit, или вернулся из функции main() 
            break;
        }

    }

    close(fd);
}



