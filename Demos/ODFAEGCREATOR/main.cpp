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
    /*void * flib = dlopen ( "./libtmp.so", RTLD_LAZY );
    typedef Item (*func)(std::string, std::string);
                func pfunc = (func) dlsym(flib,"instanciate");
                if (!pfunc) {
                    std::cerr << "could not locate the function" << std::endl;
                }
                if (pfunc) {
                    Item item = pfunc("1", "2");
                    std::cout<<item.getName()<<std::endl;
                }*/
    /*std::ofstream file("test.cpp");
    file<<"#include \"odfaeg/Core/class.hpp\"\n";
    file<<"#include \"Test/Scripts/item.hpp\"\n";
    file<<"extern \"C\" {\n";
    file<<"   sorrok::Item createItem(const char* name, sorrok::Item::Type type, const char* requiredClass);\n";
    file<<"}\n";
    file<<"sorrok::Item createItem(const char* name, sorrok::Item::Type type, const char* requiredClass) {\n";
    file<<"    return sorrok::Item(name, type, requiredClass);\n";
    file<<"}\n";
    file.close();
    RuntimeCompiler rtc("create");
    rtc.addRuntimeFunction("createItem");
    rtc.addSourceFile("test");
    rtc.addSourceFile("Test/Scripts/item");
    rtc.addOption("std=c++14");
    rtc.addMacro("ODFAEG_STATIC");
    rtc.addIncludeDir("\"..\\..\\..\\..\\Program Files (x86)\\ODFAEG\\include\"");
    rtc.addIncludeDir("..\\..\\Windows\\ODFAEG\\extlibs\\headers");
    rtc.addLibraryDir("\"..\\..\\..\\..\\Program Files (x86)\\ODFAEG\\lib\"");
    rtc.addLibraryDir("..\\..\\Windows\\ODFAEG\\extlibs\\libs-mingw\\x86");
    rtc.addLibrary("odfaeg-network-s");
	rtc.addLibrary("odfaeg-audio-s");
	rtc.addLibrary("odfaeg-graphics-s");
	rtc.addLibrary("odfaeg-physics-s");
	rtc.addLibrary("odfaeg-core-s");
	rtc.addLibrary("odfaeg-window-s");
	rtc.addLibrary("odfaeg-math-s");
	rtc.addLibrary("sfml-network");
	rtc.addLibrary("sfml-audio");
	rtc.addLibrary("sfml-graphics");
	rtc.addLibrary("sfml-window");
	rtc.addLibrary("sfml-system");
	rtc.addLibrary("crypto.dll");
	rtc.addLibrary("ssl.dll");
	rtc.addLibrary("freetype");
	rtc.addLibrary("glew32.dll");
	rtc.addLibrary("opengl32");
	rtc.addLibrary("gdi32");
	rtc.addLibrary("dl.dll");
	rtc.compile();
	std::string errors = rtc.getCompileErrors();
	std::cout<<"errors : "<<errors<<std::endl;
	sorrok::Item item = rtc.run<sorrok::Item>("createItem", "HP_POTION", sorrok::Item::Type::HP_POTION, "All");
	std::cout<<"item name : "<<item.getName()<<std::endl;
	//Class cl = Class::getClass("Item");
    //std::cout<<item.getName()<<std::endl;*/

   /*std::vector<sorrok::Item> vItem;
   sorrok::Item hp_potion = sorrok::Item ("HP_POTION",sorrok::Item::HP_POTION,"All");
   vItem.push_back(hp_potion);
   sorrok::Item mana_potion = sorrok::Item ("MANA_POTION",sorrok::Item::HP_POTION,"All");
   vItem.push_back(mana_potion);
   std::ofstream ofItem ("Item.oc");
   std::cout<<vItem.size()<<std::endl;
   odfaeg::core::OTextArchive oaItem (ofItem);
   oaItem (vItem);
   ofItem.close();*/
    Class cl = Class::getClass("Hero");
    /*std::cout<<"class : "<<cl.getName()<<std::endl;
    std::vector<Class> superClasses = cl.getSuperClasses();

    for (unsigned int i = 0; i < superClasses.size(); i++) {
        std::cout<<"super class : "<<superClasses[i].getName()<<std::endl;
    }*/
    ODFAEGCreator app(sf::VideoMode(1000,700),"ODFAEG Creator");
    return app.exec();

}
