#include "../../../include/odfaeg/Core/class.hpp"
namespace odfaeg {
    namespace core {
        using namespace std;
        Class::Class(std::string name, std::string filePath, std::string sourceFile) : name(name), filePath(filePath), sourceFile(sourceFile) {

        }
        Class Class::getClass(std::string name) {
            std::string appiDir = getCurrentPath();
            std::vector<std::string> files;
            findFiles(".hpp .h", files, appiDir);
            //Recherche si la classe existe dans le projet.
            bool found=false;
            std::string fileContent;
            std::string headerFile, sourceFile;
            for (unsigned int i = 0; i < files.size() && !found; i++) {
                ifstream ifs(files[i]);
                if (ifs) {
                    std::string line;
                    fileContent="";
                    while(getline(ifs, line)) {
                        fileContent += line;
                        if (line.find("class") != std::string::npos && line.find(name) != std::string::npos && line.find("{") != std::string::npos) {
                            headerFile = files[i];
                            found = true;
                        }
                    }
                    ifs.close();
                }
            }
            findFiles(".cpp .c", files, appiDir);
            found = false;
            for (unsigned int i = 0; i < files.size() && !found; i++) {
                ifstream ifs(files[i]);
                if (ifs) {
                    std::string line;
                    while(getline(ifs, line)) {
                        if (line.find(name+"::"+name) != std::string::npos) {
                            sourceFile = files[i];
                            found = true;
                        }
                    }
                    ifs.close();
                }
            }
            if (found) {
                Class cl(name, headerFile, sourceFile);
                std::string namespc;
                int i = 0;
                bool found = false;
                //Check if the class is in one or more namespaces.
                while(fileContent.find("namespace") != std::string::npos && !found) {
                    unsigned int pos = fileContent.find("namespace");
                    //Check the namespace name.
                    fileContent = fileContent.substr(pos+9, fileContent.size()-pos-9);

                    while(fileContent.size() > 0 && fileContent.at(0) == ' ') {
                        fileContent.erase(0, 1);
                    }
                    std::vector<std::string> parts = split(fileContent, " ");
                    //Check if this is the first namespace.
                    if (i == 0)
                        namespc += parts[0];
                    else
                        namespc += "::"+parts[0];

                    pos = fileContent.find_first_of("{");
                    fileContent = fileContent.substr(pos+1, fileContent.size()-pos-1);
                    pos = fileContent.find("namespace");
                    unsigned int pos2 = fileContent.find(name);
                    //Check if we have found the namespace where the class is.
                    //std::cout<<pos<<" "<<pos2<<std::endl;
                    if (pos > pos2) {
                        cl.setNamespace(namespc);
                        found = true;
                    }
                }
                int pos = fileContent.find_first_of("{");
                fileContent = fileContent.substr(pos+1, fileContent.size()-pos-1);
                std::string innerClass = "";
                int lvl = 0;
                checkInnerClass(innerClass, fileContent, lvl,  cl);
                return cl;
            }
            throw Erreur(70, "Class not found in project files!", 3);
        }
        void Class::checkInnerClass(std::string innerClass, std::string fileContent, int lvl, Class& cl) {
            if (fileContent.find("class") != std::string::npos || fileContent.find("enum") != std::string::npos || fileContent.find("struct") != std::string::npos) {
                if (fileContent.find("class") != std::string::npos) {
                    //std::cout<<"find inner class"<<std::endl;
                    int pos = fileContent.find("class");
                    //Check the namespace name.
                    fileContent = fileContent.substr(pos+5, fileContent.size()-pos-5);
                } else if (fileContent.find("enum") != std::string::npos) {
                    //std::cout<<"find inner enum"<<std::endl;
                    int pos = fileContent.find("enum");
                    //Check the namespace name.
                    fileContent = fileContent.substr(pos+4, fileContent.size()-pos-4);
                } else if (fileContent.find("struct") != std::string::npos) {
                    //std::cout<<"find inner struct"<<std::endl;
                    int pos = fileContent.find("struct");
                    fileContent = fileContent.substr(pos+6, fileContent.size()-pos-6);
                }
                while(fileContent.size() > 0 && fileContent.at(0) == ' ') {
                   fileContent.erase(0, 1);
                }
                std::vector<std::string> parts = split(fileContent, " ");
                if (lvl == 0) {
                    innerClass = cl.getName()+"::"+parts[0];
                } else {
                    innerClass += "::"+parts[0];
                }
                //std::cout<<"inner class : "<<innerClass<<std::endl;
                lvl++;
                checkInnerClass(innerClass, fileContent, lvl, cl);
            } else {
                //std::cout<<"add inner class"<<std::endl;
                cl.addInnerClass(innerClass);
            }
        }
        void Class::setNamespace(std::string namespc) {
            //std::cout<<"namespc : "<<namespc<<std::endl;
            this->namespc = namespc;
        }
        void Class::addInnerClass(std::string innerClass) {
            std::cout<<"innerClass type : "<<innerClass<<std::endl;
            innerClasses.push_back(innerClass);
        }
        std::string Class::getName() {
            return name;
        }
    }
}
