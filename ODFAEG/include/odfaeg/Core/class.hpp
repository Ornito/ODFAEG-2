#ifndef ODFAEG_CLASS_HPP
#define ODFAEG_CLASS_HPP
#include <string>
#include "utilities.h"
#include "erreur.h"
#include <fstream>
#include <iostream>
#include <windows.h>
#include <dlfcn.h>
#include <string>
namespace odfaeg {
    namespace core {
        class Class {
        public :
            Class(std::string name, std::string filePath, std::string sourceFile);
            static Class getClass(std::string name);
            template<typename C, typename... Args>
            C instanciate(Args... args) {
                std::ifstream file(filePath);
                std::cout<<filePath<<std::endl;

                if (file) {
                    std::string line;

                    while (getline(file, line)) {
                        if (line.find(name) != std::string::npos && line.find("(") != std::string::npos) {
                            while (line.size() > 0 && line.at(0) == ' ') {
                                line = line.erase(0, 1);
                            }
                            while (line.size() > 0 && line.at(name.size()) == ' ') {
                                line = line.erase(name.size(), 1);
                            }
                            int index = line.find_first_of("(");
                            if (line.find(name) == 0 && index == name.size()) {
                               int pos = line.find_first_of("(");
                               int pos2 = line.find_first_of(")");
                               std::string types = line.substr(pos+1, pos2-pos-1);
                               std::vector<std::string> arguments = split(types, ",");
                               std::vector<std::string> constructArgsTypes;
                               for (unsigned int i = 0; i < arguments.size(); i++) {
                                    while(arguments[i].size() > 0 && arguments[i].at(0) == ' ') {
                                        arguments[i] = arguments[i].erase(0, 1);
                                    }
                                    std::vector<std::string> argTypeName = split(arguments[i], " ");
                                    std::string fullTypeName;
                                    if (argTypeName[0] == "const") {
                                        fullTypeName = argTypeName[0]+" "+argTypeName[1];
                                    } else {
                                        fullTypeName = argTypeName[0];
                                    }
                                    if(fullTypeName != "") {
                                        for (unsigned int i = 0; i < innerClasses.size(); i++) {
                                            //std::cout<<"inner class : "<<innerClasses[i]<<std::endl;
                                            if (innerClasses[i].find(fullTypeName) != std::string::npos) {
                                                fullTypeName = namespc + "::" + innerClasses[i];
                                            }
                                        }
                                    }
                                    std::cout<<"full type name : "<<fullTypeName<<std::endl;
                                    constructArgsTypes.push_back(fullTypeName);
                               }
                               constructors.push_back(constructArgsTypes);
                            }
                        }
                    }
                    file.close();
                }
                std::ofstream outfile("tmp.cpp");
                outfile<<"#include \""<<filePath<<"\"\n";
                outfile<<"#include <iostream>\n";
                outfile<<"extern \"C\" {\n";
                outfile<<name<<" instanciate(";
                for (unsigned int i = 0; i < constructors.size(); i++) {
                    if (sizeof...(Args) == constructors[i].size()) {
                        for (unsigned int j = 0; j < constructors[i].size(); j++) {
                            outfile<<constructors[i][j]<<" arg"<<j;
                            if (j != constructors[i].size()-1) {
                                outfile<<",";
                            }
                        }
                        outfile<<");\n }\n";
                    }
                }
                outfile<<name<<" instanciate(";
                unsigned int numArgs = -1;
                for (unsigned int i = 0; i < constructors.size(); i++) {
                    if (sizeof...(Args) == constructors[i].size()) {
                        numArgs = constructors[i].size();
                        for (unsigned int j = 0; j < constructors[i].size(); j++) {
                            outfile<<constructors[i][j]<<" arg"<<j;
                            if (j != constructors[i].size()-1) {
                                outfile<<",";
                            }
                        }
                        outfile<<") {\n";
                    }
                }
                if (numArgs == -1) {
                    throw Erreur(71, "No matching constructor found!", 3);
                }
                outfile<<"  std::cout<<\"call function\"<<std::endl;\n";
                outfile<<"  return "<<name<<"(";
                for (unsigned int i = 0; i < numArgs; i++) {
                    outfile<<"arg"<<i;
                    if (i != numArgs - 1) {
                        outfile<<",";
                    }
                }
                outfile<<");\n}";
                //outfile<<"}\n";
                outfile.close();
                std::ofstream outfile2("libtmp.DEF");
                outfile2<<"LIBRARY \"libtmp\"\n";
                outfile2<<"EXPORTS\n";
                outfile2<<"instanciate\n";
                outfile2.close();
                std::string cmd = "g++ -c Test/"+name+".cpp"+" -o "+name+".o";
                system (cmd.c_str());
                system ( "g++ -c tmp.cpp -o tmp.o");
                cmd = "g++ -shared -o libtmp.so tmp.o "+name+".o";
                system(cmd.c_str());
                void * flib = dlopen ( "./libtmp.so", RTLD_LAZY );
                if ( !flib ) {
                    std::cerr << "Cannot open library: " << dlerror() << '\n';
                }
                typedef C (*func)(Args...);
                func pfunc = (func) dlsym(flib,"instanciate");
                if (!pfunc) {
                    std::cerr << "could not locate the function" << std::endl;
                }
                if (pfunc) {
                    return pfunc(args...);
                }
            }
            std::string getName();
            void addInnerClass(std::string innerClass);
            void setNamespace(std::string namespc);
        private :
            static void checkInnerClass(std::string innerClass, std::string fileContent, int lvl, Class& cl);
            std::string name;
            std::string filePath;
            std::string sourceFile;
            std::string namespc;
            std::vector<std::string> innerClasses;
            std::vector<std::vector<std::string>> constructors;
            std::vector<std::string> innerClass;
        };
    }
}
#endif // ODFAEG_CLASS_HPP
