#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Menu.h"
#include "SplashScreen.h"
#include "About.h"
#include "Material.h"
#include <sstream>

int main() {
    // Создаем окно в полноэкранном режиме
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Exam Game", sf::Style::Fullscreen);
    // Загрузка и настройка фоновой музыки
   

    // Получаем текущий размер окна
    const sf::Vector2u windowSize = window.getSize();
    const float targetAspectRatio = 16.0f / 9.0f;

    // Рассчитываем масштабирующий вид (view)
    sf::View view;
    float windowAspectRatio = static_cast<float>(windowSize.x) / windowSize.y;
    float viewWidth = 1920.0f;
    float viewHeight = 1080.0f;
    float viewportX = 0.0f;
    float viewportY = 0.0f;
    float viewportWidth = 1.0f;
    float viewportHeight = 1.0f;

    if (windowAspectRatio > targetAspectRatio) {
        // Широкое окно - добавляем черные полосы по бокам
        viewWidth = viewHeight * windowAspectRatio;
        viewportWidth = targetAspectRatio / windowAspectRatio;
        viewportX = (1.0f - viewportWidth) / 2.0f;
    }
    else {
        // Высокое окно - добавляем черные полосы сверху и снизу
        viewHeight = viewWidth / windowAspectRatio;
        viewportHeight = windowAspectRatio / targetAspectRatio;
        viewportY = (1.0f - viewportHeight) / 2.0f;
    }

    view.setSize(viewWidth, viewHeight);
    view.setCenter(viewWidth / 2, viewHeight / 2);
    view.setViewport(sf::FloatRect(viewportX, viewportY, viewportWidth, viewportHeight));
    window.setView(view);

    // Показываем заставку
    SplashScreen splash;
    splash.show(window);
   
    

    // Создаем и показываем меню
    Menu mainMenu;
    mainMenu.show(window);

    // Основной игровой цикл

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                window.close();
            }

        }

        window.clear();
        // Отрисовка игры...
        window.display();
    }
  
    return 0;
}