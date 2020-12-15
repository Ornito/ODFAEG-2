#include "../../../include/odfaeg/Core/class.hpp"
namespace odfaeg {
    namespace core {
        using namespace std;
        Class::Class(std::string name, std::string filePath, std::string sourceFile) : name(name), filePath(filePath), sourceFile(sourceFile) {

        }
        std::vector<std::string> Class::getClasses(std::string path) {
            std::vector<std::string> classes;
            std::string appiDir = getCurrentPath()+"\\"+path;
            std::cout<<appiDir<<std::endl;
            std::vector<std::string> files;
            findFiles(".hpp", files, appiDir);
            for (unsigned int i = 0; i < files.size(); i++) {
                //std::cout<<"file : "<<files[i]<<std::endl;
                ifstream ifs(files[i]);
                if (ifs) {
                    std::string line;
                    while(getline(ifs, line)) {
                        if (line.find("class") != std::string::npos && line.find("{") != std::string::npos) {
                            while (line.size() > 0 && line.at(0) == ' ') {
                                line.erase(0, 1);
                            }
                            std::vector<std::string> parts = split(line, " ");
                            while (parts[1].size() > 0 && parts[1].at(0) == ' ') {
                                parts[1].erase(0, 1);
                            }
                            //std::cout<<"class : "<<parts[1]<<std::endl;
                            classes.push_back(parts[1]);
                        }
                    }
                }
            }
            return classes;
        }
        Class Class::getClass(std::string name) {
            std::string appiDir = getCurrentPath();
            std::vector<std::string> files;
            findFiles(".hpp", files, appiDir);
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
            findFiles(".cpp", files, appiDir);
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
                std::string type = "";
                int lvl = 0;
                checkInnerClass(innerClass, type, fileContent, lvl,  cl);

                checkConstructors(fileContent, cl);
                return cl;
            }
            throw Erreur(70, "Class not found in project files!", 3);
        }
        void Class::checkConstructors(std::string& fileContent, Class& cl) {
            //std::cout<<"split parts"<<std::endl;
            std::vector<std::string> parts = split(fileContent, "}");
            for (unsigned int i = 0; i < parts.size(); i++) {
                //std::cout<<"check parts"<<std::endl;
                while(parts[i].find("{") != std::string::npos) {
                    int pos = parts[i].find_first_of("{");
                    int pos2 = parts[i].find_first_of("}");
                    parts[i] = parts[i].erase(pos, pos2-pos+1);
                }
                //std::cout<<"split parts 2 "<<std::endl;
                std::vector<std::string> parts2 = split(cl.getName(), "::");
                std::string name = parts2[parts2.size()-1];
                parts2 = split(parts[i], ";");
                for (unsigned int j = 0; j < parts2.size(); j++) {
                    //std::cout<<"remove spaces"<<std::endl;
                    while (parts2[j].size() > 0 && parts2[j].at(0) == ' ') {
                        parts2[j] = parts2[j].erase(0, 1);
                    }
                    while (parts2[j].size() > 0 && parts2[j].at(name.size()) == ' ') {
                        parts2[j] = parts2[j].erase(name.size(), 1);
                    }
                    //std::cout<<"space removed"<<std::endl;
                    int index = parts2[j].find_first_of("(");
                    //std::cout<<"index : "<<std::endl;
                    if (parts2[j].find(name) == 0 && index == name.size()) {
                        //std::cout<<"check construct args"<<std::endl;
                       int pos = parts2[j].find_first_of("(");
                       int pos2 = parts2[j].find_first_of(")");
                       std::string types = parts2[j].substr(pos+1, pos2-pos-1);
                       std::vector<std::string> arguments = split(types, ",");
                       Constructor constructor(cl.getName());
                       for (unsigned int i = 0; i < arguments.size(); i++) {
                            while(arguments[i].size() > 0 && arguments[i].at(0) == ' ') {
                                arguments[i] = arguments[i].erase(0, 1);
                            }
                            std::vector<std::string> argTypeName = split(arguments[i], " ");
                            std::string fullTypeName="";
                            if (argTypeName[0] == "const") {
                                fullTypeName = argTypeName[0]+" "+argTypeName[1];
                            } else {
                                fullTypeName = argTypeName[0];
                            }
                            if (fullTypeName != "") {
                                constructor.addArgType(fullTypeName);
                            }
                       }
                       pos = fileContent.find(cl.getName());
                       if (fileContent.find("{") != std::string::npos) {
                           pos2 = fileContent.find_first_of("}");
                       } else {
                           pos2 = fileContent.find_first_of(";");
                       }
                       fileContent.erase(pos, pos2-pos+1);
                       cl.addConstructor(constructor);

                    }
                }
            }
        }
        void Class::checkInnerClass(std::string innerClass, std::string type, std::string& fileContent, int lvl, Class& cl) {
            if (fileContent.find("class") != std::string::npos || fileContent.find("enum") != std::string::npos || fileContent.find("struct") != std::string::npos) {
                std::string type;
                if (fileContent.find("class") != std::string::npos) {
                    //std::cout<<"find inner class"<<std::endl;
                    int pos = fileContent.find("class");
                    //Check the namespace name.
                    fileContent = fileContent.substr(pos+5, fileContent.size()-pos-5);
                    type = "class";
                } else if (fileContent.find("enum") != std::string::npos) {
                    //std::cout<<"find inner enum"<<std::endl;
                    int pos = fileContent.find("enum");
                    //Check the namespace name.
                    fileContent = fileContent.substr(pos+4, fileContent.size()-pos-4);
                    type = "enum";
                } else if (fileContent.find("struct") != std::string::npos) {
                    //std::cout<<"find inner struct"<<std::endl;
                    int pos = fileContent.find("struct");
                    fileContent = fileContent.substr(pos+6, fileContent.size()-pos-6);
                    type = "struct";
                }
                while(fileContent.size() > 0 && fileContent.at(0) == ' ') {
                   fileContent.erase(0, 1);
                }
                std::vector<std::string> parts = split(fileContent, " ");
                //std::cout<<"part : "<<parts[0]<<std::endl;
                if (lvl == 0) {
                    innerClass = cl.getName()+"::"+parts[0];
                } else {
                    innerClass += "::"+parts[0];
                }
                //std::cout<<"inner class : "<<innerClass<<std::endl;
                lvl++;
                int pos = fileContent.find_first_of("{");
                fileContent = fileContent.substr(pos, fileContent.size() - pos + 1);
                checkInnerClass(innerClass, type, fileContent, lvl, cl);
            } else {
                Class innerCl(innerClass, cl.getFilePath(), cl.getSourcePath());
                int pos = fileContent.find_first_of("};");
                fileContent = fileContent.substr(pos+2, fileContent.size()-pos-2);
                cl.addInnerClass(innerCl);
            }
        }
        void Class::setNamespace(std::string namespc) {
            //std::cout<<"namespc : "<<namespc<<std::endl;
            this->namespc = namespc;
        }
        void Class::addInnerClass(Class innerClass) {
            //std::cout<<"innerClass type : "<<innerClass.getName()<<std::endl;
            innerClasses.push_back(innerClass);
        }
        void Class::addConstructor(Constructor c) {
            std::cout<<"add constructor : "<<c.getName()<<std::endl;
            std::vector<std::string> argsTypes = c.getArgsTypes();
            for (unsigned int i = 0; i < argsTypes.size(); i++) {
                std::cout<<"arg type : "<<argsTypes[i]<<std::endl;
            }
            constructors.push_back(c);
        }
        std::string Class::getName() {
            return name;
        }
        std::string Class::getFilePath() {
            return filePath;
        }
        std::string Class::getSourcePath() {
            return sourceFile;
        }
        std::vector<Constructor> Class::getConstructors() {
            return constructors;
        }
        std::string Class::getNamespace() {
            return namespc;
        }
    }
}
