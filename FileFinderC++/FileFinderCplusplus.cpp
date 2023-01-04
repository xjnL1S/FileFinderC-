#include <iostream>
#include <thread>
#include <vector>
#include <windows.h>
#include <cstdio>
#include <string>
#include "TreeHolder.cpp"
using namespace std;

static int MAX_THREADS = 10;
static int RUNNING_PARALLEL_THREADS = 0;
bool isFounded = false;

void findFilesAndCatalogs(const std::string &basePath, std::vector<std::string> &files, std::vector<std::string> &catalogs);
void buildTreeRecursive(TreeHolder &tree);
void searchFileInTreeRecursive(TreeHolder &tree, string&searchedFileName);
void startThreadSearching(TreeHolder& tree, string &searchedFileName);


int main(int argc, char* argv[]) {

    setlocale(LC_ALL, "rus");

    std::string searchedFile;

    std::string basePath = "C:";

    // Проверяем на наличие обязательных и опциональных параметров
    if (argc == 2 || argc == 4 || argc == 6) {
        searchedFile = argv[1];
        for (int i = 2; i < argc; i += 2) {
            std::string parameterName = argv[i];
            std::string parameterValue = argv[i + 1];
            if (parameterName == "--path") {
                basePath = parameterValue;
            }
            else if (parameterName == "--num_threads") {
                MAX_THREADS = stoi(parameterValue) <= MAX_THREADS ? std::stoi(parameterValue) : MAX_THREADS;
            }
            else {
                cout << "Only parameters --path and --num_threads supported";
                return -1;
            }
        }
    }
    else {
        cout << "Please enter filename as required first parameter. Also you can give optional parameters like this o./find_app some_file_name --path base_path --num_threads 8";
        return -1;
    }

    //Создаем стартовый заданный корень дерева и выстраиваем рекурсивно узлы дерева
    TreeHolder startTree;
    startTree.setPath(basePath);
    std::cout << "Starting building tree from: " + basePath << std::endl;
    buildTreeRecursive(startTree);
    std::cout << "Building tree is finished" << std::endl;
    std::cout << "Search file: " + searchedFile << std::endl;
    std::cout << "Max threads: " << MAX_THREADS << std::endl;

    searchFileInTreeRecursive(startTree, searchedFile);

    while (RUNNING_PARALLEL_THREADS > 0) {
        this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if (!isFounded) {
        std::cout << "Can't find this file: " + searchedFile << std::endl;
    }

    return 0;
}

void findFilesAndCatalogs(const std::string& basePath, std::vector<std::string>& files, std::vector<std::string>& catalogs) {
    // Функция, считывающая, какие и есть каталоги и файлы в каталоге root
    WIN32_FIND_DATA f;

    HANDLE h = FindFirstFile((basePath + "/*").c_str(), &f);
    if (h != INVALID_HANDLE_VALUE) {
        do {
            std::string fileName = std::string(f.cFileName);
            if (fileName != "." && fileName != "..") {
                // Данная проверка позволяет отфильтровать файлы от каталогов
                if (f.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    catalogs.push_back(fileName);
                }
                else {
                    files.push_back(fileName);
                }
            }
        } while (FindNextFile(h, &f));
    }
};

//функция по рекурсивному построению дерева
void buildTreeRecursive(TreeHolder &tree) {
    vector<string> files;
    vector<string> catalogs;

    findFilesAndCatalogs(tree.getPath(), files, catalogs);

    tree.setContainedFiles(files);
    
    //Если были найдены каталоги в дереве, то создаем дочерние узлы дерева и вызываем рекурсию еще раз
    for (string child : catalogs) {
         TreeHolder treeChild;
         treeChild.setPath(tree.getPath() + "/" + child);
         buildTreeRecursive(treeChild);
         tree.addChild(treeChild);
    }
}

//функция по рекурсивному поиску файлов
void searchFileInTreeRecursive(TreeHolder &tree, string &searchedFileName) {
    if (isFounded) {
        return;
    }
    for (string file : tree.getContainedFiles()) {
        if (file == searchedFileName) {
           std::cout << "Founded file in path: " + tree.getPath() + "/" + searchedFileName << std::endl;
           isFounded = true;
           return;
        }
    }

    // Для каждого дочернего узла подключаем при поиске (в случае наличия свободных) новый фоновый поток
    for (TreeHolder childTree : tree.getChildsTreeHolders()) {
        if (!isFounded) {
            if (RUNNING_PARALLEL_THREADS < MAX_THREADS) {
                thread thread(startThreadSearching, ref(tree), ref(searchedFileName));
                thread.detach();
                
                //ждем старта параллельного потока (чтобы в основном потоке счетчик работающих параллельных потоков не стал 0 раньше, чем закончится работа параллельных потоков).
                this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            else {
                searchFileInTreeRecursive(childTree, searchedFileName);
            }
        }
    }
}

//функция по старту потока (инкремент, декремент счетчика потоков)
void startThreadSearching(TreeHolder &tree, string &searchedFileName) {
    RUNNING_PARALLEL_THREADS++;
    searchFileInTreeRecursive(tree, searchedFileName);
    RUNNING_PARALLEL_THREADS--;
}



