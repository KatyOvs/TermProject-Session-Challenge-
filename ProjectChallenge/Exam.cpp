#include "Exam.h"
#include <iostream>
#include <random>
#include <fstream>
#include <locale>
#include <cwctype> // Для iswspace()
Exam::Exam(int teacherId) : teacherId(teacherId),
currentState(GameState::Entrance),
shouldExit(false),
showDarkOverlay(false),
isPaused(false),
ticketSelected(false),
luckyTicket(false),
transitioning(false),
transitionComplete(false),
selectedTicketIndex(0),
transitioningToTickets(false),
fadeAlpha(150.0f),
transitionAlpha(0.0f),
fadeInComplete(false),
greetedTeacher(false),
score(0), // Начинаем с 0 баллов
answer1Submitted(false),
answer2Submitted(false),
hoveredOption(-1),
showFail(false),
failAlpha(0.0f),
showSucces(false),
succesAlpha(0.0f),
heartAlreadyLost(false),
question3Completed(false),
// Флаг, что балл за приветствие уже начислен
scoreUpdatedForQuestion3(false) {
    std::locale::global(std::locale(""));
    loadResources();
    loadQuestionsForSubject();
}
void Exam::loadQuestionsForSubject() {
    // Загрузка вопросов для каждого предмета
    switch (teacherId) {
    case 1: // ОАиП
        testQuestions[0] = Question{
            "Название алгоритма, действия которого выполняются без ветвлений?",
            {"1. Разветвляющийся алгоритм", "2. Линейный алгоритм ", "3. Циклический алгоритм"},
            1 // Правильный ответ - второй вариант (индексация с 0)
        };
        testQuestions[1] = Question{
           "Какой заголовочный файл нужен для использования cout?",
            {"1. <iostream>", "2. <math.h>", "3. <stdio.h>"},
            0 // Правильный ответ - первый вариант
        };
        testQuestions[2] = Question{
            "Что означает ключевое слово 'const' в объявлении метода?",
            {"1. Метод не изменяет состояние объекта", "2. Метод возвращает константное значение", "3. Метод нельзя переопределять"},
            0 // Правильный ответ - первый вариант
        };
        codeQuestions[0] = CodeQuestion{ "img/code1.png", "if (x = 10) {" };
        codeQuestions[1] = CodeQuestion{ "img/code2.png", "string c = a + b + 5;" };
        codeQuestions[2] = CodeQuestion{ "img/code3.png", "cout << ::x; " };
        break;
    case 2: // ИПО
        testQuestions[0] = Question{
            "Что делает метод split() в Python?",
            {"1. Разделяет строку на подстроки и возвращает список",
             "2. Объединяет несколько строк в одну",
             "3. Удаляет пробелы в начале и конце строки"},
            0 // Правильный ответ - первый вариант
        };
        testQuestions[1] = Question{
            "Какой метод преобразует строку в список символов?",
            {"1. split()",
             "2. list()",
             "3. to_list()"},
            1 // Правильный ответ - второй вариант
        };
        testQuestions[2] = Question{
            "Какой результат вернет выражение \"Hello\".upper()?",
            {"1. \"hello\"",
             "2. \"HELLO\"",
             "3. \"Hello\""},
            1 // Правильный ответ - второй вариант перепишите
        };
        codeQuestions[0] = CodeQuestion{ "img/code4.png", "print(fruits[3])" };
        codeQuestions[1] = CodeQuestion{ "img/code5.png", "point [0] = 15 " };
        codeQuestions[2] = CodeQuestion{ "img/code6.png", "print(parsed[Name]) " };
        break;
    case 3: // Математика
        testQuestions[0] = Question{
           "Геометрический смысл производной функции в точке — это:",
            {"1. Угловой коэффициент касательной", "2. Интеграл функции", "3. Предел последовательности"},
            0
        };
        testQuestions[1] = Question{
            "Какая матрица называется квадратной?",
            {"1. Матрица, у которой одна строка", "2. Матрица, у которой все элементы равны нулю", "3. Матрица, у которой число строк равно числу столбцов "},
            2
        };
        testQuestions[2] = Question{
            "Что вычисляет интеграл?",
            {"1. Тангенс угла", "2. Площадь под кривой ", "3. Корни уравнения"},
            1
        };
        codeQuestions[0] = CodeQuestion{ "img/code7.png", "разность квадратов" };
        codeQuestions[1] = CodeQuestion{ "img/code8.png", "первый замечательный предел" };
        codeQuestions[2] = CodeQuestion{ "img/code9.png", "сумма ряда" };
        break;
    }
}
void Exam::loadResources() {

    switch (teacherId) {
    case 1: // Шатова
        backgroundTexture.loadFromFile("img/oaipdoc.png");
        teacherTexture.loadFromFile("img/prepod/shatovaneutral.png");
        teacherHappyTexture.loadFromFile("img/prepod/shatovahappy.png");
        teacherCheckTexture.loadFromFile("img/prepod/shatovachek.png");
        teacherAngryTexture.loadFromFile("img/prepod/shatovaangry.png");
        codePiecesTexture.loadFromFile("img/oaippieces.png");
        codeSolutionTexture.loadFromFile("img/oaipsolution.png");
        if (!oaipBasket1Texture.loadFromFile("img/oaip_basket1.png") ||
            !oaipBasket2Texture.loadFromFile("img/oaip_basket2.png") ||
            !oaipBasket3Texture.loadFromFile("img/oaip_basket3.png")) {
            std::cerr << "Failed to load basket textures!" << std::endl;
        }

        for (int i = 0; i < 9; ++i) {
            std::string path = "img/oaip_element" + std::to_string(i + 1) + ".png";
            if (!oaipElementTextures[i].loadFromFile(path)) {
                std::cerr << "Failed to load element texture: " << path << std::endl;
            }
        }
        break;
    case 2: // Кизер
        backgroundTexture.loadFromFile("img/ipodoc.png");
        teacherTexture.loadFromFile("img/prepod/kizerneutral.png");
        teacherHappyTexture.loadFromFile("img/prepod/kizerhappy.png");
        teacherCheckTexture.loadFromFile("img/prepod/kizerchek.png");
        teacherAngryTexture.loadFromFile("img/prepod/kizerangry.png");
        codePiecesTexture.loadFromFile("img/ipopieces.png");
        codeSolutionTexture.loadFromFile("img/iposolution.png");
        break;
    case 3: // Лукша
        backgroundTexture.loadFromFile("img/matemdoc.png");
        teacherTexture.loadFromFile("img/prepod/lukshaneutral.png");
        teacherHappyTexture.loadFromFile("img/prepod/lukshahappy.png");
        teacherCheckTexture.loadFromFile("img/prepod/lukshachek.png");
        teacherAngryTexture.loadFromFile("img/prepod/lukshaangry.png");
        codePiecesTexture.loadFromFile("img/mathpieces.png");
        codeSolutionTexture.loadFromFile("img/mathsolution.png");
        break;
    }
    // Загрузка музыки
    if (!examMusic.openFromFile("audio/examaudio.ogg")) {
        std::cerr << "Failed to load music!" << std::endl;
    }
    else {
        examMusic.setLoop(true);
        examMusic.setVolume(50);
        examMusic.play();
    }
    // Загрузка общих текстур
    dialogTexture.loadFromFile("img/dialogboard.png");
    heartTexture.loadFromFile("img/heart.png");
    helloButtonTexture.loadFromFile("img/hello.png");
    silenceButtonTexture.loadFromFile("img/silence.png");
    pauseButtonTexture.loadFromFile("img/pausebutton.png");
    saveAndGoTexture.loadFromFile("img/ango.png");
    comebackTexture.loadFromFile("img/comeback.png");
    stolTexture.loadFromFile("img/stol.png");
    biletTexture.loadFromFile("img/bilet.png");
    biletHoverTexture.loadFromFile("img/bilet_hover.png");
    questionBackgroundTexture.loadFromFile("img/1qbilet.png");
    exitButtonTexture.loadFromFile("img/exitbutton.png");

    //  submitButtonTexture.loadFromFile("img/submitbutton.png");
    submitButtonTexture2.loadFromFile("img/submitbutton.png");
    // Настройка спрайтов
    teacherCheck.setTexture(teacherCheckTexture);
    submitButton2.setTexture(submitButtonTexture2);
    submitButton2.setPosition(800, 800);
    background.setTexture(backgroundTexture);
    teacher.setTexture(teacherTexture);
    dialogBoard.setTexture(dialogTexture);
    pauseButton.setTexture(pauseButtonTexture);
    saveAndGoButton.setTexture(saveAndGoTexture);
    comebackButton.setTexture(comebackTexture);
    stol.setTexture(stolTexture);
    questionBackground.setTexture(questionBackgroundTexture);
    exitButton.setTexture(exitButtonTexture);
    //   submitButton.setTexture(submitButtonTexture);
       // Позиционирование
    pauseButton.setPosition(1700, 50);
    saveAndGoButton.setPosition(1920 / 2 - saveAndGoTexture.getSize().x / 2, 1080 / 2 - 100);
    comebackButton.setPosition(1920 / 2 - comebackTexture.getSize().x / 2, 1080 / 2 + 100);
    exitButton.setPosition(1700, 900);
    //  submitButton.setPosition(500, 550);
      // Настройка билетов
    for (int i = 0; i < 3; ++i) {
        sf::Sprite ticket;
        ticket.setTexture(biletTexture);
        ticket.setPosition(100 + i * 600, 200);
        tickets.push_back(ticket);
    }
    // Настройка сердечка

    heart.setTexture(heartTexture);
    heart.setTextureRect(sf::IntRect(0, 0, 150, heartTexture.getSize().y));
    heart.setPosition(50, 50);
    hearts.push_back(heart);

    // Настройка кнопок ответов
    helloButton.setTexture(helloButtonTexture);
    silenceButton.setTexture(silenceButtonTexture);
    helloButton.setPosition(200, 400);
    silenceButton.setPosition(200, 650);
    // Настройка шрифта и текста
    if (!font.loadFromFile("TDAText.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
    }
    // Инициализация текстовых элементов
    dialogText.setFont(font);
    dialogText.setCharacterSize(36);
    dialogText.setFillColor(sf::Color::White);
    dialogText.setPosition(200, 900);
    examTitleText.setFont(font);
    examTitleText.setCharacterSize(36);
    examTitleText.setFillColor(sf::Color::White);
    examTitleText.setPosition(200, 400);
    examTitleText.setString(
        L"[Вы толкаете тяжелую дверь аудитории, и холодный воздух встречает вас,\n"
        L"словно предупреждение. Ряды столов выстроились, как баррикады. За\n"
        L"каждым — бледные лица и сжатые кулаки. Впереди, на кафедре, лежат\n"
        L"зловещие стопки билетов. Преподаватель поднимает глаза, и этот взгляд\n"
        L"прожигает вас насквозь.]"
    );
    luckyText.setFont(font);
    luckyText.setCharacterSize(48);
    luckyText.setFillColor(sf::Color::Yellow);
   
    luckyText.setPosition(1920 / 2 - luckyText.getLocalBounds().width / 2, 200);
  
    questionText.setFont(font);
    questionText.setCharacterSize(36);
    questionText.setFillColor(sf::Color::Black);
    answerInputText.setFont(font);
    answerInputText.setCharacterSize(24);
    answerInputText.setFillColor(sf::Color::Black);
    // подсказко
    tooltipText.setFont(font);
    tooltipText.setCharacterSize(24);
    tooltipText.setFillColor(sf::Color::White);
    tooltipText.setStyle(sf::Text::Bold);

    // Добавьте после загрузки других текстур
    if (!failTexture.loadFromFile("img/fail.png")) {
        std::cerr << "Failed to load fail texture!" << std::endl;
    }
    failSprite.setTexture(failTexture);
    failSprite.setPosition(1920 / 2 - failTexture.getSize().x / 2, 1080 / 2 - failTexture.getSize().y / 2);
    failSprite.setColor(sf::Color(255, 255, 255, 0)); // Начальная прозрачность
    if (!successTexture.loadFromFile("img/succes.png")) {
        std::cerr << "Failed to load fail texture!" << std::endl;
    }
    succesSprite.setTexture(successTexture);
    succesSprite.setPosition(1920 / 2 - successTexture.getSize().x / 2, 1080 / 2 - successTexture.getSize().y / 2);
    succesSprite.setColor(sf::Color(255, 255, 255, 0)); // Начальная прозрачность

}
void Exam::start(sf::RenderWindow& window) {

    sf::Clock clock;
    while (window.isOpen() && !shouldExit) {
        float deltaTime = clock.restart().asSeconds();
        handleEvents(window);
        update(deltaTime);
        render(window);
    }

}
void Exam::handleEvents(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        if (currentState == GameState::Question2 && event.type == sf::Event::TextEntered) {
            if (event.text.unicode == '\b') {
                if (!currentAnswerW.empty()) {
                    currentAnswerW.pop_back();
                }
            }
            else if (event.text.unicode < 128 || event.text.unicode >= 1024) { // Разрешаем ASCII и кириллицу
                currentAnswerW += static_cast<wchar_t>(event.text.unicode);
            }
            answerInputText.setString(currentAnswerW);
        }
        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            showTooltip = false;
            // Обработка паузы
            if (pauseButton.getGlobalBounds().contains(mousePos) && !isPaused) {
                isPaused = true;
                continue;
            }
            if (isPaused) {
                if (saveAndGoButton.getGlobalBounds().contains(mousePos)) {
                    shouldExit = true;
                    return;
                }
                else if (comebackButton.getGlobalBounds().contains(mousePos)) {
                    isPaused = false;
                    dialogText.setString(L"");
                    continue;
                }
            }
            if (!isPaused) {
                if (currentState == GameState::Entrance) {
                    fadeInComplete = true;
                    currentState = GameState::Greeting;
                    dialogText.setString(L"...");
                    showDarkOverlay = true;
                }
                else if (currentState == GameState::Greeting) {
                    if (helloButton.getGlobalBounds().contains(mousePos)) {
                        teacher.setTexture(teacherHappyTexture);
                        greetedTeacher = true;
                        greetingBonusAdded = true;

                        score = 1;
                     
                        switch (teacherId) {
                        case 1:
                            dialogText.setString(L"Здравствуй!\nБерите свой билет и присаживайтесь.");
                            break;
                        case 2:
                            dialogText.setString(L"Здравствуй-здравствуй!\nДавай-ка вытягивай билетик.");
                            break;
                        case 3:
                            dialogText.setString(L"Здравствуй!\nБери билет и готовься.");
                            break;
                        }
                        currentState = GameState::WaitingForClickAfterGreeting;
                    }
                    else if (silenceButton.getGlobalBounds().contains(mousePos)) {

                        score = 0;
                    

                        switch (teacherId) {
                        case 1:
                            dialogText.setString(L"...\nЭто не спасет тебя от билета. Вытягивай.");
                            break;
                        case 2:
                            dialogText.setString(L"Не стесняйся!\nПросто выбери билетик.");
                            break;
                        case 3:
                            dialogText.setString(L"Молчание - золото \nНо билет тянуть надо.");
                            break;
                        }
                        currentState = GameState::WaitingForClickAfterGreeting;
                    }
                }
                else if (currentState == GameState::WaitingForClickAfterGreeting) {
                    transitioningToTickets = true;
                    transitionAlpha = 0.0f;
                }
                else if (currentState == GameState::TicketSelection && !ticketSelected) {
                    for (int i = 0; i < tickets.size(); ++i) {
                        if (tickets[i].getGlobalBounds().contains(mousePos)) {
                            ticketSelected = true;
                            selectedTicketIndex = i;
                            // 5% шанс на счастливый билет
                            std::random_device rd;
                            std::mt19937 gen(rd());
                            std::uniform_int_distribution<> dis(1, 100);
                            luckyTicket = (dis(gen) <= 5);
                            if (luckyTicket) {
                                tickets[i].setTexture(biletHoverTexture);
                                score = 10; // Автоматические 10 баллов
                              
                            }
                            transitioning = true;
                            transitionAlpha = 0.0f;
                            break;
                        }
                    }
                }
                // переходы:
                else if (currentState == GameState::Question1) {
                    // Проверка нажатия на варианты ответов по невидимым областям
                    for (int i = 0; i < optionAreas.size(); ++i) {
                        if (optionAreas[i].getGlobalBounds().contains(mousePos)) {
                            checkAnswer1(i);
                            break;
                        }
                    }
                    // Проверка нажатия на кнопку выхода (переход к вопросу 2)
                    if (exitButton.getGlobalBounds().contains(mousePos) && answer1Submitted) {
                        currentAnswerW.clear();
                        answerInputText.setString(L"");
                        questionBackgroundTexture.loadFromFile("img/2qbilet.png");
                        questionBackground.setTexture(questionBackgroundTexture);
                        setupQuestion2();
                        currentState = GameState::Question2;
                    }
                }
                else if (currentState == GameState::Question2) {
                    // Проверка нажатия на кнопку отправки ответа
                    if (exitButton.getGlobalBounds().contains(mousePos)) {
                        checkAnswer2(currentAnswer);
                    }
                    // Проверка нажатия на кнопку выхода (завершение экзамена)
                    if (exitButton.getGlobalBounds().contains(mousePos) && answer2Submitted) {
                        questionBackgroundTexture.loadFromFile("img/3qbilet.png");
                        questionBackground.setTexture(questionBackgroundTexture);
                        setupQuestion3();
                        currentState = GameState::Question3;
                    }
                }
            }
        }
        else if (event.type == sf::Event::MouseMoved) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            // Обработка наведения на кнопки в меню паузы lucky
            if (isPaused) {

                if (saveAndGoButton.getGlobalBounds().contains(mousePos)) {
                    tooltipText.setString(L"Нельзя покидать кабинет во время экзамена! Но здесь можно. Только ваши ответы я не сохраню :)");
                    showTooltip = true;
                }
                else if (comebackButton.getGlobalBounds().contains(mousePos)) {
                    tooltipText.setString(L"Продолжаем экзамен");
                    showTooltip = true;
                }
                else {
                    showTooltip = false;
                }
            }
            if (currentState == GameState::TicketSelection && !ticketSelected) {
                for (auto& ticket : tickets) {
                    if (ticket.getGlobalBounds().contains(mousePos)) {
                        ticket.setTexture(biletHoverTexture);
                    }
                    else {
                        ticket.setTexture(biletTexture);
                    }
                }
            }
            else if (currentState == GameState::Question1) {
                int newHovered = -1;
                for (int i = 0; i < optionAreas.size(); ++i) {
                    if (optionAreas[i].getGlobalBounds().contains(mousePos)) {
                        newHovered = i;
                        break;
                    }
                }
                hoveredOption = newHovered;
            }
        }
        //реализация 3 вопроса
        if (currentState == GameState::Question3 &&
            event.type == sf::Event::MouseButtonPressed) {

            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            static std::random_device rd;
            static std::mt19937 gen(rd());
            switch (question3Type) {
            case Question3Type::ClickBugs:
                // Инициализация генератора случайных чисел


                for (int i = 0; i < bugs.size(); ++i) {
                    if (!bugsClicked[i] && bugs[i].getGlobalBounds().contains(mousePos)) {
                        bugClickCounts[i]--;

                        if (bugClickCounts[i] <= 0) {
                            bugsClicked[i] = true;
                            // Если все баги/точка уничтожены
                            bool allFixed = true;
                            for (bool clicked : bugsClicked) {
                                if (!clicked) {
                                    allFixed = false;
                                    break;
                                }
                            }

                            if (allFixed) {
                                codeImageb.setTexture(codeNoBugsTexture);
                                if (teacherId == 3) {
                                    score += 3;
                                   
                                }
                            }
                        }
                        else {
                            // Генерация новых случайных координат для бага
                            sf::FloatRect area(200, 250, 900, 500);
                            std::uniform_real_distribution<float> xDist(area.left, area.left + area.width);
                            std::uniform_real_distribution<float> yDist(area.top, area.top + area.height);

                            bugs[i].setPosition(xDist(gen), yDist(gen));
                        }

                        if (teacherId == 3) {
                            // Определяем область для перемещения точки
                            sf::FloatRect area(200, 250, 900, 500);
                            std::uniform_real_distribution<float> xDist(area.left, area.left + area.width);
                            std::uniform_real_distribution<float> yDist(area.top, area.top + area.height);

                            bugs[i].setPosition(xDist(gen), yDist(gen));
                        }
                        break;
                    }
                } break;
                //экзамен
            case Question3Type::AssembleCode:
                if (event.type == sf::Event::MouseButtonPressed) {
                    for (auto& piece : codePieces) {
                        if (!piece.isCorrect && piece.sprite.getGlobalBounds().contains(mousePos)) {
                            piece.isDragging = true;
                            piece.offset = piece.sprite.getPosition() - mousePos;
                            break;
                        }
                    }
                }
                else if (event.type == sf::Event::MouseButtonReleased) {
                    for (auto& piece : codePieces) {
                        if (piece.isDragging) {
                            piece.isDragging = false;

                            // Проверяем расстояние до правильной позиции
                            float distance = std::sqrt(
                                std::pow(piece.sprite.getPosition().x - piece.correctPosition.x, 2) +
                                std::pow(piece.sprite.getPosition().y - piece.correctPosition.y, 2));

                            if (distance < 50) { // Если близко к правильной позиции
                                piece.isCorrect = true;
                                piece.sprite.setPosition(piece.correctPosition); // Фиксируем в правильном месте
                            }
                            else {
                                // Возвращаем на исходную позицию
                                piece.sprite.setPosition(piece.originalPosition);
                            }
                        }
                    }
                    checkQuestion3Completion();
                }
                else if (event.type == sf::Event::MouseMoved) {
                    for (auto& piece : codePieces) {
                        if (piece.isDragging && !piece.isCorrect) {
                            piece.sprite.setPosition(mousePos + piece.offset);
                        }
                    }
                }
                break;
            case Question3Type::SortElements:
                // Обработка перетаскивания элементов для сортировки
                for (auto& piece : codePieces) {
                    if (piece.sprite.getGlobalBounds().contains(mousePos)) {
                        piece.isDragging = true;
                        break;
                    }
                }
                break;
            }
            // Проверка нажатия на кнопку сдачи (только если задание завершено)
            if (submitButton2.getGlobalBounds().contains(mousePos)) {
                currentState = GameState::ExamComplete;
                teacher.setTexture(teacherCheckTexture);
                dialogText.setString(L"...");
                showDarkOverlay = true;
            }
        }
        else if (event.type == sf::Event::MouseButtonReleased) {
            switch (question3Type) {
            case Question3Type::AssembleCode:
                // Проверка правильности расположения элементов кода
                for (auto& piece : codePieces) {
                    if (piece.isDragging) {
                        piece.isDragging = false;

                        // Проверяем расстояние до правильной позиции
                        float distance = std::sqrt(
                            std::pow(piece.sprite.getPosition().x - piece.correctPosition.x, 2) +
                            std::pow(piece.sprite.getPosition().y - piece.correctPosition.y, 2));

                        if (distance < 50) { // Если близко к правильной позиции
                            piece.isCorrect = true;
                            piece.sprite.setPosition(piece.correctPosition); // Фиксируем в правильном месте
                        }
                        else {
                            // Возвращаем на исходную позицию
                            piece.sprite.setPosition(piece.originalPosition);
                        }
                    }
                }
                break;
            case Question3Type::SortElements:
                // Обработка перетаскивания элементов для сортировки
                if (event.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    for (auto& piece : codePieces) {
                        if (!piece.isCorrect && piece.sprite.getGlobalBounds().contains(mousePos)) {
                            piece.isDragging = true;
                            piece.offset = piece.sprite.getPosition() - mousePos;
                            break;
                        }
                    }
                }
                else if (event.type == sf::Event::MouseButtonReleased) {
                    for (auto& piece : codePieces) {
                        if (piece.isDragging) {
                            piece.isDragging = false;
                            // Проверка будет выполнена в checkQuestion3Completion()
                        }
                    }
                    checkQuestion3Completion();
                }
                else if (event.type == sf::Event::MouseMoved) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    for (auto& piece : codePieces) {
                        if (piece.isDragging) {
                            piece.sprite.setPosition(mousePos + piece.offset);
                        }
                    }
                }
                break;
            }
            // Проверка завершения задания start
            checkQuestion3Completion();
        }
        else if (event.type == sf::Event::MouseMoved) {
            // Обработка перетаскивания для соответствующих мини-игр
            if (question3Type == Question3Type::AssembleCode || question3Type == Question3Type::SortElements) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                for (auto& piece : codePieces) {
                    if (piece.isDragging) {
                        piece.sprite.setPosition(mousePos.x - 50, mousePos.y - 50);
                    }
                }
            }
        }

        else if (currentState == GameState::ExamComplete) {
            if (event.type == sf::Event::MouseButtonPressed) {
                // Проверяем баллы и меняем спрайт преподавателя
                if (score > 4) { // Если сдали (4 и более баллов)
                    teacher.setTexture(teacherHappyTexture);
                    // Устанавливаем разные реплики для каждого преподавателя lucky
                    switch (teacherId) {
                    case 1: // Шатова
                        dialogText.setString(L"Поздравляю! Вы сдали экзамен на  " + std::to_wstring(score) + L" баллов.\nДо встречи на настоящем.");
                        break;
                    case 2: // Кизер
                        dialogText.setString(L"Отлично сдано! " + std::to_wstring(score) + L" баллов!\nДо встречи на следующем курсе!");
                        break;
                    case 3: // Лукша
                        dialogText.setString(L"Хорошая работа. Ставлю " + std::to_wstring(score) + L"\nВидно что сами писали");
                        break;
                    }
                    showSucces = true;
                    succesAlpha = 0.0f;
                    currentState = GameState::ReturnToMenu2;

                }
                else { // Если не сдали
                    // Уменьшаем количество жизней

                    for (auto& heart : hearts) {
                        heart.setTextureRect(sf::IntRect(150, 0, 150, heartTexture.getSize().y));
                    }
                    teacher.setTexture(teacherAngryTexture);
                    // Устанавливаем разные реплики для каждого преподавателя
                    switch (teacherId) {
                    case 1: // Шатова
                        dialogText.setString(L"Это провал. Приходите на пересдачу\nи подготовьтесь лучше. Оценка " + std::to_wstring(score) + L" не результат");
                        break;
                    case 2: // Кизер
                        dialogText.setString(L"Ой-ой... " + std::to_wstring(score) + L" балллов...\nНо не расстраивайтесь, попробуйте ещё раз!");
                        break;
                    case 3: // Лукша
                        dialogText.setString(L"Разве вы на " + std::to_wstring(score) + L" учились весь год?\nГотовьтесь к пересдаче.");
                        break;
                    }

                    showFail = true;
                    failAlpha = 0.0f;
                    currentState = GameState::ReturnToMenu;

                }
            }
        }
        else if (currentState == GameState::ReturnToMenu) {
            if (event.type == sf::Event::MouseButtonPressed) {
                shouldExit = true; // Это приведет к выходу из экзамена и возврату в меню
            }
        }
        else if (currentState == GameState::ReturnToMenu2) {
            if (event.type == sf::Event::MouseButtonPressed) {
                shouldExit = true; // Это приведет к выходу из экзамена и возврату в меню
            }
        }

    }
}
//ПРОВЕРКА 3 ВОПРОСА
void Exam::checkQuestion3Completion() {
    bool completed = true;
    switch (question3Type) {
    case Question3Type::ClickBugs:
        for (bool clicked : bugsClicked) {
            if (!clicked) {
                completed = false;
                break;
            }
        }
        break;

    case Question3Type::AssembleCode:
        for (const auto& piece : codePieces) {
            if (!piece.isCorrect) {
                completed = false;
                break;
            }
        }
        break;

    case Question3Type::SortElements:
        for (int basketIdx = 0; basketIdx < baskets.size(); ++basketIdx) {
            for (int elemIdx : basketItems[basketIdx]) {
                if (!baskets[basketIdx].getGlobalBounds().intersects(
                    codePieces[elemIdx].sprite.getGlobalBounds())) {
                    completed = false;
                    break;
                }
            }
            if (!completed) break;
        }
        break;
    }

    if (completed) {
        if (!scoreAddedForQuestion3) {
            score += 3; // +3 за правильный ответ (только если еще не начисляли)
            scoreAddedForQuestion3 = true;
            question3Completed = true;
            submitButton2.setColor(sf::Color::White);
        }
    }
    else {
        if (scoreAddedForQuestion3) {
            score -= 3; // Снимаем баллы, если ранее начисляли за этот вопрос
            scoreAddedForQuestion3 = false;
            question3Completed = false;
        }
    }

   
}
void Exam::update(float deltaTime) {
    if (!fadeInComplete && fadeAlpha > 0) {
        fadeAlpha -= deltaTime * 100.0f;
        if (fadeAlpha <= 0) {
            fadeAlpha = 0;
            fadeInComplete = true;
        }
    }
    if (transitioningToTickets) {
        transitionAlpha += deltaTime * 200.0f;
        if (transitionAlpha >= 255.0f) {
            transitionAlpha = 255.0f;
            transitioningToTickets = false;
            showDarkOverlay = false;
            currentState = GameState::TicketSelection;
        }
    }
    if (transitioning) {
        transitionAlpha += deltaTime * 200.0f;
        if (transitionAlpha >= 255.0f) {
            transitionAlpha = 255.0f;
            transitioning = false;
            transitionComplete = true;
            if (luckyTicket) {
                currentState = GameState::ExamComplete;
            }
            else {
                setupQuestion1();
                currentState = GameState::Question1;
            }
        }
    }
    if (showFail && failAlpha < 255.0f) {
        failAlpha += deltaTime * 100.0f;
        if (failAlpha >= 255.0f) {
            failAlpha = 255.0f;
        }
        failSprite.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(failAlpha)));
    }
    if (showSucces && succesAlpha < 255.0f) {
        succesAlpha += deltaTime * 100.0f;
        if (succesAlpha >= 255.0f) {
            succesAlpha = 255.0f;
        }
        succesSprite.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(succesAlpha)));
    }
}


