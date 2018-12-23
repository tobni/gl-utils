#include <catch2/catch.hpp>
#include "../include/mat.h"
#include "../include/vec.h"

TEST_CASE( "", "[vec]" ) {

    SECTION( "Constructing" ) {
        Vec<3,1,float> vec_1{
            0.3,
            0.3,
            0.3
        };
    }

    SECTION( "Swizzling" ) {
        Vec<2,1,bool> vec_1{
            false,
            true
        };

        auto vec_2{vec_1(0,1,0,1,1)};

        CHECK( vec_2(0) == false );
        CHECK( vec_2(1) == true );
        CHECK( vec_2(2) == false );
        CHECK( vec_2(3) == true );
        CHECK( vec_2(4) == true );

        Vec<1,2,bool> vec_3 {
            true, false
        };

        auto vec_4 = vec_3(1,0,1);

        CHECK( vec_4(0) == false );
        CHECK( vec_4(1) == true );
        CHECK( vec_4(2) == false );

        vec_2(0) = true;

        CHECK( vec_2(0) == true );

        vec_2(4) = vec_1(0);


        CHECK(vec_2(4) == vec_4(0));
    }

    SECTION( "Vector Matrix Multiplication" ) {
        Vec<3,1, float> vec_1 {
            0.1,
            0.2,
            0.1
        };
        Mat<3,3,float> mat_1 {
            2, 0, 0,
            0, 2, 0,
            0, 0, 2
        };

        auto vec_2 = mat_1 * vec_1;

        CHECK( vec_2(0) == 0.2f );
        CHECK( vec_2(1) == 0.4f );
        CHECK( vec_2(2) == 0.2f );
        
        Vec<1,2,int> vec_3 {
            0, 2
        };

        Mat<2,2,int> mat_2 {
            1,0,
            0,-1
        };

        auto vec_4 = vec_3 * mat_2;
    }
}