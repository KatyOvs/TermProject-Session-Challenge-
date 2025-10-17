#include "Material.h"
#include <iostream>
#include <windows.h>
#include <memory>

Material::Material() :
    currentState(State::SUBJECTS),
    scrollOffset(0.f),
    exitRequested(false),
    currentPage(0),
    totalPages(0),
    currentTopic(-1),
    isMouseOverImageView(false)
{
    loadResources();
    setupSubjects();
}

bool numericCompare(const std::string& a, const std::string& b) {
    auto extractNumber = [](const std::string& s) {
        size_t start = s.find_last_of("/\\") + 1;
        size_t end = s.find_last_of("b");
        if (end == std::string::npos || start == std::string::npos) return -1;
        try {
            return std::stoi(s.substr(start, end - start));
        }
        catch (...) {
            return -1;
        }
        };

    int a_num = extractNumber(a);
    int b_num = extractNumber(b);

    if (a_num != -1 && b_num != -1) return a_num < b_num;
    return a < b;
}

std::vector<std::string> GetFilesInDirectory(const std::string& directory, const std::string& extension = "") {
    std::vector<std::string> files;
    std::string searchPath = directory + "/*" + extension;

    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                files.emplace_back(directory + "/" + findData.cFileName);
            }
        } while (FindNextFileA(hFind, &findData) != 0);
        FindClose(hFind);
    }

    std::sort(files.begin(), files.end(), numericCompare);
    return files;
}

void Material::loadResources() {
    auto loadTexture = [](sf::Texture& texture, const std::string& path) {
        if (!texture.loadFromFile(path)) {
            std::cerr << "Ошибка загрузки текстуры: " << path << std::endl;
            return false;
        }
        return true;
        };

    if (!loadTexture(backgroundTexture, "img/Abbk.png") ||
        !loadTexture(exitTexture, "img/exitbutton.png") ||
        !loadTexture(oaipTexture, "img/oaipb.png") ||
        !loadTexture(ipobTexture, "img/ipob.png") ||
        !loadTexture(matanTexture, "img/matemb.png")) {
        return;
    }

    background.setTexture(backgroundTexture);

    exitBtn.setTexture(exitTexture);
    exitBtn.setPosition(20.f, 900.f);

    oaipBtn.setTexture(oaipTexture);
    oaipBtn.setPosition(20.f, 50.f);

    ipobBtn.setTexture(ipobTexture);
    ipobBtn.setPosition(20.f, 300.f);

    matanBtn.setTexture(matanTexture);
    matanBtn.setPosition(20.f, 550.f);

    imageViewArea.setSize(sf::Vector2f(754.f, 1080.f));
    imageViewArea.setPosition((1920.f - 754.f) / 2, 0);
    imageViewArea.setFillColor(sf::Color(240, 240, 240, 200));
    imageViewArea.setOutlineThickness(2.f);
    imageViewArea.setOutlineColor(sf::Color::White);
}

void Material::setupSubjects() {
    currentState = State::SUBJECTS;
    scrollOffset = 0.f;
    topicButtons.clear();
    currentTopic = -1;
}

void Material::sortTopicButtons() {
    std::sort(topicButtons.begin(), topicButtons.end(),
        [](const auto& a, const auto& b) {
            return a.getPosition().y < b.getPosition().y;
        });
}

void Material::setupTopics(const std::string& subject) {
    currentState = State::TOPICS;
    currentSubject = subject;
    scrollOffset = 0.f;
    topicButtons.clear();
    topicTextures.clear();

    static const std::unordered_map<std::string, std::string> subjectFolders = {
        {"ОАиП", "img/ab/"},
        {"ИПОБ", "img/bb/"},
        {"Мат. анализ", "img/cb/"}
    };

    auto it = subjectFolders.find(subject);
    if (it == subjectFolders.end()) return;

    try {
        auto files = GetFilesInDirectory(it->second, ".png");
        topicTextures.reserve(files.size());
        topicButtons.reserve(files.size());

        for (size_t i = 0; i < files.size(); ++i) {
            sf::Texture texture;
            if (texture.loadFromFile(files[i])) {
                topicTextures.emplace_back(std::move(texture));

                sf::Sprite button;
                button.setTexture(topicTextures.back());
                button.setPosition(20.f, 50.f + i * 220.f);
                topicButtons.emplace_back(std::move(button));
            }
        }
        sortTopicButtons();
    }
    catch (...) {
        std::cerr << "Ошибка загрузки кнопок тем" << std::endl;
    }
}

void Material::showTopicMaterials(int topicNumber) {
    currentState = State::IMAGE_VIEW;
    currentPage = 0;
    contentSprites.clear();
    loadedTextures.clear();
    currentTopic = topicNumber;

    static const std::unordered_map<std::string, std::string> subjectFolders = {
        {"ОАиП", "ОАиП"},
        {"ИПОБ", "ИПОБ"},
        {"Мат. анализ", "МАТ"}
    };

    auto it = subjectFolders.find(currentSubject);
    if (it == subjectFolders.end()) return;

    std::string materialsPath = "materials/" + it->second + "/" + std::to_string(topicNumber) + "b/";

    try {
        auto imageFiles = GetFilesInDirectory(materialsPath, ".jpg");
        loadedTextures.reserve(imageFiles.size());
        contentSprites.reserve(imageFiles.size());

        for (const auto& filename : imageFiles) {
            auto texture = std::make_unique<sf::Texture>();
            if (texture->loadFromFile(filename)) {
                sf::Sprite sprite(*texture);

                float scaleX = 754.f / texture->getSize().x;
                float scaleY = 1080.f / texture->getSize().y;
                float scale = std::min(scaleX, scaleY);

                sprite.setScale(scale, scale);

                float offsetX = (754.f - texture->getSize().x * scale) / 2;
                float offsetY = (1080.f - texture->getSize().y * scale) / 2;

                sprite.setPosition((1920.f - 754.f) / 2 + offsetX, offsetY);

                contentSprites.emplace_back(std::move(sprite));
                loadedTextures.emplace_back(std::move(texture));
            }
        }
    }
    catch (...) {
        std::cerr << "Ошибка загрузки материалов" << std::endl;
    }

    totalPages = contentSprites.size();
}

