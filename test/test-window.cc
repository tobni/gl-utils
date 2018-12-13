#include <catch2/catch.hpp>
#include "../include/window.h"


TEST_CASE( "Testing window factory function", "[window]" ) {
    
    auto window_option = make_window(600, 450, "Hello Factory");
    REQUIRE(window_option.has_value());

    auto window = std::move(window_option.value());
    
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
