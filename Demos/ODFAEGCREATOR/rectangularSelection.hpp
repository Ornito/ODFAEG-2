#ifndef ODFAEG_CREATOR_RECTANGULAR_SELECTION_HPP
#define ODFAEG_CREATOR_RECTANGULAR_SELECTION_HPP
#include "odfaeg/Graphics/rectangleShape.h"
class RectangularSelection : public odfaeg::graphic::Drawable {
public :
    RectangularSelection();
    void setRect(int posX, int posY, int width, int height);
    void addItem(odfaeg::graphic::Transformable* item, sf::Color color);
    odfaeg::physic::BoundingBox getSelectionRect();
    std::vector<std::pair<odfaeg::graphic::Transformable*, sf::Color>> getColors();
    std::vector<odfaeg::graphic::Transformable*> getItems();
    void setColor(odfaeg::graphic::Transformable* transformable, sf::Color color);
    void draw (odfaeg::graphic::RenderTarget& target, odfaeg::graphic::RenderStates states);
private :
    odfaeg::graphic::RectangleShape selectionRect;
    std::vector<std::pair<odfaeg::graphic::Transformable*, sf::Color>> items;
};
#endif // RECTANGULAR_SELECTION
