#ifndef ODFAEG_CONSTRUCTOR_HPP
#define ODFAEG_CONSTRUCTOR_HPP
#include <string>
#include <vector>
namespace odfaeg {
    namespace core {
        class Constructor {
        public :
            Constructor (std::string name);
            void addArgType(std::string argType);
            std::vector<std::string> getArgsTypes();
            std::string getName();
        private:
            std::string name;
            std::vector<std::string> argsTypes;
        };
    }
}
#endif // ODFAEG_CONSTRUCTOR_HPP
