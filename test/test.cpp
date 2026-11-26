// #include <catch2/catch_test_macros.hpp>
// #include <iostream>
// #include <sstream>
// #include <string>
// using namespace std;
// // change if you choose to use a different header name
// #include "CampusCompass.h"
// string parserMain(const string& input) {
//   CampusCompass compass;
//   compass.ParseCSV("data/edges.csv", "data/classes.csv");
//
//   ostringstream output;
//   streambuf* old = cout.rdbuf(output.rdbuf());
//
//   istringstream ss(input);
//   int n;
//   ss >> n;
//   ss.ignore();
//   string line;
//   for (int i = 0; i < n; i++) {
//     getline(ss, line);
//     compass.ParseCommand(line);
//   }
//
//   cout.rdbuf(old);
//   return output.str();
// }
//
// using namespace std;
//
// TEST_CASE("Five Incorrect Commands", "[error]") {
//   // 7 digit ufid
//   string one=parserMain(R"(1 insert "Brandon" 4569999 20 2 COP3530 MAC2311)");
//   //typo for command
//   string two=parserMain(R"(1 inser "Brandon" 45679999 20 2 COP3530 MAC2311)");
//   //invalid class
//   string three=parserMain(R"(1 insert "Brandon" 45679991 20 2 CP3530 MAC2311)");
//   //not alphabet
//   string four=parserMain(R"(1 insert "Brandon!" 45679992 20 2 COP3530 MAC2311)");
//   //repeated ID
//   string five=parserMain(R"(2 insert "Brandon" 45679999 20 2 COP3530 MAC2311
// insert "Brandon" 45679999 20 2 COP3530 MAC2311)");
//   // anything that evaluates to false in a REQUIRE block will result in a
//   // failing test
//   REQUIRE(one == "unsuccessful\n");
//   REQUIRE(two == "unsuccessful\n");
//   REQUIRE(three == "unsuccessful\n");
//   REQUIRE(four == "unsuccessful\n");
//   REQUIRE(five == "successful\nunsuccessful\n");
//
// }
//
// TEST_CASE("Edge Cases", "[error]") {
//   // remove student that doesnt exist
//   string one = parserMain(R"(1 remove 99999999)");
//   //
//   parserMain(R"(1 insert "Brandon" 45679992 20 2 COP3530 MAC2311)");
//   // student doesnt have class
//   string two = parserMain(R"(1 dropClass 45679992 CDA3101)");
//   //empty class
//   string three = parserMain(R"(1 removeClass CDA3101)");
//   REQUIRE(one == "unsuccessful\n");
//   REQUIRE(two == "unsuccessful\n");
//   REQUIRE(three == "unsuccessful\n");
// }
// TEST_CASE("Drop,replace,remove Class and remove", "[error]") {
//   string input = R"(6
// insert "Student A" 10000001 1 1 COP3502
// insert "Student B" 10000002 1 1 COP3502
// insert "Student C" 10000003 1 2 COP3502 MAC2311
// dropClass 10000001 COP3502
// remove 10000001
// removeClass COP3502
// )";
//
//   string expectedOutput = R"(successful
// successful
// successful
// successful
// unsuccessful
// 2
// )";
//
//   string actualOutput=parserMain(input);
//
//   REQUIRE(actualOutput == expectedOutput);
// }
//
// TEST_CASE("shortest edge", "[error]") {
//   string input = R"(9
// insert "Brandon" 45679999 20 2 COP3530 MAC2311
// insert "Brian" 35459999 21 3 COP3530 CDA3101 MAC2311
// insert "Briana" 87879999 22 3 CDA3101 MAC2311 EEL3701
// removeClass COP3530
// remove 87879999
// removeClass CDA3101
// printShortestEdges 35459999
// toggleEdgesClosure 2 12 21 21 22
// printShortestEdges 35459999
// )";
//
//   string expectedOutput = R"(successful
// successful
// successful
// 2
// successful
// 1
// Time For Shortest Edges: Brian
// MAC2311: 11
// successful
// Time For Shortest Edges: Brian
// MAC2311: -1
// )";
//
//   string actualOutput=parserMain(input);
//
//   REQUIRE(actualOutput == expectedOutput);
// }
