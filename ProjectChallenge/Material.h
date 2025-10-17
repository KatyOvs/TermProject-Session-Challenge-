#ifndef MATERIAL_H
#define MATERIAL_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include <unordered_map>

class Material {
public:
    Material();
    bool show(sf::RenderWindow& window);

private:
    enum class State {
        SUBJECTS,
        TOPICS,
        IMAGE_VIEW
    };

    void loadResources();
    void handleEvents(sf::RenderWindow& window);
    void render(sf::RenderWindow& window);
    void setupSubjects();
    void setupTopics(const std::string& subject);
    void showTopicMaterials(int topicNumber);
    void hideMaterials();
    void sortTopicButtons();

    State currentState;
    std::string currentSubject;
    float scrollOffset;
    bool exitRequested;
    int currentPage;
    int totalPages;
    int currentTopic;
    bool isMouseOverImageView;
    bool isMouseOverTopicsMenu; // Новый флаг
  
    // Графические ресурсы
    sf::Texture backgroundTexture;
    sf::Texture exitTexture;
    std::vector<sf::Texture> topicTextures;
    std::vector<std::unique_ptr<sf::Texture>> loadedTextures;
    std::vector<sf::Sprite> contentSprites;

    sf::Sprite background;
    sf::Sprite exitBtn;
    std::vector<sf::Sprite> topicButtons;
    sf::RectangleShape imageViewArea;
    sf::RectangleShape topicsArea; // Новая область для меню тем

    sf::Texture oaipTexture;
    sf::Texture ipobTexture;
    sf::Texture matanTexture;
    sf::Sprite oaipBtn;
    sf::Sprite ipobBtn;
    sf::Sprite matanBtn;
};

#endif // MATERIAL_H