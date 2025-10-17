#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <SFML/Graphics.hpp>

class SplashScreen {
public:
    void show(sf::RenderWindow& window);

private:
    void loadResources();
    void update(float deltaTime);
    void render(sf::RenderWindow& window);

    // Ресурсы для заставки
    std::vector<sf::Texture> splashTextures;
    sf::Sprite splashSprite;
    sf::RectangleShape fadeOverlay;

    // Параметры анимации
    int currentFrame = 0;
    float frameTimer = 0.0f;
    const float frameDuration = 0.2f; // 5 FPS
    float splashTime = 0.0f;
    const float minDuration = 3.0f; // Минимальная длительность 3 секунды
    float fadeAlpha = 0.0f; // Альфа-канал для затемнения
};

#endif // SPLASHSCREEN_H