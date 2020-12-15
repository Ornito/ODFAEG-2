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
#include "constructor.hpp"
namespace odfaeg {
    namespace core {
        class Class {
        public :
            Class(std::string name, std::string filePath, std::string sourceFile);
            static std::vector<std::string> getClasses(std::string filePath);
            static Class getClass(std::string name);
            template<typename C, typename... Args>
            C instanciate(Args... args) {

                std::ofstream outfile("tmp.cpp");
                //std::cout<<"file path : "<<filePath<<std::endl;
                outfile<<"#include \""<<filePath<<"\"\n";
                outfile<<"#include <iostream>\n";
                outfile<<"extern \"C\" {\n";
                outfile<<name<<" instanciate(";
                for (unsigned int i = 0; i < constructors.size(); i++) {
                    std::vector<std::string> argsTypes = constructors[i].getArgsTypes();
                    if (sizeof...(Args) == argsTypes.size()) {
                        for (unsigned int j = 0; j < argsTypes.size(); j++) {
                            std::string argType = argsTypes[j];
                            for (unsigned int n = 0; n < innerClasses.size(); n++) {
                                if (innerClasses[n].getName().find(argType) != std::string::npos) {
                                    std::cout<<"inner class name : "<<innerClasses[n].getName()<<std::endl;
                                    argType = namespc + "::" + innerClasses[n].getName();
                                }
                            }
                            outfile<<argType<<" arg"<<j;
                            if (j != argsTypes.size()-1) {
                                outfile<<",";
                            }
                        }
                        outfile<<");\n }\n";
                    }
                }
                outfile<<name<<" instanciate(";
                unsigned int numArgs = -1;
                for (unsigned int i = 0; i < constructors.size(); i++) {
                    std::vector<std::string> argsTypes = constructors[i].getArgsTypes();
                    if (sizeof...(Args) == argsTypes.size()) {
                        numArgs = argsTypes.size();
                        for (unsigned int j = 0; j < argsTypes.size(); j++) {
                            std::string argType = argsTypes[j];
                            for (unsigned int n = 0; n < innerClasses.size(); n++) {
                                if (innerClasses[n].getName().find(argType) != std::string::npos) {
                                    argType = namespc + "::" + innerClasses[n].getName();
                                }
                            }
                            outfile<<argType<<" arg"<<j;
                            if (j != argsTypes.size()-1) {
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
                std::string cmd = "g++ -c "+sourceFile+" -o "+name+".o";
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
            void addInnerClass(Class innerClass);
            void setNamespace(std::string namespc);
            void addConstructor(Constructor constructor);
            std::string getFilePath();
            std::string getSourcePath();
            std::vector<Constructor> getConstructors();
            std::string getNamespace();
        private :
            static void checkInnerClass(std::string innerClass, std::string type, std::string& fileContent, int lvl, Class& cl);
            static void checkConstructors(std::string& fileContent, Class& cl);
            std::string name;
            std::string filePath;
            std::string sourceFile;
            std::string namespc;
            std::vector<Class> innerClasses;
            std::vector<Constructor> constructors;
            std::vector<std::string> innerClass;
        };
    }
}
#endif // ODFAEG_CLASS_HPP
