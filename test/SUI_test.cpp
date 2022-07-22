#include <functional>

#include "SUI.h"

using namespace std;
using namespace sui;

int main(int argc, char *argv[]) {
    Window *pWindow = new Window("Hello Test", 800, 600, Window_flag::window_flag_resizable);
    pWindow->set_background_color(255, 125, 125, 255);
    Rect target_rect{0, 0, 600, 600};
    Rect src_rect{0, 0, 500, 500};
    pWindow->set_background_image("background.jpg", target_rect, src_rect);
    pWindow->set_background_fill_style(Background_fill_style::full, dynamic_cast<Geometry *>(pWindow));
    // target_rect.p2.x = 800;
    // pWindow->set_background_fill_style(Background_fill_style::target_size, &target_rect);

    Button *pButton = new Button("Button", 150, 150, 200, 200);
    pButton->set_color(255, 0, 0, 125);
    pButton->set_background_image("background.jpg", target_rect, Rect{0, 0, 2000, 2000}, Element_status::button_normal);
    pButton->set_background_fill_style(Background_fill_style::full, dynamic_cast<Geometry *>(pButton), Element_status::button_normal);
    auto func_div2 = function<int (const int &)>([](const int &x)->int {return x / 2;});
    auto func_div4 = function<int (const int &)>([](const int &x)->int {return x / 4;});
    pButton->get_posX_property().bind(pWindow->get_width_property(), func_div2);
    pButton->get_width_property().bind(pWindow->get_width_property(), func_div4);
    pButton->get_posY_property().bind(pWindow->get_height_property(), func_div4);
    pButton->get_height_property().bind(pWindow->get_height_property(), func_div4);
    pButton->add_listener([=](){
        pButton->set_background_color(rand() %255, rand() %255, rand() %255, rand() %255, Element_status::button_normal);
    });

    Button *pButton_control = new Button("Control", 0, 0, 100, 100);
    pButton_control->set_color(255, 0, 0, 125);
    pButton_control->add_listener([=](){
        static bool control = true;
        if (control) {
            pButton->get_posX_property().unbind(pWindow->get_width_property());
            pButton->get_width_property().unbind(pWindow->get_width_property());
            pButton->get_posY_property().unbind(pWindow->get_height_property());
            pButton->get_height_property().unbind(pWindow->get_height_property());
            control = false;
        } else {
            pButton->get_posX_property().bind(pWindow->get_width_property(), func_div2);
            pButton->get_width_property().bind(pWindow->get_width_property(), func_div4);
            pButton->get_posY_property().bind(pWindow->get_height_property(), func_div4);
            pButton->get_height_property().bind(pWindow->get_height_property(), func_div4);
            control = true;
            pButton->set_redraw_flag(true);
            present_all();
        }
    });
    Graphic_board *pboard = new Graphic_board(300, 300, 300, 300);
    pboard->set_draw_callback(function<void (Graphic_board_base *)>([](Graphic_board_base *arg) {
        static Image img(150, 150);
        arg->set_color(Color{0, 125, 125, 255});
        arg->clear();
        img.load_img("background.jpg");;
        arg->draw_image(img, 75, 75);
        img.unload_data();
        arg->set_color(Color{255, 125, 0, 255});
        arg->move_to(50, 50);
        arg->line_to(50, 25);
        arg->line_to(75, 25);
        arg->line_to(75, 75);
        arg->line_to(25, 75);
        arg->line_to(25, 25);
        arg->draw_line(0, 0, 100, 100);
        arg->draw_text(Rect{125, 225, 300, 300}, "Hello World! Nice to meet you.", "Inkfree.ttf", Color{0, 0, 0, 255}, 20);
    }));

    Button *pButton2 = new Button("Button", 100, 100, 100, 100);
    pButton2->set_color(0, 0, 0, 125);
    pButton2->add_listener([=](){
        pButton2->set_background_color(rand() %255, rand() %255, rand() %255, rand() %255, Element_status::button_normal);
    });

    if (pWindow->add_node(pButton) == false) {
        printf("add child failure!\n");
    }
    pWindow->add_node(pButton_control);
    pWindow->add_node(pButton2);
    pWindow->add_node(new Button("Hello World", 300, 400));
    pWindow->add_node(pboard);
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
            btn->set_background_color(rand() %255, rand() %255, rand() %255, rand() %255, Element_status::button_normal);
        });
        btn->set_background_color(rand() % 255, rand() % 255, rand() % 255, rand() % 255, Element_status::button_normal);
        pWindow2->add_node(btn);
        present_all();
    });
    pWindow2->add_node(pButton3);
    pWindow2->show();
    return 0;
}

