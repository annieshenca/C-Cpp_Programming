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
for (var i = 0; i < args.length; i++) {
    stack.push(args[i]);
}

console.log(stack);


for (var j = 0; j < stack.length; j++) {
    console.log(stack[j]);
    console.log(",");
}
// Create an event handler.
// var hello = function () {
//
// }

// process.argv.forEach(function (val, index, array) {
//     console.log(index + ': ' + val);
// });
