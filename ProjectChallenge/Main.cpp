#include "Menu.h"
#include "About.h"
#include <iostream>
#include "Material.h"

void Menu::loadResources() {
    // Загрузка шрифта для подсказки
    if (!font.loadFromFile("TDAText.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
    }
    exitTooltip.setFont(font);
    exitTooltip.setString(L"Выход из игры");
    exitTooltip.setCharacterSize(24);
    exitTooltip.setFillColor(sf::Color::White);

    // Загрузка текстур для выбора преподавателя
    if (!backgroundTexture.loadFromFile("img/chbk.png") ||
        !shatovTexture.loadFromFile("img/prepod/shatovaneutral1.png") ||
        !kizerTexture.loadFromFile("img/prepod/kizerneutral1.png") ||
        !lukshaTexture.loadFromFile("img/prepod/lukshaneutral1.png")) {
        std::cerr << "Failed to load teacher selection textures!" << std::endl;
    }

    // Настройка спрайтов преподавателей
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(
        1920.0f / backgroundTexture.getSize().x,
        1080.0f / backgroundTexture.getSize().y
    );

    shatovSprite.setTexture(shatovTexture);
    kizerSprite.setTexture(kizerTexture);
    lukshaSprite.setTexture(lukshaTexture);

  
    // Позиционирование преподавателей в ряд
    float teacherWidth = shatovTexture.getSize().x;
    float spacing = (1920.0f - teacherWidth * 3) / 4.0f;
    float yPos = 1080.0f / 2 - shatovTexture.getSize().y / 2;

    shatovSprite.setPosition(spacing, yPos);
    kizerSprite.setPosition(spacing * 2 + teacherWidth, yPos);
    lukshaSprite.setPosition(spacing * 3 + teacherWidth * 2, yPos);

    // Начальная прозрачность
    shatovSprite.setColor(sf::Color(0, 0, 0, 150));
    kizerSprite.setColor(sf::Color(0, 0, 0, 150));
    lukshaSprite.setColor(sf::Color(0, 0, 0, 150));

    // Настройка текста описания
    teacherDescription.setFont(font);
    teacherDescription.setCharacterSize(30);
    teacherDescription.setFillColor(sf::Color::White);

    // Загрузка фоновой анимации для главного меню
    for (int i = 1; i <= 6; ++i) {
        sf::Texture texture;
        if (!texture.loadFromFile("img/" + std::to_string(i) + "bk.png")) {
            std::cerr << "Failed to load background texture: " << i << "bk.png" << std::endl;
        }
        backgroundTextures.push_back(texture);
    }

    // Настройка анимированного фона
    animatedBackground.setTexture(backgroundTextures[0]);
    animatedBackground.setScale(
        1920.0f / backgroundTextures[0].getSize().x,
        1080.0f / backgroundTextures[0].getSize().y
    );

    // Загрузка текстур кнопок
    if (!menuTexture1.loadFromFile("img/begin.png") ||
        !menuTexture2.loadFromFile("img/mat.png") ||
        !menuTexture3.loadFromFile("img/spravka.png") ||
        !menuTexture1Hover.loadFromFile("img/beginpr.png") ||
        !menuTexture2Hover.loadFromFile("img/matpr.png") ||
        !menuTexture3Hover.loadFromFile("img/spravkapr.png") ||
        !exitButtonTexture.loadFromFile("img/exitbutton.png") ||
        !titleTexture.loadFromFile("img/title.png")) {
        std::cerr << "Failed to load menu textures!" << std::endl;
    }

    // Настройка спрайтов
    title.setTexture(titleTexture);
    title.setPosition(50, titleStartY);

    menu1.setTexture(menuTexture1);
    menu2.setTexture(menuTexture2);
    menu3.setTexture(menuTexture3);
    exitButton.setTexture(exitButtonTexture);
    exitButton.setPosition(1700, 50);

    // Новое позиционирование кнопок
    const float buttonHeight = menu1.getLocalBounds().height;
    const float verticalSpacing = 10.0f;
    const float startY = 100.0f;

    menu1.setPosition(100, startY);
    menu2.setPosition(100, startY + buttonHeight + verticalSpacing);
    menu3.setPosition(100, startY + 2 * (buttonHeight + verticalSpacing));

   
    if (!backgroundMusic.openFromFile("audio/kaudio.ogg")) {
        // Если не удалось загрузить музыку
    }
    else {
        // Настройки музыки:
        backgroundMusic.setLoop(true); // Включаем повтор
        backgroundMusic.setVolume(50); // Устанавливаем громкость (0-100)
        backgroundMusic.play(); // Начинаем воспроизведение
    }


    // Установка масштаба для всех элементов
    scaleElements(sf::Vector2f(scaleX, scaleY));
}

void Menu::showTeacherSelection(sf::RenderWindow& window) {
    sf::Clock clock;
    bool teacherSelected = false;

    // Настройка титульного текста
    sf::Text titleText(L"Выберите у какого преподавателя сдать экзамен", font, 50);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(1920.0f / 2 - titleText.getLocalBounds().width / 2, 50);

    // Настройка кнопки выхода
    sf::Sprite exitButton;
    exitButton.setTexture(exitButtonTexture);
    exitButton.setPosition(1700, 50);
    // Текст для подсказки кнопки выхода
    sf::Text exitHintText(L"В главное меню", font, 24);
    exitHintText.setFillColor(sf::Color::White);
    bool showExitHint = false;
    // Начальные цвета - черные полупрозрачные
    shatovSprite.setColor(sf::Color(0, 0, 0, 150));
    kizerSprite.setColor(sf::Color(0, 0, 0, 150));
    lukshaSprite.setColor(sf::Color(0, 0, 0, 150));

    // Центрирование преподавателей
    float teacherWidth = shatovTexture.getSize().x;
    float spacing = -30.0f;
    float totalWidth = teacherWidth * 3 + spacing * 2;
    float startX = (1920.0f - totalWidth) / 2 + 50;
    float yPos = 1080.0f / 2 - shatovTexture.getSize().y / 2;

    shatovSprite.setPosition(startX, yPos);
    kizerSprite.setPosition(startX + teacherWidth + spacing, yPos);
    lukshaSprite.setPosition(startX + (teacherWidth + spacing) * 2, yPos);

    // Основной цикл
    while (!teacherSelected && window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (exitButton.getGlobalBounds().contains(mousePos)) {
                   
                  //  exitTooltip.setPosition(mousePos.x - 120, mousePos.y + 20);
                  //  backgroundMusic.pause();
                    return; // Возврат в главное меню
                }

                // Проверка выбора преподавателя
                if (shatovSprite.getGlobalBounds().contains(mousePos)) {
                    selectedTeacher = 1;
                }
                else if (kizerSprite.getGlobalBounds().contains(mousePos)) {
                    selectedTeacher = 2;
                }
                else if (lukshaSprite.getGlobalBounds().contains(mousePos)) {
                    selectedTeacher = 3;
                }
                else {
                    continue; // Клик не по преподавателю
                }
                backgroundMusic.stop();
                // Запуск экзамена для выбранного преподавателя
                teacherSelected = true;
                Exam exam(selectedTeacher);
                exam.start(window);

                // После завершения экзамена:
                backgroundMusic.play();
                teacherSelected = false; // Сбрасываем выбор
                selectedTeacher = 0;
                return; // Возвращаемся в меню выбора преподавателя
            }
        }

        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        float deltaTime = clock.restart().asSeconds();
        float fadeSpeed = deltaTime * 5.0f;
        showExitHint = exitButton.getGlobalBounds().contains(mousePos);
        if (showExitHint) {
            exitHintText.setPosition(mousePos.x - 150, mousePos.y + 20);
        }
        // Текст специальности
        sf::Text specialtyText;
        specialtyText.setFont(font);
        specialtyText.setCharacterSize(24);
        specialtyText.setFillColor(sf::Color::White);
        bool showSpecialty = false;

        // Обработка преподавателей с плавной анимацией
        auto updateTeacher = [&](sf::Sprite& sprite, const std::wstring& specialty) {
            sf::Color color = sprite.getColor();
            bool isHovered = sprite.getGlobalBounds().contains(mousePos);

            if (isHovered) {
                // Плавное осветление
                color.r = std::min(255, static_cast<int>(color.r + 255 * fadeSpeed));
                color.g = std::min(255, static_cast<int>(color.g + 255 * fadeSpeed));
                color.b = std::min(255, static_cast<int>(color.b + 255 * fadeSpeed));
                color.a = std::min(255, static_cast<int>(color.a + 105 * fadeSpeed));

                specialtyText.setString(specialty);
                specialtyText.setPosition(mousePos.x - 150, mousePos.y + 20);
                showSpecialty = true;
            }
            else {
                // Плавное затемнение
                color.r = std::max(0, static_cast<int>(color.r - 255 * fadeSpeed));
                color.g = std::max(0, static_cast<int>(color.g - 255 * fadeSpeed));
                color.b = std::max(0, static_cast<int>(color.b - 255 * fadeSpeed));
                color.a = std::max(150, static_cast<int>(color.a - 105 * fadeSpeed));
            }

            sprite.setColor(color);
            };

        // Обновление преподавателей
        updateTeacher(shatovSprite, L"Татьяна Ивановна.\nОсновы алгоритмизации (С++)");
        updateTeacher(kizerSprite, L"Ольга Ивановна.\nИнструментальное ПО (Python)");
        updateTeacher(lukshaSprite, L"Ирина Лешековна.\nПрофессиональная математика");

        // Отрисовка
        window.clear();
        window.draw(backgroundSprite);
        window.draw(titleText);
        window.draw(exitButton);

        window.draw(shatovSprite);
        window.draw(kizerSprite);
        window.draw(lukshaSprite);
        if (showExitHint) {
            window.draw(exitHintText);
        }
        else if (showSpecialty) {
            window.draw(specialtyText);
        }


        window.display();
    }

    if (teacherSelected) {
        isMenuOpen = false;
    }
}

