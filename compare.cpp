#include <iostream>
#include <getopt.h>
#include <map>
#include <string>
#include <vector>
#include "compbranches.h"

using std::cout;
using std::map;
using std::string;


//Вывод справки
void print_help()
{
    cout << "Использование:\n\n";


    cout << "compare [--одиночный-параметр]\n";
    cout << "compare [параметр] [аргумент] [параметр] [аргумент]\n";
    cout << "Аргументы должны быть разными\n\n";


    cout << "Одиночный параметр:\n\n";


    cout << "--help,  -h                                   Справка.\n\n";


    cout << "Параметры:\n\n";


    cout << "--branch1, --b1, -f <ИМЯ_ПЕРВОЙ_ВЕТКИ>        Указать имя первой ветки.\n";
    cout << "--branch2, --b2, -s <ИМЯ_ВТОРОЙ_ВЕТКИ>        Указать имя второй ветки.\n\n";


    cout << "Доступные имена веток:\n";
    cout << "sisyphus\n" << "p9\n" << "p10\n" << "p11\n";    

}

int main(int argc, char* argv[])
{
    if(argc == 1 || argc > 5) //Только имя утилиты (нет пераметров) или параметров больше нужного
    {
        print_help();
        return 0;
    }

    string name_branch_1 = "",name_branch_2 = "";

    map<int, string> code_name;

    code_name[1] = "sisyphus";
    code_name[2] = "p9";
    code_name[3] = "p10";
    code_name[4] = "p11";

    int res, option_index = -1;
    //f - first, s - second
    //: - значит, что аргумент обязателен
    //:: - аргумент опционально
    //без : - без аргументов
    const char* short_options = "hf:s:";


    const option long_options[] = 
    {
        //1 - обязательно должно быть значение 
        {"branch1", required_argument, NULL, 'f'},
        {"branch2", required_argument, NULL, 's'},
        {"b1", required_argument, NULL, 'f'},
        {"b2", required_argument, NULL, 's'},
        {"help", no_argument, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };

    //opterr = 0; //Не выводить сообщения об ошибке

    //Функция getopt() возвращает символ опции, если опция была успешно найдена, ":", если был пропущен параметр одной из опций, "?", если обнаружен неизвестный символ опции, или -1 в конце списка опций.
    while((res = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1) //-1, если символы опций кончились
    {
        switch(res)
        {
            case 'f':
            {
                for(int i = 1; i <= 4; i++)
                    if(optarg == code_name[i])
                    {
                        name_branch_1 = optarg;
                        break;
                    } 
                break;
            };

            case 's':
            {
                for(int i = 1; i <= 4; i++)
                {
                    if(code_name[i] == optarg)
                    {
                        name_branch_2 = optarg;
                        break;
                    }
                }

                break;
            }

            case 'h':
            {
                print_help();
                return 0;
            }
            //Совмещение трех условий
            //Пропущен параметр опции
            case ':':
            //Неизвестный символ опции
            case '?':
            {
                print_help();
                return 0;
            }
        };

        option_index = -1; //-1, чтобы проверять короткий ли параметр ибо у длинного индекс будет больше 0

    }
    

    //Хотя бы для одной ветки не введено корректное имя или имена одинаковые
    if((name_branch_1 == "" || name_branch_2 == "") || (name_branch_1 == name_branch_2))
    {
        print_help();
        return 0;
    }
    

    //curl -X GET -H 'Content-Type: application/json' https://rdb.altlinux.org/api/export/branch_binary_packages/sisyphus
    //curl -X GET -H 'Content-Type: application/json' https://rdb.altlinux.org/api/export/branch_binary_packages/p10

    string rest_api_method = "https://rdb.altlinux.org/api/export/branch_binary_packages/";

    string url_branch_1 = rest_api_method + name_branch_1;
    string url_branch_2 = rest_api_method + name_branch_2;

    char template_path_1[12 + name_branch_1.size()]; //полный путь к временному файлу
    sprintf(template_path_1, "/%s/%sXXXXXX", "tmp", name_branch_1.data());
    int fd_1 = mkstemp(template_path_1); //файловый дескриптор файла, идентифицирующий созданный файл

    char template_path_2[12 + name_branch_2.size()]; //полный путь к временному файлу
    sprintf(template_path_2, "/%s/%sXXXXXX", "tmp", name_branch_2.data());
    int fd_2 = mkstemp(template_path_2); //файловый дескриптор файла, идентифицирующий созданный файл
    
  
    get_branch_info({"curl", "-X", "GET", "-H", "\'Content-Type: application/json\'", url_branch_1}, fd_1);

    cout << "-------------------------------------------------------------------------------\n";

    get_branch_info({"curl", "-X", "GET", "-H", "\'Content-Type: application/json\'", url_branch_2}, fd_2);

    compare_branches(name_branch_1.data(), name_branch_2.data());

    return 0;

}