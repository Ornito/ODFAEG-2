#include "../../../include/odfaeg/Core/constructor.hpp"
namespace odfaeg {
    namespace core {
        Constructor::Constructor(std::string name) : name(name) {
        }
        void Constructor::addArgType(std::string argType) {
            argsTypes.push_back(argType);
        }
        std::vector<std::string> Constructor::getArgsTypes() {
            return argsTypes;
        }
        std::string Constructor::getName() {
            return name;
        }
    }
}
