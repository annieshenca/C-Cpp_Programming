/*
 * Node.js / JavaScript Reverse Polish Notation (RPN) Calculator
 *
 * Accepts an single argument in RPN, evaluates it and prints the answer to stdout.
 *
 */

// process.argv[2] gives the command line argument for the calculations part.


var args = process.argv.slice(2);
var stack = [];

// console.log(args);
// for (var i = 0; i < args.length; i++) {
//     console.log(args[i]);
//     console.log(",");
// }

console.log(args);
// Create an event handler.
// var hello = function () {
//
// }

// process.argv.forEach(function (val, index, array) {
//     console.log(index + ': ' + val);
// });
