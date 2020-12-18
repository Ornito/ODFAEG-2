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
            findFiles(".hpp .h", files, appiDir);
            for (unsigned int i = 0; i < files.size(); i++) {

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
        Class Class::getClass(std::string name, std::string nspc, std::string path) {
            std::string appiDir;
            if (path == "")
                appiDir = getCurrentPath();
            else
                appiDir = path;
            //std::cout<<"path : "<<appiDir<<std::endl;
            std::vector<std::string> files;
            findFiles(".hpp .h", files, appiDir);
            //Recherche si la classe existe dans le projet.
            bool found=false;
            std::string fileContent;
            std::string headerFile, sourceFile;
            std::string namespc="";
            for (unsigned int i = 0; i < files.size() && !found; i++) {
                ifstream ifs(files[i]);
                if (ifs) {
                    std::string line;
                    fileContent="";
                    while(getline(ifs, line)) {
                        if (line.find("/*") != std::string::npos || line.find("/**") != std::string::npos) {
                            while(line.find("*/") == std::string::npos && getline(ifs, line)) {

                            }
                        }
                        if (line.find("//") == std::string::npos && line.find("*/") == std::string::npos) {
                            if (line.find("template ") != std::string::npos) {
                                int pos = line.find("template ");
                                int pos2 = line.find(">");
                                line.erase(pos, pos2 - pos+1);
                            }
                            if (line.find("<") != std::string::npos) {
                                int pos = line.find("<");
                                int pos2 = line.find(">");
                                line.erase(pos, pos2 - pos+1);
                            }
                            std::vector<std::string> parts = split(line, ":");

                            if (parts.size() > 0 && (parts[0].find("class ") != std::string::npos || parts[0].find("struct ") != std::string::npos) && parts[0].find(name) != std::string::npos) {
                                /*if (parts.size() > 0 && files[i].find("drawable.h") != std::string::npos)
                                    std::cout<<"part : "<<parts[0]<<std::endl;*/

                                if (parts[0].find("{") != std::string::npos) {
                                    int pos = parts[0].find("{");
                                    parts[0].erase(pos);
                                }
                                std::vector<std::string> parts2 = split(parts[0], " ");

                                while(parts2[parts2.size()-1].size() > 0 && parts2[parts2.size()-1].at(0) == ' ') {
                                    parts2[parts2.size()-1].erase(0, 1);
                                }
                                while(parts2[parts2.size()-1].size() > 0 && parts2[parts2.size()-1].at(parts2[parts2.size()-1].size()-1) == ' ') {
                                    parts2[parts2.size()-1].erase(parts2[parts2.size()-1].size()-1, 1);
                                }
                                std::cout<<parts2[parts2.size()-1]<<std::endl;
                                if (parts2[parts2.size()-1] == name) {
                                    //std::cout<<"found class in "<<files[i]<<std::endl;
                                    headerFile = files[i];
                                    found = true;
                                }
                            }
                            fileContent += line+"\n";

                        }
                    }
                    ifs.close();

                }
            }
            int j= 0;
            //std::cout<<"check namespace"<<std::endl;
            //Check if the class is in one or more namespaces.
            if (found) {
                while(fileContent.find("namespace ") != std::string::npos && found) {
                    unsigned int pos = fileContent.find("namespace ");
                    //Check the namespace name.
                    //std::cout<<"sub str"<<std::endl;
                    //std::cout<<"check namespace name : "<<std::endl;
                    fileContent = fileContent.substr(pos+9, fileContent.size()-pos-9);

                    while(fileContent.size() > 0 && fileContent.at(0) == ' ') {
                        fileContent.erase(0, 1);
                    }
                    std::vector<std::string> parts = split(fileContent, " ");
                    //Check if this is the first namespace.
                    if (j == 0)
                        namespc += parts[0];
                    else
                        namespc += "::"+parts[0];

                    pos = fileContent.find_first_of("{");
                    fileContent = fileContent.substr(pos+1, fileContent.size()-pos-1);
                    pos = fileContent.find("namespace ");
                    unsigned int pos2 = fileContent.find(name+" ");

                    found = true;
                    if (pos > pos2) {
                        if (nspc != "" && namespc != nspc) {
                            found = false;
                        }
                        fileContent = fileContent.substr(pos2);
                    }
                    j++;
                }
                //std::cout<<"file content : "<<fileContent<<std::endl;
                if (found) {
                    //std::cout<<"file content : "<<fileContent<<std::endl;
                    findFiles(".cpp .c", files, appiDir);
                    bool foundSrc = false;
                    for (unsigned int i = 0; i < files.size() && !foundSrc; i++) {
                        ifstream ifs(files[i]);
                        if (ifs) {
                            std::string line;
                            while(getline(ifs, line)) {
                                if (line.find(name+"::"+name) != std::string::npos) {
                                    sourceFile = files[i];
                                    foundSrc = true;
                                }
                            }
                            ifs.close();
                        }
                    }


                    Class cl(name, headerFile, sourceFile);
                    cl.setNamespace(namespc);
                    //std::cout<<"name : "<<name<<std::endl;


                    checkSuperClasses(fileContent, cl);
                    //std::cout<<"content : "<<fileContent<<std::endl;
                    /*int pos = fileContent.find("{");
                    fileContent = fileContent.substr(pos+1, fileContent.size()-pos-1);*/

                    std::string innerClass = "";
                    std::string type = "";
                    int lvl = 0;
                    checkInnerClass(innerClass, type, fileContent, lvl,  cl);
                    checkConstructors(fileContent, cl);
                    //std::cout<<"file content : "<<fileContent<<std::endl;

                    //std::cout<<"file content : "<<fileContent<<std::endl;
                    //std::cout<<"check constructors"<<std::endl;

                    return cl;
                }
            }
            throw Erreur(70, "Class "+name+"not found in project files!", 3);
        }
        void removeSpaces (std::string& str) {
            while(str.size() > 0 && str.at(0) == ' ') {
                str.erase(0, 1);
            }
            while(str.size() > 0 && str.at(str.size()-1) == ' ') {
                str.erase(str.size()-1, 1);
            }
        }
        bool Class::isName(std::string str, std::string str2) {
            if (str.size() == str2.size())
                return true;
            return false;
        }
        void Class::checkSuperClasses (std::string &fileContent, Class& cl) {


            bool found = false;
            std::string str = fileContent;
            int pos = fileContent.find("{");
            std::string names="";
            if (pos != std::string::npos){

                names = fileContent.substr(0, pos);
                fileContent = fileContent.substr(pos+1, fileContent.size() - pos-1);
            }
            if (names != "") {
                std::vector<std::string> parts = split(names, "::");
                std::string str;
                for (unsigned int i = 0; i < parts.size(); i++) {
                    str += parts[i];
                    if (i != parts.size()-1)
                        str+=" ";
                }
                //std::cout<<"str : "<<str<<std::endl;
                parts = split(str, ":");
                if (parts.size() > 1) {
                    parts = split(parts[1], ",");
                    for (unsigned int i = 0; i < parts.size(); i++) {

                        std::vector<std::string> parts2 = split(parts[i], " ");
                        if (parts2.size() > 0) {
                            while (parts2[parts2.size()-1].size() > 0 && parts2[parts2.size()-1].at(0) == ' ') {
                                parts2[parts2.size()-1] = parts2[parts2.size()-1].erase(0, 1);
                            }
                            while (parts2[parts2.size()-1].size() > 0 && parts2[parts2.size()-1].at(parts2[parts2.size()-1].size()-1) == ' ') {
                                parts2[parts2.size()-1] = parts2[parts2.size()-1].erase(parts2[parts2.size()-1].size()-1, 1);
                            }
                            int pos = parts2[parts2.size()-1].find("<");
                            if (pos != std::string::npos) {
                                parts2[parts2.size()-1].erase(pos);
                            }

                            if (cl.getName() != parts2[parts2.size()-1]) {
                                //std::cout<<"find class part 2 : "<<parts2[parts2.size()-1]<<std::endl;
                                Class cla = Class::getClass(parts2[parts2.size() - 1]);
                                //std::cout<<"found class part 2 : "<<parts2[parts2.size()-1]<<std::endl;
                                cl.addSuperClass(cla);
                            }
                        }

                    }
                }
            }

        }
        void Class::checkConstructors(std::string& fileContent, Class& cl) {


            //std::cout<<"file content : "<<fileContent<<std::endl;
            std::vector<std::string> parts = split(fileContent, "}");
            for (unsigned int i = 0; i < parts.size(); i++) {
                unsigned int nb = 0;
                //std::cout<<"check parts : "<<parts[i]<<std::endl;
                while(parts[i].find_last_of("{") != std::string::npos) {
                    int pos = parts[i].find_last_of("{");
                    //std::cout<<"pos : "<<pos<<std::endl;
                    parts[i] = parts[i].erase(pos);
                    nb++;
                }
                //std::cout<<"parts : "<<parts[i]<<" nb : "<<nb<<std::endl;
                std::string str;
                if(parts[i].find("(") != std::string::npos) {
                    int pos = parts[i].find("(");
                    //std::cout<<"pos : "<<pos<<std::endl;
                    str = parts[i].substr(0, pos+1);
                    //std::cout<<"sub str  :"<<str<<std::endl;
                    while (str.size() > 1 && str.at(str.size()-2) == ' ') {
                        str = str.erase(str.size()-2, 1);

                    }
                }
                if (str == "") {
                    str = parts[i];
                }

                //std::cout<<"str  :"<<str<<std::endl;
                /*std::cout<<(nb == 1 && str.find("if(") == std::string::npos && str.find("else") == std::string::npos && str.find("else if(") == std::string::npos && str.find("while(") == std::string::npos && str.find("switch(") == std::string::npos
                    && str.find("for(") == std::string::npos && str.find("do(") == std::string::npos)<<std::endl;*/
                if (nb == 1 && str.find("if(") == std::string::npos && str.find("else") == std::string::npos && str.find("else if(") == std::string::npos && str.find("while(") == std::string::npos && str.find("switch(") == std::string::npos
                    && str.find("for(") == std::string::npos && str.find("do(") == std::string::npos) {
                    //std::cout<<"parts : "<<parts[i]<<" nb : "<<nb<<std::endl;
                    if(parts[i].find("public") != std::string::npos && parts[i].find("private") != std::string::npos && parts[i].find("protected") != std::string::npos) {
                        int pos = parts[i].find(":");
                        parts[i] = parts[i].substr(pos+1);
                    }
                    //std::cout<<"part : "<<parts[i]<<std::endl;

                    std::vector<std::string> parts2 = split(parts[i], ";");
                    //std::cout<<"size : "<<parts2.size()<<std::endl;
                    for (unsigned int j = 0; j < parts2.size(); j++) {
                        //std::cout<<"part 2 : "<<parts2[j]<<std::endl;
                        int index = parts2[j].find("(");
                        if (index != std::string::npos) {
                            std::string name = parts2[j].substr(0, index);
                            name.erase(0, 1);
                            while (name.size() > 0 && name.at(0) == ' ') {
                                name = name.erase(0, 1);
                            }
                            while (name.size() > 0 && name.at(name.size()-1) == ' ') {
                                name = name.erase(name.size()-1, 1);
                            }

                            //std::cout<<"name : "<<name<<std::endl;
                            /*std::cout<<"space removed"<<std::endl;
                            std::cout<<"index : "<<index<<" size : "<<name.size()<<std::endl;*/
                            if (name.find(cl.getName()) == 0 && name == cl.getName()) {
                                //std::cout<<"check construct args : "<<parts2[j]<<std::endl;
                               int pos = parts2[j].find("(");
                               int pos2 = parts2[j].find(")");
                               if (pos != std::string::npos && pos2 != std::string::npos) {
                                   std::string types = parts2[j].substr(pos+1, pos2-pos-1);
                                   //std::cout<<"types : "<<types<<std::endl;
                                   std::vector<std::string> arguments = split(types, ",");
                                   Constructor constructor(cl.getName());
                                   for (unsigned int i = 0; i < arguments.size(); i++) {
                                        //std::cout<<"argument : "<<arguments[i]<<std::endl;
                                        while(arguments[i].size() > 0 && arguments[i].at(0) == ' ' || arguments[i].at(0) == '\n') {
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
                                   //std::cout<<"file content : "<<fileContent<<" pos  : "<<pos<<std::endl;
                                   if (pos != std::string::npos) {
                                       if (fileContent.find("{") != std::string::npos) {
                                           pos2 = fileContent.find("}");
                                       } else {
                                           pos2 = fileContent.find(";");
                                       }
                                       //std::cout<<"pos : "<<pos<<" pos 2 "<<pos2<<std::endl;
                                       fileContent = fileContent.erase(pos, pos2-pos+1);
                                       cl.addConstructor(constructor);
                                   }
                               }
                            }

                        }
                    }
                }
            }
        }
        void Class::checkInnerClass(std::string innerClass, std::string type, std::string& fileContent, int lvl, Class& cl) {
            if (fileContent.find("template ") != std::string::npos) {
                int pos = fileContent.find("template ");
                int pos2 = fileContent.find(">");
                fileContent.erase(pos, pos2 - pos+1);
            }
            if (fileContent.find("<") != std::string::npos) {
                int pos = fileContent.find("<");
                int pos2 = fileContent.find(">");
                fileContent.erase(pos, pos2 - pos+1);
            }
            if ((fileContent.find("class ") != std::string::npos || fileContent.find("enum ") != std::string::npos || fileContent.find("struct ") != std::string::npos)
                && fileContent.find("friend") == std::string::npos) {
                std::string type;
                //std::cout<<"check inner class"<<std::endl;

                if (fileContent.find("class ") != std::string::npos) {
                    //std::cout<<"find inner class"<<std::endl;
                    int pos = fileContent.find("class ");
                    //Check the namespace name.
                    //std::cout<<"file content : "<<fileContent<<std::endl;
                    fileContent = fileContent.substr(pos+5, fileContent.size()-pos-5);
                    //std::cout<<"file content : "<<fileContent<<std::endl;
                    type = "class";
                } else if (fileContent.find("enum ") != std::string::npos) {
                    //std::cout<<"find inner enum"<<std::endl;
                    int pos = fileContent.find("enum ");
                    //Check the namespace name.
                    fileContent = fileContent.substr(pos+4, fileContent.size()-pos-4);
                    type = "enum";
                } else if (fileContent.find("struct ") != std::string::npos) {
                    //std::cout<<"find inner struct"<<std::endl;
                    int pos = fileContent.find("struct ");
                    fileContent = fileContent.substr(pos+6, fileContent.size()-pos-6);
                    //std::cout<<"file content : "<<fileContent<<std::endl;
                    type = "struct";
                }
                while(fileContent.size() > 0 && fileContent.at(0) == ' ') {
                   fileContent.erase(0, 1);
                }
                std::vector<std::string> parts = split(fileContent, " ");

                /*int pos = fileContent.find("{");
                fileContent = fileContent.substr(pos, fileContent.size() - pos + 1);*/
                //std::cout<<"class file content : "<<fileContent<<std::endl;
                //std::cout<<"part : "<<parts[0]<<std::endl;

                if (lvl == 0) {
                    checkInnerClass(cl.getName()+"::"+parts[0], type, fileContent, lvl+1, cl);
                } else {
                    checkInnerClass(innerClass + "::"+parts[0], type, fileContent, lvl+1, cl);;
                }
                //std::cout<<"inner class : "<<innerClass<<std::endl;
                if (fileContent.find("};") != std::string::npos) {
                    lvl--;
                }
            }
            if (lvl > 0) {
                //std::cout<<"file content : "<<fileContent<<std::endl;
                std::vector<std::string> parts = split(innerClass, "::");
                int pos = fileContent.find(parts[parts.size()-1]);
                int pos2 = fileContent.find("};");
                //std::cout<<"lvl > 0 : "<<"pos : "<<pos<<" pos 2 "<<pos2<<std::endl;
                Class innerCl(innerClass, cl.getFilePath(), cl.getSourcePath());
                //std::cout<<"sub str : "<<std::endl;
                fileContent = fileContent.erase(pos, pos2 - pos+2);

                //std::cout<<"lvl file content : "<<fileContent<<std::endl;
                cl.addInnerClass(innerCl);
            }
        }
        void Class::addSuperClass(Class cl) {
            superClasses.push_back(cl);
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
            //std::cout<<"add constructor : "<<c.getName()<<std::endl;
            std::vector<std::string> argsTypes = c.getArgsTypes();
            /*for (unsigned int i = 0; i < argsTypes.size(); i++) {
                std::cout<<"arg type : "<<argsTypes[i]<<std::endl;
            }*/
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
        std::vector<Class> Class::getSuperClasses() {
            return superClasses;
        }
    }
}
