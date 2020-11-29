#include "gameAction.hpp"
namespace sorrok {
    void GameAction::operator()(Item& item, Hero* hero) {
        if (item.getRequiredClass().find(hero->getClass()) != std::string::npos || item.getRequiredClass() == "ALL") {
            item.applyBehaviour();
        }
    }
    void GameAction::operator()(Skill& skill, Hero* hero) {
        std::cout<<"game action skill"<<std::endl;
        if (skill.getRequiredClass().find(hero->getClass()) != std::string::npos || skill.getRequiredClass() == "ALL" && hero->getMana() >= skill.getManaCost()) {
            std::cout<<"apply skill behaviour"<<std::endl;
            skill.applySkillBehaviour();
            std::cout<<"skill behaviour updated"<<std::endl;
        }
    }
}