void Exam::setupQuestion1() {
    selectedOption = -1;
    answer1Submitted = false;
    scoreAddedForQuestion1 = false;
    currentTestQuestion = testQuestions[selectedTicketIndex];
    questionText.setCharacterSize(42);
    questionText.setString(sf::String(currentTestQuestion.text));
    questionText.setPosition(200, 200);

    optionAreas.clear();
    for (int i = 0; i < 3; ++i) {
        sf::RectangleShape optionArea;
        optionArea.setSize(sf::Vector2f(600, 80));
        optionArea.setPosition(200, 300 + i * 100);
        optionArea.setFillColor(sf::Color::Transparent);
        optionAreas.push_back(optionArea);

        optionTexts[i].setFont(font);
        optionTexts[i].setString(sf::String(currentTestQuestion.options[i]));
        optionTexts[i].setCharacterSize(36);
        optionTexts[i].setPosition(250, 310 + i * 100);
        optionTexts[i].setFillColor(sf::Color::Black);
    }

}
void Exam::setupQuestion2() {
    answer2Submitted = false;
    scoreAddedForQuestion2 = false;
    currentAnswerW.clear();
    currentCodeQuestion = codeQuestions[selectedTicketIndex];
    // Используем путь из текущего codeQuestion вместо загрузки новых изображений
    if (!codeTexture.loadFromFile(currentCodeQuestion.imagePath)) {
        std::cerr << "Failed to load code image from: " << currentCodeQuestion.imagePath << std::endl;
    }
    codeImage.setTexture(codeTexture);
    codeImage.setPosition(200, 250);
    questionText.setCharacterSize(42);
    if (teacherId == 3) {
        questionText.setString(L"2. Напишите название формулы:");
    }
    else { questionText.setString(L"2. Перепишите полностью строку в которой есть ошибка:"); }
    questionText.setPosition(200, 150);
    questionText.setFillColor(sf::Color::Black);
    answerInputText.setString(L"");
    answerInputText.setPosition(200, 500);
    answerInputText.setFillColor(sf::Color::Black);
}
void Exam::setupQuestion3() {
    question3Completed = false;
    scoreAddedForQuestion3 = false;
    // Определяем сложность билета (0 - легкий, 1 - средний, 2 - сложный)
    int difficulty = selectedTicketIndex % 3;
    // Загрузка фона вопроса
    questionBackgroundTexture.loadFromFile("img/3qbilet.png");
    questionBackground.setTexture(questionBackgroundTexture);
    // Настройка кнопки submit бал
    submitButton2.setPosition(800, 800);
    switch (difficulty) {
    case 0: // Легкий билет - клик по багам
        question3Type = Question3Type::ClickBugs;
        setupClickBugsGame();
        break;
    case 1: // Средний билет - собрать код
        question3Type = Question3Type::AssembleCode;
        setupAssembleCodeGame();
        break;
    case 2: // Сложный билет - сортировка по корзинам
        question3Type = Question3Type::SortElements;
        setupSortElementsGame();
        break;
    }
}
void Exam::setupClickBugsGame() {
    // Загрузка текстур для кода с багами и без
    std::string codePathWithBugs, codePathNoBugs;

    switch (teacherId) {
    case 1: // ОАиП
        codePathWithBugs = "img/oaipbugs.png";
        codePathNoBugs = "img/oaipnobugs.png";
        break;

    case 2: // ИПО
        codePathWithBugs = "img/ipobugs.png";
        codePathNoBugs = "img/iponobugs.png";
        break;

    case 3: // Математика - особый случай
        codePathWithBugs = "img/matherror.png";
        codePathNoBugs = "img/mathcorrect.png";
        break;
    }

    // Загружаем текстуры
    if (!codeWithBugsTexture.loadFromFile(codePathWithBugs) ||
        !codeNoBugsTexture.loadFromFile(codePathNoBugs)) {
        std::cerr << "Failed to load code images!" << std::endl;
    }

    // Устанавливаем позицию картинки
    codeImageb.setTexture(codeWithBugsTexture);
    codeImageb.setPosition(200, 250);
    codeImageb.setScale(900.0f / codeWithBugsTexture.getSize().x,
        500.0f / codeWithBugsTexture.getSize().y);

    // Загрузка текстуры бага/точки
    if (!bugTexture.loadFromFile(teacherId == 3 ? "img/mathdot.png" : "img/bug.png")) {
        std::cerr << "Failed to load bug/dot texture!" << std::endl;
    }

    // Настройка багов/точек
    bugs.clear();
    bugsClicked.clear();
    bugClickCounts.clear();

    sf::FloatRect bugArea(
        codeImageb.getPosition().x,
        codeImageb.getPosition().y,
        900,
        500
    );

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> xDist(bugArea.left, bugArea.left + bugArea.width);
    std::uniform_real_distribution<float> yDist(bugArea.top, bugArea.top + bugArea.height);

    switch (teacherId) {
    case 1:
    case 2:
        // Обычные баги для ОАиП и ИПО
        for (int i = 0; i < 5; ++i) {
            sf::Sprite bug;
            bug.setTexture(bugTexture);
            bug.setPosition(xDist(gen), yDist(gen));
            bug.setScale(0.7f, 0.7f);
            bugs.push_back(bug);
            bugsClicked.push_back(false);
            bugClickCounts.push_back(std::uniform_int_distribution<>(2, 4)(gen));
        }
        questionText.setString(L"Щелкайте по багам чтобы спасти код!");
        break;

    case 3:
        // Особый случай для математики - одна точка
        sf::Sprite dot;
        dot.setTexture(bugTexture);
        dot.setScale(1.0f, 1.0f);
        dot.setPosition(xDist(gen), yDist(gen));
        bugs.push_back(dot);
        bugsClicked.push_back(false);
        bugClickCounts.push_back(3); // Нужно 3 клика
        questionText.setString(L"С помощью кликов поставьте точку на место!");
        break;
    }

    questionText.setCharacterSize(36);
    questionText.setPosition(200, 150);
    questionText.setFillColor(sf::Color::Black);
}
void Exam::setupAssembleCodeGame() {
    // Очищаем предыдущие элементы
    codePieces.clear();
    pieceTextures.clear(); // Очищаем старые текстуры

    // Загрузка изображения с правильным решением
    std::string solutionFile;
    switch (teacherId) {
    case 1: solutionFile = "img/oaipsolution.png"; break;
    case 2: solutionFile = "img/iposolution.png"; break;
    case 3: solutionFile = "img/mathsolution.png"; break;
    default: solutionFile = "img/mathsolution.png"; break;
    }

    if (!codeSolutionTexture.loadFromFile(solutionFile)) {
        std::cerr << "Failed to load code solution image!" << std::endl;
    }

    // Создаем серую подложку
    sf::Image solutionImage = codeSolutionTexture.copyToImage();


    // Создаем текстуру для серой подложки
    sf::Texture grayBackgroundTexture;
    grayBackgroundTexture.loadFromImage(solutionImage);

    completedCodeImage.setTexture(grayBackgroundTexture);
    completedCodeImage.setPosition(400, 300);
    completedCodeImage.setColor(sf::Color(150, 150, 150, 150));

    // Определяем размеры и количество частей
    int pieceCount = 0;
    sf::Vector2u textureSize = codeSolutionTexture.getSize();
    int pieceWidth = textureSize.x / (teacherId == 1 ? 7 : (teacherId == 2 ? 6 : 4));
    int pieceHeight = textureSize.y;

    switch (teacherId) {
    case 1: pieceCount = 7; break;
    case 2: pieceCount = 6; break;
    case 3: pieceCount = 4; break;
    }

    // Создаем текстуры для каждой части (сохраняем в член класса)
    pieceTextures.resize(pieceCount);
    for (int i = 0; i < pieceCount; ++i) {
        if (!pieceTextures[i].loadFromImage(solutionImage,
            sf::IntRect(i * pieceWidth, 0, pieceWidth, pieceHeight))) {
            std::cerr << "Failed to load piece " << i << " texture!" << std::endl;
        }
    }

    // Параметры для позиционирования
    float startX = 600;
    float startY = 300;
    float spacing = 1.0f;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> xDist(200, 1200);
    std::uniform_int_distribution<> yDist(400, 700);

    // Создаем перетаскиваемые части
    for (int i = 0; i < pieceCount; ++i) {
        DraggablePiece piece;
        piece.sprite.setTexture(pieceTextures[i]); // Используем сохраненную текстуру

        // Исходная позиция
        piece.sprite.setPosition(xDist(gen), yDist(gen));
        piece.originalPosition = piece.sprite.getPosition();

        // Правильная позиция
        piece.correctPosition = sf::Vector2f(
            startX + i * (pieceWidth + spacing),
            startY
        );
        piece.isCorrect = false;
        piece.width = pieceWidth;
        piece.height = pieceHeight;

        codePieces.push_back(piece);
    }

    // Текст инструкции
    questionText.setCharacterSize(36);
    questionText.setString(teacherId == 3 ? L"3. Соберите формулу по частям" : L"3. Соберите код по частям");
    questionText.setPosition(200, 150);
    questionText.setFillColor(sf::Color::Black);
}
void Exam::setupSortElementsGame() {
    // Очищаем предыдущие элементы
    baskets.clear();
    codePieces.clear();
    basketItems.clear();

    // Загрузка текстур корзинок и элементов в зависимости от предмета
    std::vector<std::string> basketTextures;
    std::vector<std::string> elementTextures;

    switch (teacherId) {
    case 1: // ОАиП
        basketTextures = { "img/oaip_busket1.png", "img/oaip_busket2.png", "img/oaip_busket3.png" };
        elementTextures = {
            "img/oaip_element1.png", "img/oaip_element2.png", "img/oaip_element3.png",
            "img/oaip_element4.png", "img/oaip_element5.png", "img/oaip_element6.png",
            "img/oaip_element7.png", "img/oaip_element8.png", "img/oaip_element9.png"
        };
        questionText.setString(L"Распределите элементы ОАиП по соответствующим корзинам");
        break;
    case 2: // ИПО
        basketTextures = { "img/ipo_busket1.png", "img/ipo_busket2.png", "img/ipo_busket3.png" };
        elementTextures = {
            "img/ipo_element1.png", "img/ipo_element2.png", "img/ipo_element3.png",
            "img/ipo_element4.png", "img/ipo_element5.png", "img/ipo_element6.png",
            "img/ipo_element7.png", "img/ipo_element8.png", "img/ipo_element9.png"
        };
        questionText.setString(L"Распределите элементы ИПО по соответствующим корзинам");
        break;
    case 3: // Математика
        basketTextures = { "img/math_busket1.png", "img/math_busket2.png", "img/math_busket3.png" };
        elementTextures = {
            "img/math_element1.png", "img/math_element2.png", "img/math_element3.png",
            "img/math_element4.png", "img/math_element5.png", "img/math_element6.png",
            "img/math_element7.png", "img/math_element8.png", "img/math_element9.png"
        };
        questionText.setString(L"Распределите математические элементы по соответствующим корзинам");
        break;
    }

    // Загрузка и настройка корзинок
    for (int i = 0; i < 3; ++i) {
        sf::Texture* texture = new sf::Texture(); // Динамическое выделение памяти
        if (!texture->loadFromFile(basketTextures[i])) {
            std::cerr << "Failed to load basket texture: " << basketTextures[i] << std::endl;
            delete texture;
            continue;
        }

        // Сохраняем указатель на текстуру в векторе, чтобы не потерять
        basketTexturesStorage.push_back(texture);

        sf::Sprite basket;
        basket.setTexture(*texture);
        basket.setPosition(300 + i * 500, 300);
        baskets.push_back(basket);
    }

    // Загрузка элементов
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> xDist(200, 1500);
    std::uniform_int_distribution<> yDist(500, 650);

    for (size_t i = 0; i < elementTextures.size(); ++i) {
        sf::Texture* texture = new sf::Texture(); // Динамическое выделение памяти
        if (!texture->loadFromFile(elementTextures[i])) {
            std::cerr << "Failed to load element texture: " << elementTextures[i] << std::endl;
            delete texture;
            continue;
        }

        // Сохраняем указатель на текстуру
        elementTexturesStorage.push_back(texture);

        DraggablePiece piece;
        piece.sprite.setTexture(*texture);
        piece.sprite.setPosition(xDist(gen), yDist(gen));
        piece.originalPosition = piece.sprite.getPosition();
        piece.isCorrect = false;
        codePieces.push_back(piece);
    }

    // Определение принадлежности элементов к корзинам (3 элемента на корзину)
    basketItems.resize(3);
    for (size_t i = 0; i < codePieces.size(); ++i) {
        basketItems[i / 3].push_back(i); // Первые 3 - в корзину 0, следующие 3 - в корзину 1 и т.д.
    }

    // Настройка текста вопроса
    questionText.setCharacterSize(36);
    questionText.setPosition(200, 150);
    questionText.setFillColor(sf::Color::Black);
}

