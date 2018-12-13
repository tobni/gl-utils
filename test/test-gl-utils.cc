#include <catch2/catch.hpp>
#include "../include/gl-utils.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <array>
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

TEST_CASE( "Testing VAO/VBO creaton", "[vertex-array-object]" ) {
    // For the context
    auto window_option = make_window(600, 450, "Hello VAO");
    glewInit();
    //-----

    VertexArrayObject vao{};
    std::array<GLfloat, 3> array_1{0.1f, 0.2f, 0.3f};
    std::array<GLfloat, 6> array_2{0.5f, 0.3f, 0.1f, 0.6f, 0.4f, 0.6f};
    std::array<std::string, 2> names{"data", "values"};

    std::ostringstream out;
    out << vao;
    REQUIRE(out.str() == "id=" + std::to_string(vao.get_id()) + " buffers= []");
    
    vao.add_buffer(gsl::span(array_1.data(), array_1.size()), 1, names[0]);
    vao.add_buffer(gsl::span(array_2.data(), array_2.size()), 2, names[1]);

    std::ostringstream().swap(out);
    out << vao;
    REQUIRE(out.str() == "id=" + std::to_string(vao.get_id()) + " buffers= [data, values]");
    
    std::ostringstream().swap(out);
    vao.remove_buffer(names[0]);
    out << vao;
    REQUIRE(out.str() == "id=" + std::to_string(vao.get_id()) + " buffers= [values]");

    std::ostringstream().swap(out);
    vao.remove_buffer(names[1]);
    out << vao;
    REQUIRE(out.str() == "id=" + std::to_string(vao.get_id()) + " buffers= []");

    std::ostringstream().swap(out);
    vao.add_buffer(gsl::span(array_1.data(), array_1.size()), 1, names[0]);
    vao.add_buffer(gsl::span(array_2.data(), array_2.size()), 2, names[1]);

    VertexArrayObject vao_2{std::move(vao)};
    out << vao_2;
    REQUIRE(out.str() == "id=" + std::to_string(vao_2.get_id()) + " buffers= [data, values]");

}