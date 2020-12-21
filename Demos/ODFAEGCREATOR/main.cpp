#include "application.hpp"
#include "../../../Projets/ODFAEGCREATOR/Test/Scripts/item.hpp"
using namespace odfaeg::graphic::gui;
using namespace odfaeg::graphic;
using namespace odfaeg::physic;
using namespace odfaeg::math;
using namespace odfaeg::core;
int main(int argc, char* argv[]) {
    EXPORT_CLASS_GUID(BoundingVolumeBoundingBox, BoundingVolume, BoundingBox)
    EXPORT_CLASS_GUID(EntityTile, Entity, Tile)
    EXPORT_CLASS_GUID(EntityTile, Entity, BigTile)
    EXPORT_CLASS_GUID(EntityWall, Entity, g2d::Wall)
    EXPORT_CLASS_GUID(EntityDecor, Entity, g2d::Decor)
    EXPORT_CLASS_GUID(EntityAnimation, Entity, Anim)
    EXPORT_CLASS_GUID(EntityMesh, Entity, Mesh)
    EXPORT_CLASS_GUID(EntityParticleSystem, Entity, ParticleSystem)
    EXPORT_CLASS_GUID(ShapeRectangleShape, Shape, RectangleShape)
    Class cl = Class::getClass("Item");
    ODFAEGCreator app(sf::VideoMode(1000,700),"ODFAEG Creator");
    return app.exec();

}
