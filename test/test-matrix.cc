#include <catch2/catch.hpp>
#include "../include/matrix.h"

TEST_CASE( "Testing matrices", "[matrix]" ) {

    SECTION( "Transpose" ) {
        Mat<2,2,bool>mat_1 {
            true, false,
            true, false
        };
        auto mat_2 = mat_1.t();

        REQUIRE(mat_2(0,0) == true);
        REQUIRE(mat_2(0,1) == true);
        REQUIRE(mat_2(1,0) == false);
        REQUIRE(mat_2(1,1) == false);

        Mat<3,2, int> mat_3{
            2, 3,
            1, 0,
            5, 9
        };

        auto mat_4 = mat_3.transpose();

        auto [m,n] = mat_4.size();

        REQUIRE(m == 2);
        REQUIRE(n == 3);

        REQUIRE(mat_4(0,0) == mat_3(0,0));
        REQUIRE(mat_4(0,1) == mat_3(1,0));
        REQUIRE(mat_4(1,1) == mat_3(1,1));
        REQUIRE(mat_4(0,2) == mat_3(2,0));
        REQUIRE(mat_4(1,2) == mat_3(2,1));
        REQUIRE(mat_4(1,0) == mat_3(0,1)); 
    }

    SECTION( "Matrix multiplication" ) {
        Mat<3,3,double> mat_1{
            0.2, 0.2, 0.2, 
            0.3, 0.3, 0.3,
            0.1, 2., 3.};
        Mat<3,3,int> mat_2{
            1, 0, 0,
            0, 1, 0,
            0, 0, 1};

        auto mat_3 = mat_1 * mat_2;

        REQUIRE(mat_3(0,0) == 0.2);
        REQUIRE(mat_3(0,1) == 0.2);
        REQUIRE(mat_3(0,2) == 0.2);
        REQUIRE(mat_3(1,0) == 0.3);
        REQUIRE(mat_3(1,1) == 0.3);
        REQUIRE(mat_3(1,2) == 0.3);
        REQUIRE(mat_3(2,0) == 0.1);
        REQUIRE(mat_3(2,1) == 2.);
        REQUIRE(mat_3(2,2) == 3.);

        Mat<2,3,int> mat_4{
            1, 3, 5,
            4, 1, 6};
        Mat<3,1,bool> mat_5{
            false,
            true,
            false};

        auto mat_6 = mat_4 * mat_5;

        REQUIRE(mat_6(0,0) == 3);
        REQUIRE(mat_6(0,0) == 3);

        Mat<2, 1, float> mat_7{
            1.f,
            1.f
        };

        Mat<1, 2, float> mat_8{
            0.f, 10.f
        };

        auto number = mat_8 * mat_7;

        REQUIRE(number == 10.f); 
    }

    SECTION( "Matrix addition/subtraction" ) {
        Mat<3,3,float> mat_1 {
            1.7f, 0.2f, 0.3,
            0.7f, 0.8f, 0.9f,
            1.1f, 1.3f, 1.5f
        };

        auto mat_2{mat_1};

        auto mat_3 = mat_1 + mat_2;

        Mat<3,3,int> mat_4{1};

        REQUIRE(mat_4(0) == 1);
        REQUIRE(mat_4(1) == 0);

        auto mat_4_11 = mat_4(0,0);

        mat_4+=mat_1;
        // Type safety requires lhs type of += .
        REQUIRE(mat_4(0) == (int)( mat_4_11+ mat_1(0)) );

        Mat<2,2,float> mat_5{
            0.2, 0.0,
            0.3, 0.1
        };

        Mat<2,2,float> mat_6{
            0.1, 0.2,
            0.2, 0.3
        };

        auto mat_7 = mat_5 + mat_6;

        REQUIRE(mat_7(0,0) == 0.2f+0.1f);
        REQUIRE(mat_7(0,1) == 0.0f+0.2f);
        REQUIRE(mat_7(1,0) == 0.3f+0.2f);
        REQUIRE(mat_7(1,1) == 0.1f+0.3f);

        mat_7-= mat_6;

        REQUIRE(mat_7(0,0) == Approx( 0.2 ));
        REQUIRE(mat_7(0,1) == Approx( 0.0 ));
        REQUIRE(mat_7(1,0) == Approx( 0.3 ));
        REQUIRE(mat_7(1,1) == Approx( 0.1 ));

        auto mat_8 = mat_5 - mat_6;

        REQUIRE(mat_8(0,0) == Approx( 0.1 ));
        REQUIRE(mat_8(0,1) == Approx( -0.2 ));
        REQUIRE(mat_8(1,0) == Approx( 0.1 ));
        REQUIRE(mat_8(1,1) == Approx( -0.2 ));
    }
    
}