#include <string>
#include <vector>
using namespace std;


class TreeHolder {
public:
	void setPath(string path) {
		this->path = path;
	}

	string getPath() {
		return this->path;
	}

	void setChildsTreeHolders(vector <TreeHolder> childsTreeHolders) {
		this->childsTreeHolders = childsTreeHolders;
	}

	vector<TreeHolder> getChildsTreeHolders() {
		return this->childsTreeHolders;
	}

	void setContainedFiles(vector <string> containedFiles) {
		this->containedFiles = containedFiles;
	}

	vector<string> getContainedFiles() {
		return this->containedFiles;
	}

	void addChild(TreeHolder child) {
		this->childsTreeHolders.push_back(child);
	}

private:
	string path;
	vector <TreeHolder> childsTreeHolders;
	vector <string> containedFiles;
};
