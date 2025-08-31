#include <SFML/Graphics.hpp>
#include <optional>
#include <string>

int main() {
    sf::RenderWindow window(sf::VideoMode({900, 600}), "Mini Dashboard");
    window.setFramerateLimit(60);

    // --- Load font (place a TTF in assets/, e.g., NotoSans-Regular.ttf)
    sf::Font font("/System/Library/Fonts/Supplemental/Arial.ttf");

    // --- UI: title
    sf::Text title(font, "Dashboard", 36);
    title.setPosition({32.f, 24.f});

    // --- UI: input label
    sf::Text label(font, "Enter text:", 20);
    label.setFillColor(sf::Color(160,160,160));
    label.setPosition({32.f, 120.f});

    // --- UI: input box
    sf::RectangleShape inputBox({520.f, 44.f});
    inputBox.setPosition({32.f, 150.f});
    inputBox.setFillColor(sf::Color(245,245,245));
    inputBox.setOutlineThickness(2.f);
    inputBox.setOutlineColor(sf::Color(200,200,200));

    sf::Text inputText(font, "", 22);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition({42.f, 158.f});

    bool inputFocused = false;
    std::u32string inputBuffer; // handle unicode cleanly

    // --- UI: submit button
    sf::RectangleShape btn({120.f, 44.f});
    btn.setPosition({570.f, 150.f});
    btn.setFillColor(sf::Color(30,144,255)); // dodger blue
    sf::Text btnText(font, "Submit", 22);
    btnText.setPosition({590.f, 158.f});

    // --- UI: output
    sf::Text outputTitle(font, "Output:", 20);
    outputTitle.setFillColor(sf::Color(160,160,160));
    outputTitle.setPosition({32.f, 220.f});

    sf::RectangleShape outputBox({658.f, 160.f});
    outputBox.setPosition({32.f, 250.f});
    outputBox.setFillColor(sf::Color(250,250,250));
    outputBox.setOutlineThickness(2.f);
    outputBox.setOutlineColor(sf::Color(230,230,230));

    sf::Text outputText(font, "(nothing yet)", 22);
    outputText.setFillColor(sf::Color(60,60,60));
    outputText.setPosition({42.f, 260.f});

    auto submit = [&](){
        if (inputBuffer.empty()) {
            outputText.setString("(empty input)");
        } else {
            // Convert UTF-32 buffer to UTF-8 for SFML text
            std::string utf8;
            for (char32_t ch : inputBuffer) {
                if (ch <= 0x7F) utf8.push_back(static_cast<char>(ch));
                else if (ch <= 0x7FF) {
                    utf8.push_back(static_cast<char>(0xC0 | ((ch >> 6) & 0x1F)));
                    utf8.push_back(static_cast<char>(0x80 | (ch & 0x3F)));
                } else if (ch <= 0xFFFF) {
                    utf8.push_back(static_cast<char>(0xE0 | ((ch >> 12) & 0x0F)));
                    utf8.push_back(static_cast<char>(0x80 | ((ch >> 6) & 0x3F)));
                    utf8.push_back(static_cast<char>(0x80 | (ch & 0x3F)));
                } else {
                    utf8.push_back(static_cast<char>(0xF0 | ((ch >> 18) & 0x07)));
                    utf8.push_back(static_cast<char>(0x80 | ((ch >> 12) & 0x3F)));
                    utf8.push_back(static_cast<char>(0x80 | ((ch >> 6) & 0x3F)));
                    utf8.push_back(static_cast<char>(0x80 | (ch & 0x3F)));
                }
            }
            outputText.setString("You entered: " + utf8);
        }
    };

    while (window.isOpen()) {
        while (const std::optional ev = window.pollEvent()) {
            if (ev->is<sf::Event::Closed>()) window.close();

            if (const auto* me = ev->getIf<sf::Event::MouseButtonPressed>()) {
                const sf::Vector2f mpos{static_cast<float>(me->position.x), static_cast<float>(me->position.y)};
                inputFocused = inputBox.getGlobalBounds().contains(mpos);
                // Click submit
                if (btn.getGlobalBounds().contains(mpos)) submit();
            }

            if (const auto* te = ev->getIf<sf::Event::TextEntered>()) {
                if (!inputFocused) continue;
                char32_t code = te->unicode;
                // Handle backspace (SFML 3: Backspace emits codepoint 8)
                if (code == 8) {
                    if (!inputBuffer.empty()) inputBuffer.pop_back();
                } else if (code == 13) { // Enter
                    submit();
                } else if (code >= 32) { // printable
                    inputBuffer.push_back(code);
                }

                // Mirror buffer to UI text (truncate if too long for box)
                // naive width guard: limit chars ~ fits box; refine if needed
                const size_t maxChars = 64;
                std::u32string view = inputBuffer;
                if (view.size() > maxChars) view = view.substr(view.size() - maxChars);
                // convert to UTF-8 for display
                std::string utf8;
                for (char32_t ch : view) {
                    if (ch <= 0x7F) utf8.push_back(static_cast<char>(ch));
                    else if (ch <= 0x7FF) {
                        utf8.push_back(static_cast<char>(0xC0 | ((ch >> 6) & 0x1F)));
                        utf8.push_back(static_cast<char>(0x80 | (ch & 0x3F)));
                    } else if (ch <= 0xFFFF) {
                        utf8.push_back(static_cast<char>(0xE0 | ((ch >> 12) & 0x0F)));
                        utf8.push_back(static_cast<char>(0x80 | ((ch >> 6) & 0x3F)));
                        utf8.push_back(static_cast<char>(0x80 | (ch & 0x3F)));
                    } else {
                        utf8.push_back(static_cast<char>(0xF0 | ((ch >> 18) & 0x07)));
                        utf8.push_back(static_cast<char>(0x80 | ((ch >> 12) & 0x3F)));
                        utf8.push_back(static_cast<char>(0x80 | ((ch >> 6) & 0x3F)));
                        utf8.push_back(static_cast<char>(0x80 | (ch & 0x3F)));
                    }
                }
                inputText.setString(utf8);
            }
        }

        // Hover effect for button
        sf::Vector2i mp = sf::Mouse::getPosition(window);
        if (btn.getGlobalBounds().contains(sf::Vector2f{(float)mp.x, (float)mp.y})) {
            btn.setFillColor(sf::Color(65, 170, 255));
        } else {
            btn.setFillColor(sf::Color(30, 144, 255));
        }

        // Focus highlight
        inputBox.setOutlineColor(inputFocused ? sf::Color(30,144,255) : sf::Color(200,200,200));

        window.clear(sf::Color(250,250,252));
        window.draw(title);
        window.draw(label);
        window.draw(inputBox);
        window.draw(inputText);
        window.draw(btn);
        window.draw(btnText);
        window.draw(outputTitle);
        window.draw(outputBox);
        window.draw(outputText);
        window.display();
    }
}
