#include <iostream>
#include <thread>
#include <vector>
#include <windows.h>
#include <cstdio>
#include <string>
#include <queue>
#include "TreeHolder.cpp"
using namespace std;

static int DEFAULT_MAX_THREADS = 10;
bool isFounded = false;
void findFilesAndCatalogs(const std::string &basePath, std::vector<std::string> &files, std::vector<std::string> &catalogs);
void buildTreeRecursive(TreeHolder &tree);
void searchFileInTreeRecursive(TreeHolder &tree, string &searchedFileName);


int main(int argc, char* argv[]) {

    setlocale(LC_ALL, "rus");

    std::string searchedFile;

    std::string basePath = "C:";

    int threads = DEFAULT_MAX_THREADS;

    if (argc == 2 || argc == 4 || argc == 6) {
        searchedFile = argv[1];
        for (int i = 2; i < argc; i += 2) {
            std::string parameterName = argv[i];
            std::string parameterValue = argv[i + 1];
            if (parameterName == "--path") {
                basePath = parameterValue;
            }
            else if (parameterName == "--num_threads") {
                threads = stoi(parameterValue) <= DEFAULT_MAX_THREADS ? std::stoi(parameterValue) : DEFAULT_MAX_THREADS;
            }
            else {
                cout << "Only parameters --path and --num_threads supported";
                return -1;
            }
        }
    }
    else {
        cout << "Please enter filename as required first parameter. Also you can give optional parameters like this o./find_app some_file_name --path base_path --num_threads 15";
        return -1;
    }

    TreeHolder startTree;
    startTree.setPath(basePath);
    std::cout << "Starting building tree from: " + basePath << std::endl;
    buildTreeRecursive(startTree);
    std::cout << "Building tree is finished" << std::endl;
    searchFileInTreeRecursive(startTree, searchedFile);

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

void buildTreeRecursive(TreeHolder &tree) {
    vector<string> files;
    vector<string> catalogs;

    findFilesAndCatalogs(tree.getPath(), files, catalogs);

    tree.setContainedFiles(files);
    
    for (string child : catalogs) {
         TreeHolder treeChild;
         treeChild.setPath(tree.getPath() + "/" + child);
         buildTreeRecursive(treeChild);
         tree.addChild(treeChild);
    }
}

void searchFileInTreeRecursive(TreeHolder& tree, string& searchedFileName) {
        if (!tree.getContainedFiles().empty()) {
            for (string file : tree.getContainedFiles()) {
                if (file == searchedFileName) {
                    std::cout << "Founded file in path: " + tree.getPath() + "/" + searchedFileName << std::endl;
                    isFounded = true;
                    return;
                }
            }
        }
        for (TreeHolder childTree : tree.getChildsTreeHolders()) {
            if (!isFounded) {
                searchFileInTreeRecursive(childTree, searchedFileName);
        }
    }
}




