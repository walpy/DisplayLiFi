/**
 * Resolves how much register does it need for pull-up register
 *
 * (3.3VSource)
 *  |
 *  +-+---+
 *    |   |
 *    |   +--------(R)vvv-+
 *    +--------(R)vvv-+   |
 *                    |   |
 *  (SDA_DEVICE1)-----+----------(SDA_DEVICE2)
 *                        |
 *  (SCL_DEVICE1)---------+------(SCL_DEVICE2)
 *
 * http://electronics.stackexchange.com/questions/1849/is-there-a-correct-resistance-value-for-i2c-pull-up-resistors
 */

"use strict"

var VCC = 3.3; // 3.3 V
var C_BUS = 10 * Math.pow(10, -12); // 10 pF

var r_min = (VCC - 0.4) / (3 / 1000/*3mA*/);
var r_max = 300 * Math.pow(10, -9)/*10nSec*/ / C_BUS;

console.log("R_min: " + r_min + " \u03A9");
console.log("R_max: " + r_max + " \u03A9 (" + Math.round(r_max / 1000) + " k\u03A9)");
