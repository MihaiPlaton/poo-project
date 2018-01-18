//
// Created by Ionescu Alex on 17/01/2018.
//

#include <iostream>
#include "Postprocess.h"

void testRegex(PossiblePlate &plate) {
    std::smatch sm;    // same as std::match_results<string::const_iterator> sm;
    std::regex_search (plate.strChars, sm, alt);

    for (const auto &i : sm) {
        std::string current = i.str();

        if (i.length() == 7) {
            plate.strChars = current;
            plate.patternVerified = true;

            std::cout << "Found normal pattern: " << i.str() << "\n";

            return;
        }
    }

    std::regex_search (plate.strChars, sm, buc);
    for (const auto &i : sm) {
        std::string current = i.str();

        if (i.length() == 6 || i.length() == 7) {
            plate.strChars = current;
            plate.patternVerified = true;

            std::cout << "Found BUC pattern: " << i.str() << "\n";
        }
    }
}
