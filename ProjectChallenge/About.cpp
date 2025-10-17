#include "About.h"
#include <iostream>
#include <sstream>
#include <windows.h> // Для ShellExecute

About::About() : showGameInfo(true), showAuthorInfo(false), exitRequested(false) {
    loadResources();
}

void About::loadResources() {
    // Загрузка текстур
    if (!backgroundTexture.loadFromFile("img/Abbk.png") ||
        !aboutGameBtnTexture.loadFromFile("img/abgam.png") ||
        !aboutAuthorBtnTexture.loadFromFile("img/abav.png") ||
        !exitBtnTexture.loadFromFile("img/exitbutton.png") ||
        !helpBtnTexture.loadFromFile("img/morebut.png")) {
        std::cerr << "Ошибка загрузки текстур About!" << std::endl;
    }

    // Настройка спрайтов
    background.setTexture(backgroundTexture);

    aboutGameBtn.setTexture(aboutGameBtnTexture);
    aboutGameBtn.setPosition(0.f, 150.f);

    aboutAuthorBtn.setTexture(aboutAuthorBtnTexture);
    aboutAuthorBtn.setPosition(0.f, 400.f);

    exitBtn.setTexture(exitBtnTexture);
    exitBtn.setPosition(0.f, 900.f);

    // Кнопка справки
    helpBtn.setTexture(helpBtnTexture);
    helpBtn.setPosition(0.f, 650.f);

    // Загрузка шрифта
    if (!font.loadFromFile("TDAText.ttf")) {
        std::cerr << "Ошибка загрузки шрифта!" << std::endl;
    }

    // Настройка текста
    gameText.setFont(font);
    gameText.setLineSpacing(1.2f);
    gameText.setString(
        L"Про игру:\n\n"
        L"В данной игре вам предстоит погрузиться в атмосферу студенческой\n"
        L"сессии, где вы будете сдавать экзамены по разным предметам.\n\n"
        L"Вам нужно выбрать дисциплину, взять билет и ответить на вопросы\n"
        L"трёх типов: теоретические с выбором варианта, практические и\n"
        L"логические головоломки.\n\n"
        L"Ваши ответы влияют на оценку.Получите оценку ниже 4 - потеряете жизнь и\n"
        L"провалите экзамен.\n\nЦель - успешно сдать все экзамены и пережить сессию!\n\n"
        L"Управление простое: используйте мышь для выбора и клавиатуру\nдля ввода ответов.\nУдачи на экзаменах!\n\n"
        L"Версия 1.0"
    );

    gameText.setCharacterSize(32);
    gameText.setFillColor(sf::Color::White);
    gameText.setStyle(sf::Text::Regular);
    gameText.setPosition(500.f, 150.f);

    authorText.setFont(font);
    authorText.setString(
        L"Автор:\n\n"
        L"Я Овсяник Екатерина и на момент создания игры я\nучусь на 2 курсе колледжа!\n\n"
        L"Этот проект является моей курсовой работой\nи он полностью реализован на языке программирования С++\n\n"
        L"Это мой первый серьезный проект и я очень\nнадеюсь что вам понравится! :)\n"
    );
    authorText.setCharacterSize(32);
    authorText.setFillColor(sf::Color::White);
    authorText.setStyle(sf::Text::Regular);
    authorText.setPosition(500.f, 150.f);
}

void About::handleEvents(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if (aboutGameBtn.getGlobalBounds().contains(mousePos)) {
                showGameInfo = true;
                showAuthorInfo = false;
            }
            else if (aboutAuthorBtn.getGlobalBounds().contains(mousePos)) {
                showGameInfo = false;
                showAuthorInfo = true;
            }
            else if (exitBtn.getGlobalBounds().contains(mousePos)) {
                exitRequested = true;
            }
            else if (helpBtn.getGlobalBounds().contains(mousePos)) {
                // Открытие справки в полноэкранном режиме
                SHELLEXECUTEINFO sei = { sizeof(sei) };
                sei.lpFile = L"Spravka.chm";
                sei.nShow = SW_SHOWMAXIMIZED; // Открыть в полноэкранном режиме
                sei.fMask = SEE_MASK_FLAG_NO_UI; // Скрыть диалоговые окна ошибок

                if (!ShellExecuteEx(&sei)) {
                    // Если не удалось открыть, попробуем стандартным способом
                    ShellExecute(NULL, L"open", L"Spravka.chm", NULL, NULL, SW_SHOWMAXIMIZED);
                }
            }
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            exitRequested = true;
        }
    }
}

void About::render(sf::RenderWindow& window) {
    window.clear();
    window.draw(background);
    window.draw(aboutGameBtn);
    window.draw(aboutAuthorBtn);
    window.draw(exitBtn);
    window.draw(helpBtn);

    if (showGameInfo) {
        window.draw(gameText);
    }
    else {
        window.draw(authorText);
    }

    window.display();
}

bool About::show(sf::RenderWindow& window) {
    exitRequested = false;

    while (window.isOpen() && !exitRequested) {
        handleEvents(window);
        render(window);
    }

    return exitRequested;
}