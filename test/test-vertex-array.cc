#include <catch2/catch.hpp>
#include "../include/vertex-array.h"
#include "../include/window.h"

#include <utility>

TEST_CASE( "Testing VAO creaton", "[vao]" ) {
    // For the context
    auto window_option = make_window(600, 450, "Hello VAO");
    glewInit();
    //-----

    VertexArray vao_1{};
    VertexArray vao_2{};

    auto const id_1 = vao_1.get_id();

    REQUIRE(vao_1.get_id());
    REQUIRE(vao_2.get_id() != id_1);

    VertexArray vao_3{vao_1};
    REQUIRE(vao_3.get_id() == id_1);

    REQUIRE(vao_1.get_id() == id_1);

    auto vao_4{VertexArray()};
    auto vao_5{vao_1};
    REQUIRE(vao_4.get_id());
    REQUIRE(vao_5.get_id());
}