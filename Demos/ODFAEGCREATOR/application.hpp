#ifndef ODFAEGCREATOR
#define ODFAEGCREATOR
#include "odfaeg/Core/application.h"
#include "odfaeg/Core/stateStack.h"
#include "odfaeg/Graphics/GUI/menubar.hpp"
#include "odfaeg/Graphics/GUI/menuItem.hpp"
#include "odfaeg/Graphics/GUI/filedialog.hpp"
#include "odfaeg/Graphics/GUI/textArea.hpp"
#include "odfaeg/Graphics/GUI/button.hpp"
#include "odfaeg/Graphics/GUI/label.hpp"
#include "odfaeg/Graphics/GUI/dropDownList.hpp"
#include "odfaeg/Graphics/GUI/node.hpp"
#include "odfaeg/Graphics/GUI/tabPane.hpp"
#include "odfaeg/Graphics/circleShape.h"
#include "odfaeg/Graphics/rectangleShape.h"
#include "odfaeg/Graphics/sprite.h"
#include "odfaeg/Graphics/map.h"
#include "odfaegCreatorStateExecutor.hpp"
#include "odfaeg/Graphics/perPixelLinkedListRenderComponent.hpp"
#include "odfaeg/Graphics/zSortingRenderComponent.hpp"
#include "odfaeg/Graphics/shadowRenderComponent.hpp"
#include "odfaeg/Graphics/lightRenderComponent.hpp"
#include "odfaeg/Graphics/entitiesUpdater.h"
#include "rectangularSelection.hpp"
class ODFAEGCreator : public odfaeg::core::Application,
                      public odfaeg::graphic::gui::MenuItemListener,
                      public odfaeg::graphic::gui::ActionListener {
    public :
    ODFAEGCreator (sf::VideoMode vm, std::string title);
    void moveCursor(sf::Vector2f mousePos);
    void onLoad();
    void onInit ();
    void onRender(odfaeg::graphic::RenderComponentManager *cm);
    void onDisplay(odfaeg::graphic::RenderWindow* window);
    void onUpdate (odfaeg::graphic::RenderWindow*, odfaeg::window::IEvent& event);
    void onExec ();
    void processKeyHeldDown(odfaeg::window::IKeyboard::Key key);
    void actionPerformed(odfaeg::graphic::gui::Button* item);
    void actionPerformed(odfaeg::graphic::gui::MenuItem* item);
    void showProjectsFiles(odfaeg::graphic::gui::Label* label);
    void showSourcesFiles(odfaeg::graphic::gui::Label* label);
    void showHeadersFiles(odfaeg::graphic::gui::Label* label);
    void showGUI(odfaeg::graphic::gui::Label* label);
    void showFileContent(odfaeg::graphic::gui::Label* lab);
    void displayInfos(odfaeg::graphic::Shape* shape);
    void displayInfos(odfaeg::graphic::Tile* tile);
    void displayChildren(odfaeg::graphic::gui::Label* label);
    void onObjectPosChanged(odfaeg::graphic::gui::TextArea* ta);
    void onObjectColorChanged(odfaeg::graphic::gui::TextArea* ta);
    void onSelectedTextureChanged(odfaeg::graphic::gui::DropDownList* dp);
    void onTexCoordsChanged(odfaeg::graphic::gui::TextArea* ta);
    void onSelectedEmChanged(odfaeg::graphic::gui::DropDownList* dp);
    void onSelectedRTChanged(odfaeg::graphic::gui::DropDownList* dp);
    void addShape(odfaeg::graphic::Shape *shape);
    bool removeShape (unsigned int id);
    void updateScriptPos(odfaeg::graphic::Transformable* shape);
    void updateScriptText(odfaeg::graphic::Shape* shape, odfaeg::graphic::Texture* text);
    enum Fonts {
        Serif
    };
    private :
        odfaeg::graphic::gui::MenuBar* menuBar;
        odfaeg::graphic::gui::Menu *menu1, *menu2, *menu3, *menu4;
        odfaeg::graphic::gui::MenuItem *item11, *item12, *item13, *item14, *item15, *item21, *item22, *item23, *item31, *item32, *item33,
        *item34, *item41, *item42, *item43, *item44, *item45;
        odfaeg::core::ResourceCache<> cache;
        odfaeg::graphic::gui::FileDialog* fdTexturePath, *fdProjectPath;
        odfaeg::graphic::RenderWindow* wApplicationNew, *wNewMap, *wNewComponent, *wNewEntitiesUpdater;
        odfaeg::graphic::gui::TextArea* ta, *taComponentExpression, *taComponentLayer, *taEntitiesUpdaterName;
        odfaeg::graphic::gui::DropDownList* dpList, *dpSelectTexture, *dpMapTypeList, *dpComponentType, *dpSelectEm;
        odfaeg::graphic::gui::Label *lWidth, *lHeight, *lMapWidth, *lMapHeight;
        odfaeg::graphic::gui::TextArea *taWidth, *taHeight, *tScriptEdit, *taMapName, *taMapWidth, *taMapHeight;
        odfaeg::graphic::gui::Panel *pProjects, *pScriptsFiles, *pScriptsEdit, *pInfos, *pTransform, *pMaterial;
        std::string appliname, minAppliname;
        std::string applitype;
        std::string path;
        std::string cppAppliContent;
        std::vector<std::string> textPaths;
        std::unique_ptr<odfaeg::graphic::gui::Node> rootNode, rootPropNode, rootMaterialNode, rootInfosNode;
        odfaeg::graphic::CircleShape cursor;
        odfaeg::math::Vec3f guiSize, guiPos, mousePosition;
        bool isGuiShown, showGrid, alignToGrid, showRectSelect;
        std::vector<std::unique_ptr<odfaeg::graphic::Shape>> shapes;
        odfaeg::graphic::Transformable* selectedObject;
        odfaeg::graphic::gui::TextArea *tPosX, *tPosY, *tPosZ, *tRColor, *tGColor, *tBColor, *tAColor,
        *tTexCoordX, *tTexCoordY, *tTexCoordW, *tTexCoordH;
        odfaeg::graphic::gui::Label *lPosX, *lPosY, *lPosZ, *lPosition, *lColor, *lRColor,
        *lGColor, *lBColor, *lAColor, *lTexture, *lTexCoordX, *lTexCoordY, *lTexCoordW, *lTexCoordH, *lTexImage;
        odfaeg::graphic::gui::TabPane* tabPane;
        odfaeg::graphic::gui::Button* bChooseText, *bCreateComponent, *bCreateScene, *bCreateEntitiesUpdater, *bCreateAppli;
        odfaeg::graphic::Shape* sTextRect;
        odfaeg::core::StateStack stateStack;
        ODFAEGCreatorStateExecutor se;
        std::vector<odfaeg::graphic::ConvexShape> cshapes;
        std::vector<std::unique_ptr<odfaeg::graphic::Entity>> entities;
        odfaeg::graphic::Map* theMap;
        int gridWidth, gridHeight;
        RectangularSelection rectSelect;
};
#endif
