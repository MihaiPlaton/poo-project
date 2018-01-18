//
// Created by Ionescu Alex on 17/01/2018.
//

#ifndef POO_ALPR_POSTPROCESS_H
#define POO_ALPR_POSTPROCESS_H

#include <string>
using std::string;
#include <regex>

#include "PossiblePlate.h"


const std::regex e ("[B](0[1-9]|[1-9][0-9])(([A-Z]{1}[0]{1}[A-Z]{1})|"
                            "([A-Z]{2}[0]{1})|([0]{1}[A-Z]{2})|([0]{1}[A-Z]{1}[0]{1})|([A-Z]{3}))|"
                            "((B|AB|AG|AR|BC|BH|BN|BR|BT|BV|BZ|CJ|CL|CS|CT|CV|DB|DJ|GJ|GL|GR|HD|HR|IF|IL|IS|MH|MM|MS|NT|OT|PH|SB|SJ|SM|SV|TL|TM|TR|VL|VN|VS)(0[1-9]|[1-9][0-9])(([A-Z]{1}[0]{1}[A-Z]{1})|"
                            "[A-Z]{2}[0]{1})|([0]{1}[A-Z]{2})|([0]{1}[A-Z]{1}[0]{1})|([A-Z]{3})))|"
                            "[B]([1-9][0-9][0-9])(([A-Z]{1}[0]{1}[A-Z]{1})|([A-Z]{2}[0]{1})|"
                            "([0]{1}[A-Z]{2})|([0]{1}[A-Z]{1}[0]{1})|([A-Z]{3}))");

const std::regex alt ("(AB|AG|AR|BC|BH|BN|BR|BT|BV|BZ|CJ|CL|CS|CT|CV|DB|DJ|GJ|GL|GR|HD|HR|IF|IL|IS|MH|MM|MS|NT|OT|PH|SB|SJ|SM|SV|TL|TM|TR|VL|VN|VS)[0-9]{2}[A-Z]{3}");
const std::regex buc ("B[0-9]{2,3}[A-Z]{3}");

void testRegex(PossiblePlate &plate);

#endif //POO_ALPR_POSTPROCESS_H
