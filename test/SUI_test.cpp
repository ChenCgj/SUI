#include <iostream>
#include <functional>

#include "SUI.h"

using namespace std;
using namespace sui;

int main(int argc, char *argv[]) {
    set_run_mode(Run_mode::wait);
    Window *pWindow = new Window("Hello Test", 1280, 960, Window_flag::window_flag_resizable);
    pWindow->set_background_color(255, 125, 125, 255);
    Rect target_rect{0, 0, 600, 600};
    Rect src_rect{0, 0, 500, 500};
    pWindow->set_background_image("background.jpg", target_rect, src_rect);
    pWindow->set_background_fill_style(Background_fill_style::full, dynamic_cast<Geometry *>(pWindow));
    // target_rect.p2.x = 800;
    // pWindow->set_background_fill_style(Background_fill_style::target_size, &target_rect);

    Pane *ppane = new Pane(10, 10, 0, 0);
    ppane->get_width_property().bind(pWindow->get_width_property(), function<int (const int &)>([](const int &x)->int {return x * 7 / 8;}));
    ppane->get_height_property().bind(pWindow->get_height_property(), function<int (const int &)>([](const int &x)->int {return x * 7 / 8;}));

    Button *pButton = new Button("Button", 150, 150, 200, 200);
    pButton->set_always_redraw(true);
    pButton->set_color(255, 0, 0, 125);
    pButton->set_background_image("background.jpg", target_rect, Rect{0, 0, 2000, 2000}, Element_status::button_normal);
    pButton->set_background_fill_style(Background_fill_style::full, dynamic_cast<Geometry *>(pButton), Element_status::button_normal);
    auto func_div2 = function<int (const int &)>([](const int &x)->int {return x / 2;});
    auto func_div4 = function<int (const int &)>([](const int &x)->int {return x / 4;});
    pButton->get_posX_property().bind(ppane->get_width_property(), func_div2);
    pButton->get_width_property().bind(ppane->get_width_property(), func_div4);
    pButton->get_posY_property().bind(ppane->get_height_property(), func_div4);
    pButton->get_height_property().bind(ppane->get_height_property(), func_div4);
    pButton->add_listener([=](const Mouse_button_event &, void *){
        pButton->set_background_color(rand() %255, rand() %255, rand() %255, rand() %255, Element_status::button_normal);
    }, sui::Button::Button_event::be_up, nullptr);

    Button *pButton_control = new Button("Control", 0, 0, 100, 100);
    pButton_control->set_color(255, 0, 0, 125);
    pButton_control->add_listener([=](const Mouse_button_event &, void *){
        static bool control = true;
        if (control) {
            pButton->get_posX_property().unbind(ppane->get_width_property());
            pButton->get_width_property().unbind(ppane->get_width_property());
            pButton->get_posY_property().unbind(ppane->get_height_property());
            pButton->get_height_property().unbind(ppane->get_height_property());
            control = false;
        } else {
            pButton->get_posX_property().bind(ppane->get_width_property(), func_div2);
            pButton->get_width_property().bind(ppane->get_width_property(), func_div4);
            pButton->get_posY_property().bind(ppane->get_height_property(), func_div4);
            pButton->get_height_property().bind(ppane->get_height_property(), func_div4);
            control = true;
            pButton->set_redraw_flag(true);
            present_all();
        }
    }, sui::Button::Button_event::be_up, nullptr);
    Graphic_board *pboard = new Graphic_board(300, 300, 300, 300);
    // pboard->set_draw_callback(function<void (Graphic_board_base *)>([](Graphic_board_base *arg) {
    //     static Image img(150, 150);
    //     arg->set_draw_color(Color{0, 125, 125, 255}, true);
    //     arg->clear(true);
    //     img.load_img("background.jpg");
    //     arg->draw_image(img, 75, 75, true);
    //     img.unload_data();  // why should I add this?
    //     arg->set_draw_color(Color{255, 125, 0, 255}, true);
    //     arg->move_to(50, 50, true);
    //     arg->line_to(50, 25, true);
    //     arg->line_to(75, 25, true);
    //     arg->line_to(75, 75, true);
    //     arg->line_to(25, 75, true);
    //     arg->line_to(25, 25, true);
    //     arg->draw_line(0, 0, 100, 100, true);
    //     arg->draw_text(Rect{125, 225, 300, 300}, "Hello World! Nice to meet you.", "Inkfree.ttf", Color{0, 0, 0, 255}, 20, true);
    // }));
    // bug
    static Image img(150, 150);
    pboard->set_draw_color(Color{0, 125, 125, 255});
    pboard->clear();
    img.load_img("background.jpg");;
    pboard->draw_image(img, 75, 75);
    // img.unload_data();
    pboard->set_draw_color(Color{255, 125, 0, 255});
    pboard->move_to(50, 50);
    pboard->line_to(50, 25);
    pboard->line_to(75, 25);
    pboard->line_to(75, 75);
    pboard->line_to(25, 75);
    pboard->line_to(25, 25);
    pboard->draw_line(0, 0, 100, 100);
    pboard->draw_text(Rect{125, 225, 300, 300}, "Hello World! Nice to meet you.", "Inkfree.ttf", Color{0, 0, 0, 255}, 20);
    pboard->add_listener([&](const Mouse_button_event &, void *) {
        auto pos = get_mouse_pos();
        cout << "x: " << pos.first << " y: " << pos.second << endl;
    }, Graphic_board::Graphic_board_event::gbe_move, nullptr);
    pboard->add_listener([&](const Mouse_button_event &, void *) {
        cout << "button down in graphic board" << endl;
    }, Graphic_board::Graphic_board_event::gbe_down, nullptr);
    pboard->add_listener([&](const Mouse_button_event &, void *) {
        cout << "button up in graphic board" << endl;
    }, Graphic_board::Graphic_board_event::gbe_up, nullptr);

    Button *pButton2 = new Button("Button", 100, 100, 100, 100);
    pButton2->set_color(0, 0, 0, 125);
    pButton2->add_listener([=](const Mouse_button_event &, void *) {
        pButton2->set_background_color(rand() %255, rand() %255, rand() %255, rand() %255, Element_status::button_normal);
    }, sui::Button::Button_event::be_up, nullptr);

    Grid_pane *gpane = new Grid_pane(800, 600, 300, 300, 3, 3);
    gpane->set_gap(10, 10);
    Button *buttons[9] = {nullptr};
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            buttons[3 * i + j] = new Button(to_string(i) + ", " + to_string(j));
            buttons[3 * i + j]->set_background_color(rand() % 255, rand() % 255, rand() % 255, rand() % 255, Element_status::button_normal);
            gpane->add_node(buttons[3 * i + j]);
            gpane->add_content(*buttons[3 * i + j], i, j);
        }
    }
    gpane->get_posX_property().bind(ppane->get_width_property(), std::function<int (const int &)>([](const int &value){return value * 3 / 4;}));
    gpane->get_width_property().bind(ppane->get_width_property(), std::function<int (const int &)>([](const int &value){return value / 4;}));
    gpane->get_posY_property().bind(ppane->get_height_property(), std::function<int (const int &)>([](const int &value){return value * 3 / 4;}));
    gpane->get_height_property().bind(ppane->get_height_property(), std::function<int (const int &)>([](const int &value){return value / 4;}));

    Horizontal_pane *hpane = new Horizontal_pane(10, 600, 300, 300);
    hpane->set_gap(5);
    hpane->set_height(80);
    // hpane->get_width_property().bind(pWindow->get_width_property(), std::function<int (const int &)>([](const int &value) {return value * 3 / 4;}));
    // hpane->get_height_property().bind(pWindow->get_height_property(), std::function<int (const int &)>([](const int &value){return value / 8;}));
    // hpane->get_posY_property().bind(pWindow->get_height_property(), std::function<int (const int &)>([](const int &value){return value / 4;}));
    Button *hbuttons[4] = {nullptr};
    for (int i = 0; i < 4; ++i) {
        hbuttons[i] = new Button(to_string(i));
        hbuttons[i]->set_width(30 * (i + 1));
        hpane->add_node(hbuttons[i]);
        hpane->add_content(*hbuttons[i]);
    }


    Vertical_pane *vpane = new Vertical_pane(800, 600, 300, 300);
    vpane->set_gap(5);
    vpane->get_posX_property().bind(ppane->get_width_property(), std::function<int (const int &)>([](const int &value){return value / 4;}));
    vpane->get_width_property().bind(ppane->get_width_property(), std::function<int (const int &)>([](const int &value){return value / 3;}));
    vpane->get_posY_property().bind(ppane->get_height_property(), std::function<int (const int &)>([](const int &value){return value * 1 / 2;}));
    vpane->get_height_property().bind(ppane->get_height_property(), std::function<int (const int &)>([](const int &value){return value / 2;}));

    Button *vbuttons[4] = {nullptr};
    for (int i = 0; i < 4; i++) {
        vbuttons[i] = new Button(to_string(i));
        vbuttons[i]->set_height(20 * (i + 1));
        vpane->add_node(vbuttons[i]);
        vpane->add_content(*vbuttons[i]);
    }
    vpane->add_node(hpane);
    vpane->add_content(*hpane);

    if (ppane->add_node(pButton) == false) {
        printf("add child failure!\n");
    }
    ppane->add_node(pButton_control);
    ppane->add_content(*pButton_control, 0, 0);
    ppane->add_node(pButton2);
    ppane->add_content(*pButton2, 100, 100);
    // ppane->add_node(new Button("Hello World", 300, 400));
    ppane->add_node(pboard);
    ppane->add_node(gpane);
    ppane->add_node(vpane);
    ppane->add_node(hpane);
    pWindow->add_node(ppane);
    pWindow->show();

