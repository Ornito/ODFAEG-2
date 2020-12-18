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
            static Class getClass(std::string name, std::string nspc="", std::string path="");
            std::string getName();
            std::string getFilePath();
            std::string getSourcePath();
            std::vector<Constructor> getConstructors();
            std::vector<Class> getSuperClasses();
            std::string getNamespace();
        private :
            static void removeSpaces(std::string &str);
            static bool isName(std::string str, std::string str2);
            static void checkInnerClass(std::string innerClass, std::string type, std::string& fileContent, int lvl, Class& cl);
            static void checkConstructors(std::string& fileContent, Class& cl);
            static void checkSuperClasses(std::string& fileContent, Class& cl);
            void addInnerClass(Class innerClass);
            void setNamespace(std::string namespc);
            void addConstructor(Constructor constructor);
            void addSuperClass(Class cl);
            std::string name;
            std::string filePath;
            std::string sourceFile;
            std::string namespc;
            std::vector<Class> innerClasses;
            std::vector<Class> superClasses;
            std::vector<Constructor> constructors;
            std::vector<std::string> innerClass;
        };
    }
}
#endif // ODFAEG_CLASS_HPP
