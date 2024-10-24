#include <iostream>
#include "lib.cpp"

int main(int argc, char* argv[])
{
    int br1 {0}, br2 {0};
    string name_branch_1,name_branch_2;

    bool is_correct1 = false, is_correct2 = false;;

    cout << "Выбор веток для сравнения\n";
    cout << "1. sisyphus\n";
    cout << "2. sisyphus_e2k\n";
    cout << "3. sisyphus_riscv64\n";
    cout << "4. sisyphus_loongarch64\n";
    cout << "5. p11\n";
    cout << "6. p10\n";
    cout << "7. p10_e2k\n";
    cout << "8. p9\n";
    cout << "9. p9_e2k\n";
    cout << "10. p8\n";
    cout << "11. c10f2\n";
    cout << "12. c10f1\n";
    cout << "13. c9f2\n";
    cout << "14. c7\n";
    
    map<int, string> code_name;

    code_name[1] = "sisyphus";
    code_name[2] = "sisyphus_e2k";
    code_name[3] = "sisyphus_riscv64";
    code_name[4] = "sisyphus_loongarch64";
    code_name[5] = "p11";
    code_name[6] = "p10";
    code_name[7] = "p10_e2k";
    code_name[8] = "p9";
    code_name[9] = "p9_e2k";
    code_name[10] = "p8";
    code_name[11] = "c10f2";
    code_name[12] = "c10f";
    code_name[13] = "c9f2";
    code_name[14] = "c7";

    while(!is_correct1)
    {
        cout << "Выбор первой ветки (число от 1 до 14): ";
        if(scanf("%d", &br1) == 1 && br1 >= 1 && br1 <= 14)
        {
            is_correct1 = true;
            name_branch_1 = code_name[br1];
        }
        else continue;
        
    }

    while(!is_correct2)
    {
        cout << "\nВыбор второй ветки (число от 1 до 14): ";
        cin >> br2;
        if(br2 >= 1 && br2 <= 14 && br2 != br1)
        {
            is_correct2 = true;
            name_branch_2 = code_name[br2];
        }
        else continue;;

    }

    //curl -X GET -H 'Content-Type: application/json' https://rdb.altlinux.org/api/export/branch_binary_packages/sisyphus
    //curl -X GET -H 'Content-Type: application/json' https://rdb.altlinux.org/api/export/branch_binary_packages/p10

    string rest_api_method = "https://rdb.altlinux.org/api/export/branch_binary_packages/";

    string url_branch_1 = rest_api_method + name_branch_1;
    string url_branch_2 = rest_api_method + name_branch_2;

    char template_path_1[12 + name_branch_1.size()]; //полный путь к временному файлу
    sprintf(template_path_1, "/%s/%sXXXXXX", "tmp", name_branch_1.data());
    //cout << template_path_1 << endl;
    int fd_1 = mkstemp(template_path_1); //файловый дескриптор файла, идентифицирующий созданный файл

    char template_path_2[12 + name_branch_2.size()]; //полный путь к временному файлу
    sprintf(template_path_2, "/%s/%sXXXXXX", "tmp", name_branch_2.data());
    //cout << template_path_2 << endl;
    int fd_2 = mkstemp(template_path_2); //файловый дескриптор файла, идентифицирующий созданный файл
    
  
    get_branch_info({"curl", "-X", "GET", "-H", "\'Content-Type: application/json\'", url_branch_1}, fd_1);

    cout << "-------------------------------------------------------------------------------\n";

    get_branch_info({"curl", "-X", "GET", "-H", "\'Content-Type: application/json\'", url_branch_2}, fd_2);

    compare_branches(name_branch_1.data(), name_branch_2.data());

    return 0;
}