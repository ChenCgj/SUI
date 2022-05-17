#include <functional>
#include <stdlib.h>

#include "SUI.h"
#include "SUI_button.h"
#include "SUI_main.h"
#include "SUI_window.h"

using namespace std;
using namespace sui;

int main(int argc, char *argv[]) {
    Window *pWindow = new Window("Hello Test", 800, 600, Window_flag::window_flag_resizable);
    pWindow->set_background_color(255, 255, 255, 255);

    Button *pButton = new Button("Button", 150, 150, 200, 200);
    pButton->set_color(255, 0, 0, 125);
    pButton->get_posX_property().bind(pWindow->get_width_property(), function<int (const int &)>([](const int &x)->int {return x / 2;}));
    pButton->get_width_property().bind(pWindow->get_width_property(), function<int (const int &)>([](const int &x)->int {return x / 4;}));
    pButton->add_listener([=](){
        pButton->set_background_color(rand() %255, rand() %255, rand() %255, rand() %255);
        pButton->set_redraw_flag(true);
        present_all();
    });

    Button *pButton2 = new Button("Button", 100, 100, 100, 100);
    pButton2->set_color(0, 0, 0, 125);
    pButton2->add_listener([=](){
        pButton2->set_background_color(rand() %255, rand() %255, rand() %255, rand() %255);
        pButton2->set_redraw_flag(true);
        present_all();
    });

    if (pWindow->add_node(pButton) == false) {
        printf("add child failure!\n");
    }
    pWindow->add_node(pButton2);
    pWindow->add_node(new Button("Hello", 300, 400));
    pWindow->show();

    Window *pWindow2 = new Window("Test2", 800, 600, 100, 100, Window_flag::window_flag_resizable);
    pWindow2->set_background_color(0, 125, 125, 200);
    pWindow2->set_posX(200);
    pWindow2->set_posY(300);
    Button *pButton3 = new Button();
    pButton3->set_posX(0);
    pButton3->set_posY(0);
    pButton3->set_width(100);
    pButton3->set_height(100);
    pButton3->add_listener([=](){
        Button *btn = new Button();
        btn->get_posX_property().bind(pWindow2->get_width_property(),
            std::function<int (const int &)>([](const int &x) {
                if (x - 10 > 0)
                    return rand() % (x - 20);
                else return 0;
            }));
        btn->get_posY_property().bind(pWindow2->get_height_property(),
            std::function<int (const int &)>([](const int &x) {
                if (x - 10 > 0)
                    return rand() % (x - 20);
                else return 0;
            }));
        btn->add_listener([=](){
            btn->set_background_color(rand() %255, rand() %255, rand() %255, rand() %255);
            btn->set_redraw_flag(true);
            present_all();
        });
        btn->set_background_color(rand() % 255, rand() % 255, rand() % 255, rand() % 255);
        pWindow2->add_node(btn);
        present_all();
    });
    pWindow2->add_node(pButton3);
    pWindow2->show();
    return 0;
}
