#ifndef MENU_H
#define MENU_H
#include "Material.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "Exam.h"  
#include <fstream>
#include <SFML/Audio.hpp>
enum MenuResult {
    Nothing,
    Exit,
    StartGame,
    ShowMaterial,
    ShowAbout,
    ContinueGame // Новое значение для продолжения игры
};

class Menu {
public:
    void show(sf::RenderWindow& window);
    bool isMenuOpen = true;

private:
    void loadResources();
    void handleEvents(sf::RenderWindow& window);
    void render(sf::RenderWindow& window);
    void updateAnimation(float deltaTime);
    void updateTitleAnimation(float deltaTime);
    void scaleElements(const sf::Vector2f& factors);
    void showTeacherSelection(sf::RenderWindow& window);

    // Ресурсы
    sf::Texture menuTexture1, menuTexture2, menuTexture3;
    sf::Texture menuTexture1Hover, menuTexture2Hover, menuTexture3Hover;
    sf::Texture exitButtonTexture, exitButtonHoverTexture;
    sf::Texture aboutTexture, titleTexture;
    sf::Texture continueTexture; // Новые текстуры для кнопки продолжения
    sf::Music backgroundMusic;
    // Ресурсы для выбора преподавателя
    sf::Texture backgroundTexture;
    sf::Texture shatovTexture, kizerTexture, lukshaTexture;
    sf::Texture shatovHoverTexture, kizerHoverTexture, lukshaHoverTexture;
    sf::Texture svetTexture;
    sf::Sprite backgroundSprite;
    sf::Sprite shatovSprite, kizerSprite, lukshaSprite;
    sf::Sprite continueButton; // Спрайт кнопки продолжения
    sf::Text teacherDescription;

    std::vector<sf::Texture> backgroundTextures;
    sf::Sprite menu1, menu2, menu3, exitButton, about, title;
    sf::Sprite animatedBackground;

    // Анимация
    int currentFrame = 0;
    float frameTimer = 0.0f;
    const float frameDuration = 0.2f;
    int menuNum = 0;
    float titleAnimationTime = 0.0f;
    const float titleAnimationDuration = 1.0f;
    float titleStartY = -200.0f;
    float titleTargetY = 50.0f;
    float scaleX = 1.0f, scaleY = 1.0f;

    // Состояние выбора преподавателя
    bool selectingTeacher = false;
    int selectedTeacher = 0; // 0 - none, 1 - shatov, 2 - kizer, 3 - luksha

    // Константы для позиционирования
    const float BUTTONS_START_Y = 100.0f;
    const float BUTTONS_SPACING = 50.0f;
    const float BUTTONS_LEFT_MARGIN = 100.0f;
    const float EXIT_BUTTON_RIGHT = 1700.0f;
    const float EXIT_BUTTON_TOP = 50.0f;
    const float CONTINUE_BUTTON_X = 1700.0f; // Позиция кнопки продолжения
    const float CONTINUE_BUTTON_Y = 600.0f;

    // Подсказка
    bool showExitTooltip = false;
    bool showContinueTooltip = false; // Подсказка для кнопки продолжения
    sf::Text exitTooltip;
    sf::Text continueTooltip;
    sf::Font font;

    // Проверка наличия сохранения
    bool hasSaveFile() const {
        std::ifstream in("save.dat");
        bool exists = in.good();
        in.close();
        return exists;
    }
};

#endif // MENU_H