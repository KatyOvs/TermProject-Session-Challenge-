#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <SFML/Audio.hpp>
#include <random>
#include <map>
#include <algorithm>

class Exam {
public:
    Exam(int teacherId);
    void start(sf::RenderWindow& window);
    ~Exam();
private:
    // Структуры данных
    struct Question {
        std::string text;
        std::vector<std::string> options;
        int correctOption;
    };

    struct CodeQuestion {
        std::string imagePath;
        std::string correctAnswer;
    };

    struct DraggablePiece {
        sf::Sprite sprite;
        sf::Vector2f originalPosition;
        sf::Vector2f correctPosition;
        sf::Vector2f offset; // Добавлено
        bool isDragging = false;
        bool isCorrect = false;
        int width;    // Добавляем поле для ширины
        int height;   // Добавляем поле для высоты

    };
    // Состояния игры
    enum class GameState {
        Entrance,
        Greeting,
        WaitingForClickAfterGreeting,
        TicketSelection,
        Question1,
        Question2,
        Question3,
        ExamComplete,
        ReturnToMenu,
        ReturnToMenu2,
        ExamFailed
    };

    enum class Question3Type {
        ClickBugs,      // Легкий билет - клик по багам
        AssembleCode,   // Средний билет - собрать код
        SortElements    // Сложный билет - сортировка по корзинам
    };

    // Методы
    void loadResources();
    void handleEvents(sf::RenderWindow& window);
    void update(float deltaTime);
    void render(sf::RenderWindow& window);

    void setupQuestion1();
    void setupQuestion2();
    void setupQuestion3();
    void setupClickBugsGame();
    void setupAssembleCodeGame();
    void setupSortElementsGame();

    void checkAnswer1(int selectedOption);
    void checkAnswer2(const std::string& answer);
    void checkQuestion3Completion();
    void loadQuestionsForSubject();
    std::vector<sf::Texture> pieceTextures; // Для хранения текстур частей кода
    // Переменные-члены
    // Основные параметры
    int teacherId;
    GameState currentState;
    Question3Type question3Type;
    bool shouldExit;
    bool showDarkOverlay;
    bool isPaused;
    
    int selectedOption = -1;
    // Состояния билетов и вопросов
    bool ticketSelected;
    bool luckyTicket;
    int selectedTicketIndex;
    bool answer1Submitted;
    bool answer2Submitted;
    bool question3Completed;
    bool scoreUpdatedForQuestion3;

    // Переходы и анимации
    bool transitioning;
    bool transitionComplete;
    bool transitioningToTickets;
    float fadeAlpha;
    float transitionAlpha;
    bool fadeInComplete;
    bool greetingBonusAdded = false; // Чтобы балл за приветствие начислялся только один раз
    // Взаимодействие с преподавателем
    bool greetedTeacher;
    bool examSubmitted;
    bool examResultShown;
    int score;
    int hoveredOption;

    // Вопросы и ответы
    std::map<int, Question> testQuestions;
    std::map<int, CodeQuestion> codeQuestions;
    Question currentTestQuestion;
    CodeQuestion currentCodeQuestion;
    std::string currentAnswer;
    std::wstring currentAnswerW;
    std::vector<sf::RectangleShape> optionAreas;
    bool heartAlreadyLost = false; // Флаг, что сердечко уже отнято
    // Аудио
    sf::Music examMusic;
    
    // Текстуры
    sf::Texture backgroundTexture;
    sf::Texture teacherTexture;
    sf::Texture teacherHappyTexture;
    sf::Texture teacherAngryTexture;
    sf::Texture teacherCheckTexture;
    sf::Texture dialogTexture;
    sf::Texture heartTexture;
    sf::Texture helloButtonTexture;
    sf::Texture silenceButtonTexture;
    sf::Texture pauseButtonTexture;
    sf::Texture saveAndGoTexture;
    sf::Texture comebackTexture;
    sf::Texture stolTexture;
    sf::Texture biletTexture;
    sf::Texture biletHoverTexture;
    sf::Texture luckyBiletTexture;
    sf::Texture questionBackgroundTexture;
    sf::Texture exitButtonTexture;
    sf::Texture codeTexture;
    sf::Texture codePiecesTexture;
    sf::Texture codeSolutionTexture;
    sf::Texture submitButtonTexture2;
    sf::Texture bugTexture;
    sf::Texture basket1Texture;
    sf::Texture basket2Texture;
    sf::Texture basket3Texture;
    sf::Texture elementsTexture;

    // Спрайт
    sf::Texture successTexture;
    sf::Sprite succesSprite;
    bool showSucces = false;
    float succesAlpha = 0.0f;

    sf::Texture failTexture;
    sf::Sprite failSprite;
    bool showFail = false;
    float failAlpha = 0.0f;
    sf::Sprite heart;
    sf::Sprite background;
    sf::Sprite teacher;
    sf::Sprite teacherCheck;
    sf::Sprite dialogBoard;
    sf::Sprite helloButton;
    sf::Sprite silenceButton;
    sf::Sprite pauseButton;
    sf::Sprite saveAndGoButton;
    sf::Sprite comebackButton;
    sf::Sprite stol;
    sf::Sprite questionBackground;
    sf::Sprite exitButton;
    sf::Sprite codeImage;
    sf::Sprite completedCodeImage;
    sf::Sprite submitButton2;
    std::vector<sf::Sprite> hearts;
    std::vector<sf::Sprite> tickets;
    std::vector<sf::Sprite> optionButtons;
    sf::Texture codeNoBugsTexture;
    // Элементы мини-игр
    std::vector<DraggablePiece> codePieces;
    std::vector<sf::Sprite> bugs;
    std::vector<bool> bugsClicked;
    std::vector<sf::Sprite> baskets;
    std::vector<std::vector<int>> basketItems;
    std::vector<int> bugClickCounts;
    std::vector<int> currentClicks;
    std::vector<sf::Texture*> basketTexturesStorage; // Для хранения текстур корзин
    std::vector<sf::Texture*> elementTexturesStorage; // Для хранения текстур элементов
    sf::Clock defeatTimer;
    bool showDefeatScreen = false;
    sf::Texture codeWithBugsTexture;  // Новая текстура для кода с багами
    sf::Sprite codeImageb;            // Новый спрайт для кода с багами
    // Текст и шрифты
    sf::Font font;
    sf::Text dialogText;
    sf::Text examTitleText;
    sf::Text luckyText;
    sf::Text questionText;
   // sf::Text scoreText;
    sf::Text optionTexts[3];
    sf::Text answerInputText;
    sf::Text tooltipText;
    bool showTooltip;
    sf::Vector2f tooltipPosition;
    bool scoreAddedForQuestion1 = false;
    bool scoreAddedForQuestion2 = false;
    bool scoreAddedForQuestion3 = false;
    sf::Texture oaipBasket1Texture, oaipBasket2Texture, oaipBasket3Texture;
    sf::Texture oaipElementTextures[9];
    sf::Sprite mathErrorSprite;  // Единственный спрайт ошибки
    sf::Texture mathErrorTexture;
    int errorClickCount = 0;     // Счетчик кликов по ошибке
    const int clicksRequired = 3; // Требуемое количество кликов
    sf::Vector2f errorPosition;  // Текущая позиция ошибки
    bool errorFixed = false;     // Флаг исправления ошибки
};

