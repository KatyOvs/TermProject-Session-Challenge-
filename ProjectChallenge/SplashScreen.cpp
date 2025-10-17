#include "SplashScreen.h"
#include <iostream>
// загрузочный экран

void SplashScreen::loadResources() {
    // Загрузка 3 кадров для заставки
    for (int i = 1; i <= 3; i++) {
        sf::Texture texture;
        std::string filename = "img/" + std::to_string(i) + "z.png";
        if (!texture.loadFromFile(filename)) {
            std::cerr << "Failed to load splash texture: " << filename << std::endl;
        }
        splashTextures.push_back(texture);
    }

    // Настройка спрайта (растягиваем на весь виртуальный экран)
    splashSprite.setTexture(splashTextures[0]);
    splashSprite.setScale(
        1920.0f / splashTextures[0].getSize().x,
        1080.0f / splashTextures[0].getSize().y
    );

    // Настройка затемнения
    fadeOverlay.setSize(sf::Vector2f(1920, 1080));
    fadeOverlay.setFillColor(sf::Color(0, 0, 0, 0));
}

void SplashScreen::update(float deltaTime) {
    splashTime += deltaTime;

    // Обновление кадров анимации
    frameTimer += deltaTime;
    if (frameTimer >= frameDuration) {
        frameTimer = 0.0f;
        currentFrame = (currentFrame + 1) % splashTextures.size();
        splashSprite.setTexture(splashTextures[currentFrame]);
    }

    // Плавное затемнение после минимальной длительности
    if (splashTime >= minDuration) {
        fadeAlpha += deltaTime * 250; // Скорость затемнения
        if (fadeAlpha > 255) fadeAlpha = 255;
        fadeOverlay.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(fadeAlpha)));
    }
}

void SplashScreen::render(sf::RenderWindow& window) {
    window.clear();
    window.draw(splashSprite);
    window.draw(fadeOverlay);
    window.display();
}

void SplashScreen::show(sf::RenderWindow& window) {
    loadResources();
    splashTime = 0.0f;
    fadeAlpha = 0.0f;
    sf::Clock clock;

    while (splashTime < minDuration + 1.0f && window.isOpen()) {
        // Обработка событий
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
            // Пропуск заставки по клику или клавише
            if (event.type == sf::Event::KeyPressed ||
                event.type == sf::Event::MouseButtonPressed) {
                splashTime = minDuration; // Переход к затемнению
            }
        }

        // Обновление и отрисовка
        float deltaTime = clock.restart().asSeconds();
        update(deltaTime);
        render(window);

        // Выход после полного затемнения
        if (fadeAlpha >= 255) break;
    }

    // Дополнительная задержка для плавности
    sf::sleep(sf::milliseconds(300));
}