// another window ------------------------------------------------------------------------------------------------------------------------
    Window *pWindow2 = new Window("Test2", 800, 600, 100, 100, Window_flag::window_flag_resizable);
    pWindow2->set_background_color(0, 125, 125, 200);
    pWindow2->set_posX(200);
    pWindow2->set_posY(300);
    Button *pButton3 = new Button();
    pButton3->set_posX(0);
    pButton3->set_posY(0);
    pButton3->set_width(100);
    pButton3->set_height(100);
    pButton3->add_listener([=](const Mouse_button_event &, void *){
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
        // btn->set_always_redraw(true);
        btn->add_listener([=](const Mouse_button_event &e, void *){
            btn->set_background_color(rand() %255, rand() %255, rand() %255, rand() %255, Element_status::button_normal);
        }, sui::Button::Button_event::be_up, nullptr);
        btn->set_background_color(rand() % 255, rand() % 255, rand() % 255, rand() % 255, Element_status::button_normal);
        pWindow2->add_node(btn);
        present_all();
    }, sui::Button::Button_event::be_up, nullptr);
    pWindow2->add_node(pButton3);
    pWindow2->show();

    Dialog *dlg = new Dialog(pWindow2, "message...", "dialog", 250, 200);
    dlg->show();
    return 0;
}