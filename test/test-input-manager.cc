#include <catch2/catch.hpp>
#include "../include/input-manager.h"
#include "../include/window.h"

TEST_CASE( "Testing Input Manager key events", "[input-manager]" ) {
    
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
        input_manager.add_input(new_pattern, std::move(new_callback));
        input_manager.add_input(escape_pattern, std::move(escape_callback));
        window.attach(std::move(input_manager));
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