#include <catch2/catch.hpp>
#include "../include/vertex-buffer.h"
#include "../include/window.h"

#include <utility>
#include <array>

using std::byte;

struct Data {
    float position[3];
    unsigned char color[4];
};

TEST_CASE( "Testing VBO creaton", "[vbo]" ) {
    // For the context
    auto window_option = make_window(600, 450, "Hello VAO");
    glewInit();
    //-----
    Data my_data[3] = { 
        {0.2, 0.2, 0.3, 123, 0, 23, 255}, 
        {0.2, 0.2, 0.3, 123, 0, 23, 255},
        {0.2, 0.2, 0.3, 123, 0, 23, 255}
    };
    VertexBuffer vbo_1{};
    VertexBuffer vbo_2{};

    REQUIRE(vbo_1.get_id() != vbo_2.get_id());
    
    vbo_1.data(gsl::span(my_data));

    REQUIRE(vbo_1.get_data_size() == (sizeof(float)*3 + sizeof(unsigned char)*4));
}