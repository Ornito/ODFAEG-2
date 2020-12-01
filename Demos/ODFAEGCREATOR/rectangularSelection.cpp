#include "rectangularSelection.hpp"
#include "odfaeg/Graphics/renderTarget.h"
using namespace odfaeg::graphic;
using namespace odfaeg::math;
using namespace odfaeg::physic;
RectangularSelection::RectangularSelection() {
    selectionRect.setOutlineThickness(1.f);
    selectionRect.setFillColor(sf::Color::Transparent);
    selectionRect.setOutlineColor(sf::Color::Red);
}
void RectangularSelection::setRect(int posX, int posY, int width, int height) {
    selectionRect.setSize(Vec3f(width, height, 0));
    selectionRect.setPosition(Vec3f(posX, posY, 0));
}
void RectangularSelection::addItem(Transformable* item, sf::Color color) {
    bool contains = false;
    for (unsigned int i = 0; i < items.size() && !contains; i++) {
        if (items[i].first == item)
            contains = true;
    }
    if (!contains)
        items.push_back(std::make_pair(item, color));
}
BoundingBox RectangularSelection::getSelectionRect() {
    BoundingBox rect(selectionRect.getPosition().x, selectionRect.getPosition().y, 0, selectionRect.getSize().x, selectionRect.getSize().y, 0);
    return rect;
}
void RectangularSelection::setColor(Transformable* transformable, sf::Color color) {
    for (unsigned int i = 0; i < items.size(); i++) {
        if (items[i].first == transformable) {
            items[i].second = color;
        }
    }
}
std::vector<std::pair<odfaeg::graphic::Transformable*, sf::Color>> RectangularSelection::getColors() {
    return items;
}
std::vector<Transformable*> RectangularSelection::getItems() {
    std::vector<Transformable*> itms;
    for (unsigned int i = 0; i < items.size(); i++) {
        itms.push_back(items[i].first);
    }
    return itms;
}
void RectangularSelection::draw(RenderTarget& target, RenderStates states) {
    target.draw(selectionRect, states);
}
