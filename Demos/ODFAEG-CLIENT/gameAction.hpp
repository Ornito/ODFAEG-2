#ifndef GAME_ACTION_HPP
#define GAME_ACTION_HPP
#include "hero.h"
namespace sorrok {
    class GameAction : public odfaeg::core::Visitor<> {
        public :
        void operator()(Item& item, Hero* hero);
        void operator()(Skill& skill, Hero* hero);
    };
}
#endif
