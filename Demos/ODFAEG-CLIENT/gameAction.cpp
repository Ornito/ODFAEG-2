#include "gameAction.hpp"
namespace sorrok {
    void GameAction::operator()(Item& item, Hero* hero) {
        if (item.getRequiredClass().find(hero->getClass()) != std::string::npos || item.getRequiredClass() == "ALL") {
            item.applyBehaviour();
        }
    }
    void GameAction::operator()(Skill& skill, Hero* hero) {
        if (skill.getRequiredClass().find(hero->getClass()) != std::string::npos || skill.getRequiredClass() == "ALL" && hero->getMana() >= skill.getManaCost()) {
            skill.applySkillBehaviour();
        }
    }
}
