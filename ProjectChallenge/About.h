#ifndef ABOUT_H
#define ABOUT_H

#include <SFML/Graphics.hpp>
#include <string>

class About {
public:
    About();
    bool show(sf::RenderWindow& window);

private:
    void loadResources();
    void handleEvents(sf::RenderWindow& window);
    void render(sf::RenderWindow& window);

    // Графические ресурсы
    sf::Texture backgroundTexture;
    sf::Texture aboutGameBtnTexture;
    sf::Texture aboutAuthorBtnTexture;
    sf::Texture exitBtnTexture;
    sf::Texture helpBtnTexture;  // Текстура кнопки справки

    sf::Sprite background;
    sf::Sprite aboutGameBtn;
    sf::Sprite aboutAuthorBtn;
    sf::Sprite exitBtn;
    sf::Sprite helpBtn;  // Спрайт кнопки справки

    // Текст
    sf::Font font;
    sf::Text gameText;
    sf::Text authorText;

    // Состояние
    bool showGameInfo;
    bool showAuthorInfo;
    bool exitRequested;
};

#endif // ABOUT_H