/*
 * Node.js / JavaScript Reverse Polish Notation (RPN) Calculator
 *
 * Accepts an single argument in RPN, evaluates it and prints the answer to stdout.
 *
 */

// process.argv[2] gives the command line argument for the calculations part.

var stack = [];

var hi = "one two three four five";
// splice then split
// args.length need - 1
// var arr = process.argv.slice(2);
// var temp = arr.split(" ");
// console.log(args);
// console.log(temp);
// for (var i = 0; i < arr.length; i++) {
//     temp[i] = ++arr[i];
// }

var hii = hi.split(" ");
console.log(hii);

// console.log(args);
// for (var i = 0; i < args.length; i++) {
//     stack.push(args[i]);
// }
//
// console.log(stack);
//
//
// for (var j = 0; j < stack.length; j++) {
//     console.log(stack[j]);
//     console.log(",");
// }
// Create an event handler.
// var hello = function () {
//
// }

// process.argv.forEach(function (val, index, array) {
//     console.log(index + ': ' + val);
// });