void Material::hideMaterials() {
    loadedTextures.clear();
    contentSprites.clear();
    currentTopic = -1;
}

void Material::handleEvents(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // Получаем позицию мыши относительно основного view (без прокрутки)
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        isMouseOverImageView = imageViewArea.getGlobalBounds().contains(mousePos);

        if (event.type == sf::Event::MouseWheelScrolled) {
            if (currentState != State::SUBJECTS && !isMouseOverImageView) {
                float maxOffset = std::max(0.f, topicButtons.size() * 220.f - 800.f);
                scrollOffset = std::max(0.f, std::min(scrollOffset + event.mouseWheelScroll.delta * 30.f, maxOffset));
            }
            else if (currentState == State::IMAGE_VIEW && isMouseOverImageView && !contentSprites.empty()) {
                if (event.mouseWheelScroll.delta > 0) {
                    currentPage = std::max(0, currentPage - 1);
                }
                else {
                    currentPage = std::min(totalPages - 1, currentPage + 1);
                }
            }
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            if (exitBtn.getGlobalBounds().contains(mousePos)) {
                if (currentState == State::SUBJECTS) {
                    exitRequested = true;
                }
                else {
                    setupSubjects();
                }
            }
            else if (currentState == State::SUBJECTS) {
                if (oaipBtn.getGlobalBounds().contains(mousePos)) setupTopics("ОАиП");
                else if (ipobBtn.getGlobalBounds().contains(mousePos)) setupTopics("ИПОБ");
                else if (matanBtn.getGlobalBounds().contains(mousePos)) setupTopics("Мат. анализ");
            }
            else if ((currentState == State::TOPICS || currentState == State::IMAGE_VIEW) && !topicButtons.empty()) {
                // Корректируем позицию мыши с учетом прокрутки
                sf::Vector2f correctedMousePos(mousePos.x, mousePos.y + scrollOffset);

                for (size_t i = 0; i < topicButtons.size(); ++i) {
                    if (topicButtons[i].getGlobalBounds().contains(correctedMousePos)) {
                        if (currentTopic != static_cast<int>(i + 1)) {
                            showTopicMaterials(i + 1);
                        }
                        break;
                    }
                }
            }
            else if (currentState == State::IMAGE_VIEW && !contentSprites.empty()) {
                if (mousePos.x > 960) {
                    currentPage = std::min(totalPages - 1, currentPage + 1);
                }
                else if (mousePos.x < 960 - 754.f / 2) {
                    currentPage = std::max(0, currentPage - 1);
                }
            }
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                if (currentState == State::IMAGE_VIEW) {
                    hideMaterials();
                    currentState = State::TOPICS;
                }
                else {
                    setupSubjects();
                }
            }
            else if (currentState == State::IMAGE_VIEW && !contentSprites.empty()) {
                if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Up) {
                    currentPage = std::max(0, currentPage - 1);
                }
                else if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::Down) {
                    currentPage = std::min(totalPages - 1, currentPage + 1);
                }
            }
        }
    }
}

void Material::render(sf::RenderWindow& window) {
    window.clear();
    window.draw(background);

    if (currentState != State::SUBJECTS) {
        sf::View scrollView = window.getView();
        scrollView.setCenter(960, 540 + scrollOffset);
        window.setView(scrollView);

        for (auto& btn : topicButtons) {
      
            if (currentTopic != -1 && &btn - &topicButtons[0] + 1 == currentTopic) {
                btn.setColor(sf::Color(255, 255, 255, 200));
            }
            else {
                btn.setColor(sf::Color::White);
            }
            window.draw(btn);
        }

        scrollView.setCenter(960, 540);
        window.setView(scrollView);

        if (topicButtons.size() * 220.f > 800.f) {
            float scrollRatio = 800.f / (topicButtons.size() * 220.f);
            float scrollBarHeight = 800.f * scrollRatio;
            float scrollBarPos = (scrollOffset / (topicButtons.size() * 220.f - 800.f)) * (800.f - scrollBarHeight);

            sf::RectangleShape scrollBar(sf::Vector2f(10, scrollBarHeight));
            scrollBar.setPosition(1900, 50 + scrollBarPos);
            scrollBar.setFillColor(sf::Color(150, 150, 150, 200));
            window.draw(scrollBar);
        }
    }

    switch (currentState) {
    case State::SUBJECTS:
        window.draw(oaipBtn);
        window.draw(ipobBtn);
        window.draw(matanBtn);
        break;

    case State::IMAGE_VIEW:
        window.draw(imageViewArea);
        if (!contentSprites.empty() && currentPage < contentSprites.size()) {
            window.draw(contentSprites[currentPage]);
        }
        break;
    }

    window.draw(exitBtn);
    window.display();
}

bool Material::show(sf::RenderWindow& window) {
    exitRequested = false;
    while (window.isOpen() && !exitRequested) {
        handleEvents(window);
        render(window);
    }
    return exitRequested;
}