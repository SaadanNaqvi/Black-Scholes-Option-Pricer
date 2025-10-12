#ifndef TEXT_H
#define TEXT_H

#include "InputElement.h"
#include <string>
using namespace std;


class Text : public InputElement {
private:
    string placeholder;
    string content;
    bool active;
    int maxLength;
public:
    Text(Rectangle rect, string placeholder, int maxLength=32);
    
    void draw() override;
    void update() override;

    bool isActive();
    string getContent();
    void setContent(string text);
    void clear();
};

#endif