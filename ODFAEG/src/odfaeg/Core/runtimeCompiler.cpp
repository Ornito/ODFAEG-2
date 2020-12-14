#include "../../../include/odfaeg/Core/runtimeCompiler.hpp"
#include <iostream>
namespace odfaeg {
    namespace core {
        RuntimeCompiler::RuntimeCompiler(std::string functionName) : funcName(functionName) {
        }
        void RuntimeCompiler::compile() {
            std::ofstream file(funcName+".DEF");
            file<<"LIBRARY \""+funcName+"\"\n";
            file<<"EXPORTS\n";
            file<<funcName+"\n";
            file.close();
            std::string command;
            for (unsigned int s = 0; s < sourceFiles.size(); s++) {
                command="g++ -c "+sourceFiles[s]+".cpp -o "+sourceFiles[s]+".o ";
                for (unsigned int i = 0; i < options.size(); i++) {
                    command+="-"+options[i]+" ";
                }
                for (unsigned int i = 0; i < macros.size(); i++) {
                    command+="-D"+macros[i]+" ";
                }
                for (unsigned int i = 0; i < includeDirs.size(); i++) {
                    /*if (i == 0)
                        command += "-I";*/
                    command += "-I"+includeDirs[i]+" ";
                }
                command += " 2> "+funcName+".err";
                system(command.c_str());
                std::ifstream ifs(funcName+".err");
                std::string line;
                if (ifs) {
                    while (getline(ifs, line)) {
                        compileErrors+=line+"\n";
                    }
                    ifs.close();
                }
                std::cout<<"command : "<<command<<std::endl;
            }
            for (unsigned int i = 0; i < libraryDirs.size(); i++) {
                if (i == 0)
                    command = "g++ ";
                command += "-L"+libraryDirs[i]+" ";
            }
            for (unsigned int i = 0; i < libraries.size(); i++) {
                command += "-l"+libraries[i]+" ";
            }
            std::cout<<"command : "<<command<<std::endl;
            system(command.c_str());
            command = "g++ -shared -o "+funcName+".so";
            for (unsigned int s = 0; s < sourceFiles.size(); s++) {
                command+=" "+sourceFiles[s]+".o";
            }
            std::cout<<"command : "<<command<<std::endl;
            system(command.c_str());
        }
        void RuntimeCompiler::addSourceFile(std::string sourceFile) {
            sourceFiles.push_back(sourceFile);
        }
        std::string RuntimeCompiler::getCompileErrors() {
            return compileErrors;
        }
        void RuntimeCompiler::addMacro(std::string macro) {
            macros.push_back(macro);
        }
        void RuntimeCompiler::addOption(std::string option) {
            options.push_back(option);
        }
        void RuntimeCompiler::addIncludeDir(std::string includeDir) {
            includeDirs.push_back(includeDir);
        }
        void RuntimeCompiler::addLibraryDir(std::string libraryDir) {
            libraryDirs.push_back(libraryDir);
        }
        void RuntimeCompiler::addLibrary(std::string library) {
            libraries.push_back(library);
        }
    }
}
