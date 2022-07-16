# SUI

A simple UI library built base on SDL2

## Build

- build on ubuntu

  1. install SDL2 library

     ```sh
     sudo apt-get install libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev
     ```

  2. build

     ```sh
     make
     ```

  3. simply test

     ```sh
     make test
     # before you run this test, you are supposed to prepare a background.jpg under the `./`
     # and you are supposed to modified the font used for button
     # if you are using ubuntu, may you can modified the font to `"/usr/share/fonts/truetype/ubuntu/UbuntuMono-B.ttf"` in `source/ui/SUI_button.cpp:23:32`
     ./test/sui_test
     ```

- build on widnow

  1. install SDL2, SDL2_ttf and SDL2_image
  2. change the %PATH% or make the header files with the code
  3. build

tips: you can define `DEBUG_OPEN` in `source/include/SUI_in_debug.h` to output the information of running

## Quickly start

When you finish building, you will get the static library in `build`.

you could move the `include` folder and the `libsui.a` to your project root.

You can simply create a window just call `Window *pw = new Window(title_name, width, height)` and then show the window by calling `pw->show()`

You can create a button by calling `Button *pb = new Button(infomation, x, y, w, h)` and then show it on the widnow by calling `pw->add_node(pb)`

```cpp
// this is a demo
// demo.cpp
#include <cstdlib>
#include "SUI.h"
using namespace sui;
// notice that you should use the prototype int main(int, char **)
int main(int argc, char *argv[]) {
    Window *pw = new Window("demo", 800, 600);
    Button *pb = new Button("demo", 100, 100, 160, 90);

    pb->add_listener([=](){
        // when click the button, change the background color
        pb->set_background_color(rand() % 255, rand() % 255, rand() % 255, rand() % 255, Element_status::button_normal);
        // you need to update the window and the button as you change the color
        // button should be redraw
        pb->set_redraw_flag(true);
        // window should be update
        present_all();
    });
    // show the window
    pw->add_node(pb);
    pw->show();
    return 0;
}
```

```sh
g++ -g -Wall -Iinclude -o demo demo.cpp -L. -lsui -lSDL2 -lSDL2_ttf -lSDL2_image
./demo
```

May be you can add `-no-pie` flag to achieve that the program can be executed by double clicking if you use the high version g++.

## Notice

*you should use the prototype `int main(int, char **)`*

*remember to call `object->set_redraw_flag(true)` if you change the style of the object*

*remember to call `present_all()` if you need to show something new on the window*

*you could simply create any object which type is derived from Object by new and not delete it, the library will manage it for you.*

*you are not support to create any object which type is derived from Object **on stack**!*

*Be sure that you create **at least one** window and **show** it*

*every window **should be closed** by user or call `pw->close()`*
