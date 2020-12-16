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
#include "odfaeg/Graphics/GUI/checkBox.hpp"
#include "odfaeg/Graphics/circleShape.h"
#include "odfaeg/Graphics/rectangleShape.h"
#include "odfaeg/Graphics/sprite.h"
#include "odfaeg/Graphics/2D/decor.h"
#include "odfaeg/Graphics/2D/wall.h"
#include "odfaeg/Graphics/map.h"
#include "odfaegCreatorStateExecutor.hpp"
#include "odfaeg/Graphics/perPixelLinkedListRenderComponent.hpp"
#include "odfaeg/Graphics/shadowRenderComponent.hpp"
#include "odfaeg/Graphics/lightRenderComponent.hpp"
#include "odfaeg/Graphics/reflectRefractRenderComponent.hpp"
#include "odfaeg/Graphics/entitiesUpdater.h"
#include "rectangularSelection.hpp"
#include "odfaeg/Graphics/anim.h"
#include "odfaeg/Graphics/animationUpdater.h"
#include "odfaeg/Graphics/particleSystemUpdater.hpp"
#include "odfaeg/Graphics/tGround.h"
#include "odfaeg/Math/distributions.h"
#include "odfaeg/Core/class.hpp"
#include "odfaeg/Core/runtimeCompiler.hpp"
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
    void displayEntityInfos(odfaeg::graphic::Entity* entity);
    void displayTransformInfos(odfaeg::graphic::Transformable* transf);
    void displayInfos(odfaeg::graphic::Tile* tile);
    void displayInfos(odfaeg::graphic::g2d::Decor* decor);
    void displayInfos(odfaeg::graphic::g2d::Wall* wall);
    void displayInfos(odfaeg::graphic::Anim* anim);
    void displayInfos(odfaeg::physic::ParticleSystem* ps);
    void displayInfos(odfaeg::graphic::g2d::PonctualLight* pl);
    void displayChildren(odfaeg::graphic::gui::Label* label);
    void onObjectPosChanged(odfaeg::graphic::gui::TextArea* ta);
    void onObjectSizeChanged(odfaeg::graphic::gui::TextArea* ta);
    void onObjectColorChanged(odfaeg::graphic::gui::TextArea* ta);
    void onSelectedTextureChanged(odfaeg::graphic::gui::DropDownList* dp);
    void onTexCoordsChanged(odfaeg::graphic::gui::TextArea* ta);
    void onSelectedEmChanged(odfaeg::graphic::gui::DropDownList* dp);
    void onAnimUpdaterChanged(odfaeg::graphic::gui::DropDownList* dp);
    void onObjectMoveChanged(odfaeg::graphic::gui::TextArea* ta);
    void onObjectScaleChanged(odfaeg::graphic::gui::TextArea* ta);
    void onObjectRotationChanged(odfaeg::graphic::gui::TextArea* ta);
    void addShape(odfaeg::graphic::Shape *shape);
    void addTile(odfaeg::graphic::Tile *tile);
    bool removeShape (unsigned int id);
    void updateScriptPos(odfaeg::graphic::Transformable* shape);
    void updateScriptColor(odfaeg::graphic::Transformable* shape);
    void updateScriptTextCoords(odfaeg::graphic::Transformable* shape);
    void updateScriptText(odfaeg::graphic::Shape* shape, const odfaeg::graphic::Texture* text);
    void updateScriptText(odfaeg::graphic::Tile* tile, const odfaeg::graphic::Texture* text);
    void onObjectNameChanged(odfaeg::graphic::gui::TextArea* ta);
    void onSelectedParentChanged(odfaeg::graphic::gui::DropDownList* dp);
    void onWallTypeChanged(odfaeg::graphic::gui::TextArea* taWallType);
    void onFrameRateChanged(odfaeg::graphic::gui::TextArea* taFRChanged);
    void onParentClicked(odfaeg::graphic::gui::Label* label);
    void onParticleSystemUpdaterChanged(odfaeg::graphic::gui::DropDownList* dp);
    void onComponentExpressionChanged(odfaeg::graphic::gui::TextArea* ta);
    void onSelectedComponentChanged(odfaeg::graphic::gui::DropDownList* dp);
    void onShadowCenterChanged(odfaeg::graphic::gui::TextArea* ta);
    void onShadowScaleChanged(odfaeg::graphic::gui::TextArea* ta);
    void onShadowRotAngleChanged(odfaeg::graphic::gui::TextArea* ta);
    void onCollisionBoundingBoxChanged(odfaeg::graphic::gui::TextArea* ta);
    void onSelectedClassChanged(odfaeg::graphic::gui::DropDownList* dp);
    void onSelectedFunctionChanged(odfaeg::graphic::gui::DropDownList* dp);
    void onDroppedDown(odfaeg::graphic::gui::DropDownList* dp);
    enum Fonts {
        Serif
    };
    private :
        void convertSlash(std::string& path);
        odfaeg::math::Vec3f getGridCellPos(odfaeg::math::Vec3f pos);
        const float speed = 10.f;
        odfaeg::graphic::gui::MenuBar* menuBar;
        odfaeg::graphic::gui::Menu *menu1, *menu2, *menu3, *menu4, *menu5;
        odfaeg::graphic::gui::MenuItem *item11, *item12, *item13, *item14, *item15, *item16, *item17, *item18, *item21, *item22, *item23, *item31, *item32, *item33,
        *item34, *item35, *item36, *item37, *item38, *item39, *item310, *item311, *item41, *item42, *item43, *item44, *item45, *item51, *item52;
        odfaeg::core::ResourceCache<> cache;
        odfaeg::graphic::gui::FileDialog* fdTexturePath, *fdProjectPath;
        odfaeg::graphic::RenderWindow* wApplicationNew, *wNewMap, *wNewComponent, *wNewEntitiesUpdater, *wNewAnimUpdater, *wNewEmitter, *wNewParticleSystemUpdater, *wCreateNewWindow, *wCreateNewObject;
        odfaeg::graphic::gui::TextArea* ta, *taComponentExpression, *taComponentLayer, *taEntitiesUpdaterName, *taComponentName, *taAnimUpdaterName, *taPSName, *taEmissionRate,
        *taMinLifeTime, *taMaxLifeTime, *taRCPosX, *taRCPosY, *taRCPosZ, *taRCSizeX, *taRCSizeY, *taRCSizeZ, *taDeflX, *taDeflY, *taDeflZ, *taDeflAngle,
        *taRotMin, *taRotMax, *taTexIndexMin, *taTexIndexMax, *taScaleMinX, *taScaleMinY, *taScaleMinZ, *taScaleMaxX, *taScaleMaxY, *taScaleMaxZ, *taColor1, *taColor2,
        *taParticleSystemUpdaterName;
        odfaeg::graphic::gui::DropDownList* dpList, *dpSelectTexture, *dpMapTypeList, *dpComponentType, *dpSelectEm, *dpSelectComponent, *dpSelectParent, *dpSelectAU, *dpSelectPPType, *dpSelectPSU, *dpSelectClass, *dpSelectFunction, *dpSelectPointerType;
        odfaeg::graphic::gui::Label *lWidth, *lHeight, *lMapWidth, *lMapHeight;
        odfaeg::graphic::gui::TextArea *taWidth, *taHeight, *tScriptEdit, *taMapName, *taMapWidth, *taMapHeight, *taWallType, *taIntensity, *taQuality, *taWindowPos, *taWindowSize, *taWindowTitle, *taWindowName, *taObjectName, *taSelectExpression;
        odfaeg::graphic::gui::Panel *pProjects, *pScriptsFiles, *pScriptsEdit, *pInfos, *pTransform, *pMaterial, *pShadows, *pCollisions, *pComponent, *pObjectsParameters;
        std::string appliname, minAppliname;
        std::string applitype;
        std::string path;
        std::map<std::string, std::string> cppAppliContent;
        std::vector<std::string> openedProjects;
        std::vector<std::string> textPaths;
        std::vector<odfaeg::graphic::gui::TextArea*> tmpTextAreas;
        std::unique_ptr<odfaeg::graphic::gui::Node> rootNode, rootPropNode, rootMaterialNode, rootInfosNode, rootShadowsNode, rootCollisionNode, rootObjectParams;
        odfaeg::graphic::CircleShape cursor;
        odfaeg::math::Vec3f guiSize, guiPos, mousePosition;
        bool isGuiShown, showGrid, alignToGrid, showRectSelect;
        std::vector<std::unique_ptr<odfaeg::graphic::Shape>> shapes;
        odfaeg::graphic::Transformable* selectedObject;
        odfaeg::graphic::gui::TextArea *tPosX, *tPosY, *tPosZ, *tRColor, *tGColor, *tBColor, *tAColor, *tSizeW, *tSizeH, *tSizeD,
        *tMoveX, *tMoveY, *tMoveZ, *tScaleX, *tScaleY, *tScaleZ, *tRotAngle, *tTexCoordX, *tTexCoordY, *tTexCoordW, *tTexCoordH, *taName, *taChangeComponentExpression,
        *taXShadowCenter, *taYShadowCenter, *taZShadowCenter, *taXShadowScale, *taYShadowScale, *taZShadowScale, *taShadowRotAngle, *taBoundingBoxColX, *taBoundingBoxColY,
        *taBoundingBoxColZ, *taBoundingBoxColW, *taBoundingBoxColH, *taBoundingBoxColD;
        odfaeg::graphic::gui::Label *lPosX, *lPosY, *lPosZ, *lPosition, *lColor, *lRColor,
        *lGColor, *lBColor, *lAColor, *lTexture, *lTexCoordX, *lTexCoordY, *lTexCoordW, *lTexCoordH, *lTexImage, *lParent;
        odfaeg::graphic::gui::TabPane* tabPane;
        odfaeg::graphic::gui::Button* bChooseText, *bAddTexRect, *bCreateComponent, *bCreateScene, *bCreateEntitiesUpdater, *bCreateAppli, *bCreateAnimUpdater, *bCreateEmitter, *bCreateParticleSystemUpdater, *bCreateWindow, *bCreateObject;
        odfaeg::graphic::Shape* sTextRect;
        odfaeg::core::StateStack stateStack;
        ODFAEGCreatorStateExecutor se;
        std::vector<odfaeg::graphic::ConvexShape> cshapes;
        std::vector<odfaeg::graphic::ConvexShape> cellsPassableShapes;
        std::vector<odfaeg::graphic::RectangleShape> collisionsBox;
        odfaeg::graphic::Map* theMap;
        int gridWidth, gridHeight;
        RectangularSelection rectSelect;
        std::vector<std::string> emitterParams;
        std::vector<std::string> affectorParams;
        odfaeg::math::Vec3f viewPos;
        odfaeg::core::RuntimeCompiler rtc;
};
#endif
