#include <catch2/catch.hpp>
#include "../include/gl-utils.h"
#include <iostream>

using std::move;

TEST_CASE( "Testing window factory function", "[window]" ) {
    
    auto window_option = make_window(600, 450, "Hello Factory");
    REQUIRE(window_option.has_value());

    auto window = move(window_option.value());
    
    SECTION( "Window size is the same as asked" ) {
        REQUIRE(window.width() == 600);
        REQUIRE(window.height() == 450);
    }

    SECTION( "Window is open until called to close" ) {
        REQUIRE(window.should_close() == false);
        window.close();
        REQUIRE(window.should_close() == true);
    }
    
}


TEST_CASE( "Testing Input Manager key events", "[InputManager]" ) {
    
    auto window = *make_window(600, 450, "Hello Input");
    
    InputManager input_manager{};
    KeyPattern new_pattern{GLFW_KEY_A, {}, {}, {}};
    bool captured_bool = false;
    KeyCallback new_callback{
        "Awesome callback!",
        [&captured_bool] () {
            captured_bool = !captured_bool;
        }
    };

    SECTION( "The callback can affect captured values" ) {
        new_callback.callback();
        REQUIRE(captured_bool == true);
        new_callback.callback();
        REQUIRE(captured_bool == false);
        REQUIRE(new_callback.description == "Awesome callback!");
    }

    KeyPattern escape_pattern{GLFW_KEY_ESCAPE, {}, {}, {}};
    KeyCallback escape_callback{
        "Closes window!",
        [&window] () {
            window.close();
        }
    };
    
    SECTION( "Attaching the input manager to the window and call" ) {
        input_manager.add_input(new_pattern, move(new_callback));
        input_manager.add_input(escape_pattern, move(escape_callback));
        window.attach(move(input_manager));
        window.input_manager->press_key(GLFW_KEY_A, {}, {}, {});
        REQUIRE(captured_bool == true);
        window.set_title("Press 'A' to pass test");
        while(captured_bool) {
            window.poll();
            window.input_manager->press_key(GLFW_KEY_A, {}, {}, {});
        }
        REQUIRE(captured_bool == false);
        
        window.set_title("Press 'ESC' to pass test");
        while(!window.should_close()) {
            window.poll();
            window.input_manager->press_key(GLFW_KEY_ESCAPE, {}, {}, {});
        }
        REQUIRE(window.should_close());
    }
}