void Menu::updateAnimation(float deltaTime) {
    frameTimer += deltaTime;

    if (frameTimer >= frameDuration) {
        frameTimer = 0.0f;
        currentFrame = (currentFrame + 1) % 12;

        int textureIndex = 0;
        if (currentFrame < 4) {
            textureIndex = currentFrame;
        }
        else if (currentFrame < 8) {
            textureIndex = 4;
        }
        else {
            textureIndex = 5;
        }

        animatedBackground.setTexture(backgroundTextures[textureIndex]);
    }
}

void Menu::scaleElements(const sf::Vector2f& factors) {
    animatedBackground.setScale(factors.x, factors.y);
    menu1.setScale(factors.x, factors.y);
    menu2.setScale(factors.x, factors.y);
    menu3.setScale(factors.x, factors.y);
    exitButton.setScale(factors.x, factors.y);
    title.setScale(factors.x, factors.y);

    float scaledStartY = 200.0f * factors.y;
    float scaledVerticalSpacing = 5.0f * factors.y;
    float buttonHeight = menu1.getGlobalBounds().height;

    menu1.setPosition(100 * factors.x, scaledStartY);
    menu2.setPosition(100 * factors.x, scaledStartY + buttonHeight + scaledVerticalSpacing);
    menu3.setPosition(100 * factors.x, scaledStartY + 2 * (buttonHeight + scaledVerticalSpacing));
    exitButton.setPosition(1700 * factors.x, 50 * factors.y);
    title.setPosition(50 * factors.x, title.getPosition().y);
}

