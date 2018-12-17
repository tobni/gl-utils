#include <catch2/catch.hpp>
#include "../include/matrix.h"
#include <iostream>

TEST_CASE( "Testing matrices", "[matrix]" ) {

    SECTION( "Transpose" ) {
        Mat<2,2,bool>mat_1 {
            true, false,
            true, false
        };
        auto mat_2 = mat_1.t();

        CHECK(mat_2(0,0) == true);
        CHECK(mat_2(0,1) == true);
        CHECK(mat_2(1,0) == false);
        CHECK(mat_2(1,1) == false);

        Mat<3,2, int> mat_3{
            2, 3,
            1, 0,
            5, 9
        };

        auto mat_4 = mat_3.transpose();

        auto [m,n] = mat_4.size();

        CHECK(m == 2);
        CHECK(n == 3);

        CHECK(mat_4(0,0) == mat_3(0,0));
        CHECK(mat_4(0,1) == mat_3(1,0));
        CHECK(mat_4(1,1) == mat_3(1,1));
        CHECK(mat_4(0,2) == mat_3(2,0));
        CHECK(mat_4(1,2) == mat_3(2,1));
        CHECK(mat_4(1,0) == mat_3(0,1)); 
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

        CHECK(mat_3(0,0) == 0.2);
        CHECK(mat_3(0,1) == 0.2);
        CHECK(mat_3(0,2) == 0.2);
        CHECK(mat_3(1,0) == 0.3);
        CHECK(mat_3(1,1) == 0.3);
        CHECK(mat_3(1,2) == 0.3);
        CHECK(mat_3(2,0) == 0.1);
        CHECK(mat_3(2,1) == 2.);
        CHECK(mat_3(2,2) == 3.);

        Mat<2,3,int> mat_4{
            1, 3, 5,
            4, 1, 6};
        Mat<3,1,bool> mat_5{
            false,
            true,
            false};

        auto mat_6 = mat_4 * mat_5;

        CHECK(mat_6(0,0) == 3);
        CHECK(mat_6(0,0) == 3);

        Mat<2, 1, float> mat_7{
            1.f,
            1.f
        };

        Mat<1, 2, float> mat_8{
            0.f, 10.f
        };

        auto number = mat_8 * mat_7;

        CHECK(number == 10.f); 
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

        CHECK(mat_4(0) == 1);
        CHECK(mat_4(1) == 0);

        auto mat_4_11 = mat_4(0,0);

        mat_4+=mat_1;
        // Type safety CHECKs lhs type of += .
        CHECK(mat_4(0) == (int)( mat_4_11+ mat_1(0)) );

        Mat<2,2,float> mat_5{
            0.2, 0.0,
            0.3, 0.1
        };

        Mat<2,2,float> mat_6{
            0.1, 0.2,
            0.2, 0.3
        };

        auto mat_7 = mat_5 + mat_6;

        CHECK(mat_7(0,0) == 0.2f+0.1f);
        CHECK(mat_7(0,1) == 0.0f+0.2f);
        CHECK(mat_7(1,0) == 0.3f+0.2f);
        CHECK(mat_7(1,1) == 0.1f+0.3f);

        mat_7-= mat_6;

        CHECK(mat_7(0,0) == Approx( 0.2 ));
        CHECK(mat_7(0,1) == Approx( 0.0 ));
        CHECK(mat_7(1,0) == Approx( 0.3 ));
        CHECK(mat_7(1,1) == Approx( 0.1 ));

        auto mat_8 = mat_5 - mat_6;

        CHECK(mat_8(0,0) == Approx( 0.1 ));
        CHECK(mat_8(0,1) == Approx( -0.2 ));
        CHECK(mat_8(1,0) == Approx( 0.1 ));
        CHECK(mat_8(1,1) == Approx( -0.2 ));
    }

    SECTION( "LUP decomposition") {
        Mat<2,2,float> mat_1 {
            0.0006, 3,
            5, 6};

        auto [P,L,U] = mat_1.decompose_PLU();

        auto mat_2 = P*L*U;

        CHECK(mat_1(0) == mat_2(0));
        CHECK(mat_1(1) == mat_2(1));
        CHECK(mat_1(2) == mat_2(2));
        CHECK(mat_1(3) == mat_2(3));

        Mat<4,4,float> mat_3 {
            0.02, 3, 0.003,
            5, 6, 0.2, 10000,
            345, 0.1, 34, 12,
            0, 123, 4, 0, 0.6};

        auto [P3,L3,U3] = mat_3.decompose_PLU();

        std::cout << "A = " << std::endl;
        std::cout << mat_3.print(12);

        std::cout << "P = " << std::endl;
        std::cout << P3.print(12);


        std::cout << "L = " << std::endl;
        std::cout << L3.print(12);


        std::cout << "U = " << std::endl;
        std::cout << U3.print(12);

        auto mat_4 = P3 * L3 * U3;
        CHECK(mat_3(0) == Approx( mat_4(0)));
        CHECK(mat_3(1) == Approx( mat_4(1)));
        CHECK(mat_3(2) == Approx( mat_4(2)));
        CHECK(mat_3(3) == Approx( mat_4(3)));
        CHECK(mat_3(4) == Approx( mat_4(4)));
        CHECK(mat_3(5) == Approx( mat_4(5)));
        CHECK(mat_3(6) == Approx( mat_4(6)));
        CHECK(mat_3(7) == Approx( mat_4(7)));
        CHECK(mat_3(8) == Approx( mat_4(8)));
        CHECK(mat_3(9) == Approx( mat_4(9)));
        CHECK(mat_3(10) == Approx( mat_4(10)));
        CHECK(mat_3(11) == Approx( mat_4(11)));
        CHECK(mat_3(12) == Approx( mat_4(12)));
        CHECK(mat_3(13) == Approx( mat_4(13)));
        CHECK(mat_3(14) == Approx( mat_4(14)));
        CHECK(mat_3(15) == Approx( mat_4(15)));

                
        Mat<2,4,float> mat_5 {
            0.02, 3, 0.003, 50,
            5, 6, 0.2, 1};

        auto [P5, L5, U5] = mat_5.decompose_PLU();

        auto mat_6 = P5 * L5 * U5;
        
        CHECK(mat_5(0) == Approx( mat_6(0)));
        CHECK(mat_5(1) == Approx( mat_6(1)));
        CHECK(mat_5(2) == Approx( mat_6(2)));
        CHECK(mat_5(3) == Approx( mat_6(3)));
        CHECK(mat_5(4) == Approx( mat_6(4)));
        CHECK(mat_5(5) == Approx( mat_6(5)));

        auto mat_7 = mat_5.t();

        auto [P7, L7, U7] = mat_7.decompose_PLU();

        auto mat_8 = P7 * L7 * U7;
        
        std::cout << "A =" << std::endl;
        std::cout << mat_6.print(12);
        CHECK(mat_7(0) == Approx( mat_8(0)));
        CHECK(mat_7(1) == Approx( mat_8(1)));
        CHECK(mat_7(2) == Approx( mat_8(2)));
        CHECK(mat_7(3) == Approx( mat_8(3)));
        CHECK(mat_7(4) == Approx( mat_8(4)));
        CHECK(mat_7(5) == Approx( mat_8(5)));
        
    }
    
}