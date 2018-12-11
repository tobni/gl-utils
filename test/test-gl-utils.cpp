#include <catch2/catch.hpp>
#include "../include/gl-utils.hpp"
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
    KeyPattern new_pattern{ 4, {}, 2, {} };
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
    
    SECTION( "Attaching the input manager to the window and call" ) {
        input_manager.add_input(new_pattern, move(new_callback));
        window.attach(move(input_manager));
        window.input_manager->press_key(4, {}, 2, {});
        REQUIRE(captured_bool == true);
    }
}