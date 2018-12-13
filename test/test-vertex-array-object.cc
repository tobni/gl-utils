#include <catch2/catch.hpp>
#include "../include/vertex-array-object.h"
#include "../include/window.h"
#include <string>
#include <sstream>
#include <vector>
#include <array>




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
    REQUIRE(out.str() == "id=" + std::to_string(vao.get_id()) + " buffers= [data:" + std::to_string(vao.get_buffer_id("data").value_or(0)) + ", values:" + std::to_string(vao.get_buffer_id("values").value_or(0)) + "]");
    
    std::ostringstream().swap(out);
    vao.remove_buffer(names[0]);
    out << vao;
    REQUIRE(out.str() == "id=" + std::to_string(vao.get_id()) + " buffers= [values:" + std::to_string(vao.get_buffer_id("values").value_or(0)) + "]");

    std::ostringstream().swap(out);
    vao.remove_buffer(names[1]);
    out << vao;
    REQUIRE(out.str() == "id=" + std::to_string(vao.get_id()) + " buffers= []");

    std::ostringstream().swap(out);
    vao.add_buffer(gsl::span(array_1.data(), array_1.size()), 1, names[0]);
    vao.add_buffer(gsl::span(array_2.data(), array_2.size()), 2, names[1]);

    VertexArrayObject vao_2{std::move(vao)};
    out << vao_2;
    REQUIRE(out.str() == "id=" + std::to_string(vao_2.get_id()) + " buffers= [data:" + std::to_string(vao_2.get_buffer_id("data").value_or(0)) + ", values:" + std::to_string(vao_2.get_buffer_id("values").value_or(0)) + "]");
}