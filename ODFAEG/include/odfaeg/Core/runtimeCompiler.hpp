#ifndef ODFAEG_RUNTIME_COMPILER_HPP
#define ODFAEG_RUNTIME_COMPILER_HPP
#include <vector>
#include <string>
#include <fstream>
#include <dlfcn.h>
#include "erreur.h"
namespace odfaeg {
    namespace core {
        class RuntimeCompiler {
            public :
            RuntimeCompiler(std::string funcName);
            void compile();
            template<typename R, typename... A>
            R run(std::string fName, A... a) {
                std::string path = "./"+funcName+".so";
                if (!isDllOpened) {
                    flib = dlopen(path.c_str(), RTLD_LAZY);
                    if (!flib) {
                        throw Erreur(10, "Failed to open dynamic library!", 3);
                    }
                    isDllOpened = true;
                }
                typedef R(*func)(A...);
                func pfunc = (func) dlsym(flib, fName.c_str());
                if (!pfunc) {
                    throw Erreur(10, "Failed to load the function!", 3);
                }

                return pfunc(a...);
            }
            void addIncludeDir(std::string includeDir);
            void addLibraryDir(std::string libraryDir);
            void addLibrary(std::string library);
            void addSourceFile(std::string sourceFile);
            void addOption(std::string option);
            void addMacro(std::string macro);
            void addRuntimeFunction (std::string f);
            std::string getCompileErrors();
            private :
            void* flib;
            bool isDllOpened;
            std::string funcName;
            std::string compileErrors;
            std::vector<std::string> sourceFiles;
            std::vector<std::string> includeDirs;
            std::vector<std::string> libraryDirs;
            std::vector<std::string> libraries;
            std::vector<std::string> macros;
            std::vector<std::string> options;
            std::vector<std::string> functions;
        };
    }
}
#endif