void Menu::updateTitleAnimation(float deltaTime) {
    if (titleAnimationTime < titleAnimationDuration) {
        titleAnimationTime += deltaTime;

        float progress = titleAnimationTime / titleAnimationDuration;
        if (progress > 1.0f) progress = 1.0f;

        float easedProgress = 1.0f - (1.0f - progress) * (1.0f - progress);
        float currentY = titleStartY + (titleTargetY - titleStartY) * easedProgress;
        title.setPosition(50, currentY);
    }
}

void Menu::handleEvents(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
            isMenuOpen = false;
        }

        if (event.type == sf::Event::MouseMoved) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            showExitTooltip = exitButton.getGlobalBounds().contains(mousePos);
        }

        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if (menu1.getGlobalBounds().contains(mousePos)) {
               // backgroundMusic.pause();
                selectingTeacher = true;
                showTeacherSelection(window);
            }
            else if (menu2.getGlobalBounds().contains(mousePos)) {
                Material materialScreen;
                materialScreen.show(window);
            }
            else if (menu3.getGlobalBounds().contains(mousePos)) {
                About aboutScreen;
                aboutScreen.show(window);
            }
            else if (exitButton.getGlobalBounds().contains(mousePos)) {
                window.close();
            }
        }

        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Escape) {
            window.close();
        }
    }
}

void Menu::render(sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    // Сброс состояний кнопок
    menu1.setTexture(menuTexture1);
    menu2.setTexture(menuTexture2);
    menu3.setTexture(menuTexture3);
    menuNum = 0;

    // Проверка наведения
    if (menu1.getGlobalBounds().contains(mousePos)) {
        menu1.setTexture(menuTexture1Hover);
        menuNum = 1;
    }
    else if (menu2.getGlobalBounds().contains(mousePos)) {
        menu2.setTexture(menuTexture2Hover);
        menuNum = 2;
    }
    else if (menu3.getGlobalBounds().contains(mousePos)) {
        menu3.setTexture(menuTexture3Hover);
        menuNum = 3;
    }

    // Позиционирование подсказки
    exitTooltip.setPosition(mousePos.x - 150, mousePos.y + 20);

    // Отрисовка
    window.clear();
    window.draw(animatedBackground);
    window.draw(menu1);
    window.draw(menu2);
    window.draw(menu3);
    window.draw(exitButton);
    window.draw(title);
    if (showExitTooltip) window.draw(exitTooltip);
    window.display();
}

void Menu::show(sf::RenderWindow& window) {
    loadResources();
    isMenuOpen = true;
    titleAnimationTime = 0.0f;
    sf::Clock clock;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        if (isMenuOpen) {
            handleEvents(window);
            updateAnimation(deltaTime);
            updateTitleAnimation(deltaTime);
            render(window);
        }
    }
}