//ПРОВЕРКА 1 И 2 ВОПРОСАААААААА
void Exam::checkAnswer1(int selectedOption) {
    this->selectedOption = selectedOption;

    if (selectedOption == currentTestQuestion.correctOption) {
        if (!scoreAddedForQuestion1) {
            score += 3; // +3 за правильный ответ (только если еще не начисляли)
            scoreAddedForQuestion1 = true;
            dialogText.setString(L"Правильно! +3 балла");
        }
        else {
            dialogText.setString(L"Правильно! (баллы уже начислены)");
        }
    }
    else {
        if (scoreAddedForQuestion1) {
            score -= 3; // Снимаем баллы, если ранее начисляли за этот вопрос
            scoreAddedForQuestion1 = false;
        }
        dialogText.setString(L"Неправильно! Правильный ответ: " +
            sf::String(currentTestQuestion.options[currentTestQuestion.correctOption]));
    }

    answer1Submitted = true;
   
}

void Exam::checkAnswer2(const std::string& answer) {
    std::wstring userAnswerW = currentAnswerW;
    std::wstring correctAnswerW;
    correctAnswerW.assign(currentCodeQuestion.correctAnswer.begin(), currentCodeQuestion.correctAnswer.end());

    auto removeSpaces = [](std::wstring& str) {
        str.erase(std::remove_if(str.begin(), str.end(),
            [](wchar_t c) { return std::iswspace(c); }),
            str.end());
        return str;
        };

    userAnswerW = removeSpaces(userAnswerW);
    std::transform(userAnswerW.begin(), userAnswerW.end(), userAnswerW.begin(), ::towlower);
    correctAnswerW = removeSpaces(correctAnswerW);
    std::transform(correctAnswerW.begin(), correctAnswerW.end(), correctAnswerW.begin(), ::towlower);

    bool isCorrect = (userAnswerW == correctAnswerW);

    if (isCorrect) {
        if (!scoreAddedForQuestion2) {
            score += 3; // +3 за правильный ответ (только если еще не начисляли)
            scoreAddedForQuestion2 = true;
            dialogText.setString(L"Правильно! +3 балла");
        }
        else {
            dialogText.setString(L"Правильно! (баллы уже начислены)");
        }
    }
    else {
        if (scoreAddedForQuestion2) {
            score -= 3; // Снимаем баллы, если ранее начисляли за этот вопрос
            scoreAddedForQuestion2 = false;
        }
        dialogText.setString(L"Неправильно! Правильный ответ: " +
            sf::String(currentCodeQuestion.correctAnswer));
    }

    answer2Submitted = true;
   
}
void Exam::render(sf::RenderWindow& window) {
    window.clear();
    // Отрисовка фона
    window.draw(background);
    // Отрисовка преподавателя (если не на экране выбора билетов)
    if (currentState != GameState::Entrance && currentState != GameState::TicketSelection) {
        window.draw(teacher);
    }
    // Отрисовка диалогового окна (если не на экране выбора билетов)
    if (currentState != GameState::TicketSelection && currentState != GameState::Question1 && currentState != GameState::Question2) {
        window.draw(dialogBoard);
        window.draw(dialogText);
    }
    // Отрисовка сердечек
    for (auto& heart : hearts) {
        window.draw(heart);
    }
    // Отрисовка заголовка (только на экране входа)
    if (currentState == GameState::Entrance) {
        window.draw(examTitleText);
    }
    // Отрисовка кнопок приветствия (только на экране приветствия)
    if (currentState == GameState::Greeting && !isPaused) {
        window.draw(helloButton);
        window.draw(silenceButton);
    }
    // Отрисовка выбора билетов
    if (currentState == GameState::TicketSelection) {
        window.draw(stol);
        for (const auto& ticket : tickets) {
            window.draw(ticket);
        }
        if (ticketSelected) {
            sf::RectangleShape transition(sf::Vector2f(1920, 1080));
            transition.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(transitionAlpha)));
            window.draw(transition);
            if (transitionComplete && luckyTicket) {
                currentState == GameState::ExamComplete;
                dialogText.setString(L"Счастливый билет! Автоматический зачет");
            }
        }
    }
    // Отрисовка плавного перехода к выбору билетов
    if (transitioningToTickets) {
        sf::RectangleShape transition(sf::Vector2f(1920, 1080));
        transition.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(transitionAlpha)));
        window.draw(transition);
    }
    // Отрисовка вопросов
    if (currentState == GameState::Question1 || currentState == GameState::Question2) {
        window.draw(questionBackground);
       
        if (currentState == GameState::Question1) {
            window.draw(questionText);
            for (int i = 0; i < optionAreas.size(); ++i) {
                // Выделяем выбранный вариант коричневым, остальные - черным
                if (i == selectedOption) {
                    optionTexts[i].setFillColor(sf::Color(139, 69, 19)); // Коричневый
                }
                else {
                    optionTexts[i].setFillColor(sf::Color::Black);
                }
                window.draw(optionTexts[i]);
            }
        }
        else if (currentState == GameState::Question2) {
            window.draw(questionText);
            window.draw(codeImage);
            sf::RectangleShape inputBox(sf::Vector2f(500, 40));
            inputBox.setPosition(200, 500);
            inputBox.setFillColor(sf::Color::White);
            window.draw(inputBox);
            window.draw(answerInputText);
            //  window.draw(submitButton);
        }
        window.draw(exitButton);
        if ((currentState == GameState::Question1) ||
            (currentState == GameState::Question2)) {
        }
    }
    if (currentState == GameState::Question3) {
        window.draw(questionBackground);
       
        window.draw(questionText);
        window.draw(submitButton2);

        switch (question3Type) {
        case Question3Type::ClickBugs:
            window.draw(codeImageb);
            for (int i = 0; i < bugs.size(); ++i) {
                if (!bugsClicked[i]) {
                    window.draw(bugs[i]);
                }
            }
            break;

        case Question3Type::AssembleCode: {
            // Отрисовка области сборки
            sf::RectangleShape assemblyArea(sf::Vector2f(
                codeSolutionTexture.getSize().x,
                codeSolutionTexture.getSize().y));
            assemblyArea.setPosition(600, 300);
            assemblyArea.setFillColor(sf::Color(100, 100, 100, 150));
            assemblyArea.setOutlineColor(sf::Color::White);
            assemblyArea.setOutlineThickness(2);
            window.draw(assemblyArea);

            // Отрисовка частей кода
            for (const auto& piece : codePieces) {
                window.draw(piece.sprite);
            }
            break;
        }

        case Question3Type::SortElements:
            // Сначала рисуем корзины
            for (const auto& basket : baskets) {
                window.draw(basket);
            }

            // Затем рисуем элементы поверх корзин
            for (const auto& piece : codePieces) {
                window.draw(piece.sprite);
            }

            // Отрисовка подсказки

            sf::Text hintText;
            hintText.setFont(font);
            hintText.setCharacterSize(24);
            hintText.setFillColor(sf::Color::White);
            hintText.setString(L"Перетащите элементы в соответствующие корзины");
            hintText.setPosition(200, 200);
            window.draw(hintText);

            break;
        }
    }

    if (currentState == GameState::ExamComplete) {
        window.draw(background);
        window.draw(teacher); // Будет отрисован счастливый или злой спрайт в зависимости от score
        window.draw(dialogBoard);
        window.draw(dialogText); // Будет отображена соответствующая реплика
        // Отрисовка сердечек и баллов
        for (auto& heart : hearts) {
            window.draw(heart);
        }
     
    }
    if (!isPaused) {
        window.draw(pauseButton);
    }
    if (isPaused) {
        // Затемнение
        sf::RectangleShape overlay(sf::Vector2f(1920, 1080));
        overlay.setFillColor(sf::Color(0, 0, 0, 200));
        window.draw(overlay);
        // Кнопки меню паузы
        window.draw(saveAndGoButton);
        window.draw(comebackButton);
        // Текст подсказки
        sf::Text pauseText;
        pauseText.setFont(font);
        pauseText.setCharacterSize(36);
        pauseText.setFillColor(sf::Color::White);
        pauseText.setString(L"Экзамен приостановлен");
        pauseText.setPosition(1920 / 2 - pauseText.getLocalBounds().width / 2, 300);
        window.draw(pauseText);
    }
    // Отрисовка подсказки chek
    if (showTooltip) {
        // Фон подсказки
        sf::RectangleShape tooltipBackground(sf::Vector2f(
            tooltipText.getLocalBounds().width + 20,
            tooltipText.getLocalBounds().height + 20
        ));
        tooltipBackground.setPosition(tooltipPosition - sf::Vector2f(10, 10));
        tooltipBackground.setFillColor(sf::Color(0, 0, 0, 200));
        window.draw(tooltipBackground);
        // Текст подсказки
        tooltipText.setPosition(tooltipPosition);
        window.draw(tooltipText);
    }
    if (showFail) {
        window.draw(failSprite);


    }
    if (showSucces) {
        window.draw(succesSprite);


    }
    window.display();
}
Exam::~Exam() {
    // Освобождаем память от текстур корзин
    for (auto* texture : basketTexturesStorage) {
        if (texture) {
            delete texture;
        }
    }
    basketTexturesStorage.clear();

    // Освобождаем память от текстур элементов
    for (auto* texture : elementTexturesStorage) {
        if (texture) {
            delete texture;
        }
    }
    elementTexturesStorage.clear();
    pieceTextures.clear();

}
