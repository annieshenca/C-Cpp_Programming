/*
 * Node.js / JavaScript Reverse Polish Notation (RPN) Calculator
 *
 * Accepts an single argument in RPN, evaluates it and prints the answer to stdout.
 *
 */

/*
 * Sources:
 * https://stackoverflow.com/questions/4351521/how-do-i-pass-command-line-arguments-to-a-nodejs-program
 */



// args.length need - 1
var arr = (process.argv[2]).split(" ");
var stack = [];

// Length - 1 due to hte last " ".
for (var i = 0; i < arr.length-1; i++) {
    arr[i].toString();

    // If it IS a number, append push to stack!
    if (isNaN(arr[i]) == false) {
        stack.push(arr[i]);
    }

    // If it is NOT a number, do operations!
    else {
        var result = 0;
        // Pop the stack to get the last two numbers.
        var num2 = parseInt(stack.pop());
        var num1 = parseInt(stack.pop());

        // Consider all five different operants
        if (arr[i] == "+") {
            result = num1 + num2;
        } else if (arr[i] == "-") {
            result = num1 - num2;
        } else if (arr[i] == "*") {
            result = num1 * num2;
        } else if (arr[i] == "/") {
            result = num1 / num2;
        } else {
            result = Math.pow(num1, num2);
        }

        // Push in the result at the back of the stack.
        stack.push(result);
    }
}
console.log(stack[0]);
