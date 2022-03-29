/**
 * Tests for Test.cpp
 *
 * AUTHOR: Moriah David
 * 
 * Date: 2022-03
 */

#include "doctest.h"
#include "Notebook.hpp"
using namespace ariel;

#include <string>
using namespace std;

TEST_CASE("good input"){

    Notebook nb;

    // Check write and read
    nb.write(0, 0, 0, Direction::Horizontal, "thailand");
    nb.write(0, 0, 99, Direction::Horizontal, "m");
    CHECK(nb.read(0, 0, 99, Direction::Horizontal, 1) == "m");
    CHECK(nb.read(0, 0, 0, Direction::Horizontal, 8) == "thailand");
    CHECK(nb.read(0, 0, 0, Direction::Vertical, 8) == "t_______");
    nb.write(0, 2, 0, Direction::Horizontal, "vietnam");
    CHECK(nb.read(0, 2, 2, Direction::Horizontal, 7) == "etnam__");
    CHECK(nb.read(0, 0, 0, Direction::Vertical, 5) == "t_v__");
    CHECK(nb.read(0, 0, 7, Direction::Vertical, 4) == "d___");
    nb.write(3, 0, 70, Direction::Horizontal, "_");
    nb.write(3, 0, 67, Direction::Horizontal, "_____");
    CHECK(nb.read(3, 0, 67, Direction::Horizontal, 5) == "_____");

    CHECK_NOTHROW(nb.write(3, 0, 67, Direction::Vertical, string(101, '*')));
    CHECK_NOTHROW(nb.read(3, 0, 67, Direction::Vertical, 102));
    CHECK_NOTHROW(nb.erase(3, 0, 67, Direction::Vertical, 102));

    // Check erase and read
    nb.write(1, 0, 0, Direction::Horizontal, "pizza");
    nb.write(1, 1, 0, Direction::Horizontal, "crep");
    nb.erase(1, 1, 1, Direction::Horizontal, 1);
    CHECK(nb.read(1, 1, 0, Direction::Horizontal, 4) == "c~ep");
    CHECK(nb.read(1, 0, 1, Direction::Vertical, 3) == "i~_");
    nb.erase(1, 0, 0, Direction::Horizontal, 5);
    CHECK(nb.read(1, 0, 0, Direction::Horizontal, 5) == "~~~~~");
    CHECK(nb.read(1, 0, 2, Direction::Horizontal, 5) == "~~~__");
    CHECK(nb.read(1, 0, 5, Direction::Horizontal, 2) == "__");
    // Erase on ~ area
    nb.erase(1, 0, 1, Direction::Horizontal, 5);

    // Check read before write
    CHECK(nb.read(30, 1, 9, Direction::Horizontal, 9) == "_________");
    CHECK(nb.read(30, 1, 9, Direction::Vertical, 9) == "_________");

}

TEST_CASE("bad input"){

    Notebook nb;

    // Write on 101 row
    CHECK_THROWS(nb.write(2, 4, 101, Direction::Horizontal, "pizza"));
    CHECK_THROWS(nb.write(2, 4, 100, Direction::Horizontal, "bb"));
    CHECK_THROWS(nb.write(2, 4, 101, Direction::Vertical, "pizza"));

    // Read on 101 row
    CHECK_THROWS(nb.read(2, 4, 101, Direction::Horizontal, 5));
    CHECK_THROWS(nb.read(2, 4, 100, Direction::Horizontal, 2));
    CHECK_THROWS(nb.read(2, 4, 101, Direction::Vertical, 5));

    // Write on ~ area
    nb.erase(2, 4, 50, Direction::Horizontal, 4);
    CHECK_THROWS(nb.write(2, 4, 50, Direction::Horizontal, "pizza"));
    nb.erase(2, 4, 1, Direction::Vertical, 4);
    CHECK_THROWS(nb.write(2, 4, 1, Direction::Horizontal, "abcd"));

    // Read write and erase more than 100
    CHECK_THROWS(nb.write(6, 4, 0, Direction::Horizontal, string(101, '*')));
    CHECK_THROWS(nb.write(6, 8, 50, Direction::Horizontal, string(60, '*')));
    CHECK_THROWS(nb.read(6, 4, 0, Direction::Horizontal, 101));
    CHECK_THROWS(nb.read(6, 4, 30, Direction::Horizontal, 90));
    CHECK_THROWS(nb.erase(66, 4, 0, Direction::Horizontal, 101));
    CHECK_THROWS(nb.erase(66, 4, 50, Direction::Horizontal, 60));

    // Write ~
    CHECK_THROWS(nb.write(7, 4, 50, Direction::Horizontal, "pizz~a"));
    CHECK_THROWS(nb.write(8, 4, 50, Direction::Horizontal, "~~"));

    // Write \n...
    for(char i = 0; i <= 31; i++){
        string s = "abc";
        s += i;
        CHECK_THROWS(nb.write(24, 2, 50, Direction::Horizontal, s));
    }
        string s = "abc";
        s += 127;
        CHECK_THROWS(nb.write(24, 2, 50, Direction::Horizontal, s));

    // Write negative numbers
    CHECK_THROWS(nb.write(-26, 2, 50, Direction::Horizontal, "hey"));
    CHECK_THROWS(nb.write(26, -2, 50, Direction::Horizontal, "hey"));
    CHECK_THROWS(nb.write(26, 2, -50, Direction::Horizontal, "hey"));
    CHECK_THROWS(nb.write(-26, -2, -50, Direction::Horizontal, "hey"));

    // Read negative numbers
    CHECK_THROWS(nb.read(-26, 2, 50, Direction::Horizontal, 3));
    CHECK_THROWS(nb.read(26, -2, 50, Direction::Horizontal, 3));
    CHECK_THROWS(nb.read(26, 2, -50, Direction::Horizontal, 3));
    CHECK_THROWS(nb.read(-26, -2, -50, Direction::Horizontal, 3));
    CHECK_THROWS(nb.read(26, 2, 50, Direction::Horizontal, -3));

    // Erase negative numbers
    CHECK_THROWS(nb.erase(-26, 2, 50, Direction::Horizontal, 3));
    CHECK_THROWS(nb.erase(26, -2, 50, Direction::Horizontal, 3));
    CHECK_THROWS(nb.erase(26, 2, -50, Direction::Horizontal, 3));
    CHECK_THROWS(nb.erase(-26, -2, -50, Direction::Horizontal, 3));
    CHECK_THROWS(nb.erase(26, 2, 50, Direction::Horizontal, -3));
}
