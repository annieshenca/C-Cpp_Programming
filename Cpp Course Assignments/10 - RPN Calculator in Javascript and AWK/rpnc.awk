#
# Awk Reverse Polish Notation (RPN) Calculator
#
# Accepts an single argument in RPN, evaluates it and prints the answer to stdout.
#
BEGIN {
    # ARGV[1] gets the number arguments as a whole. n is the numer for arr's length
    len = split(ARGV[1],arr," ")
    empty = "0 0 0" # The set up code for the stack/array
    elm = split(empty,stack," ")
    j = 0
    # Loop through to see operantors and operations.
    for (i = 1; i <= len; i++){
        # If it IS a number, append to stack!
        if ( arr[i] ~ /[0-9]/ ) {
            stack[++j] = arr[i]

        # If it is NOT a number, do operations.
        } else {
            num2 = stack[j];
            num1 = stack[j-1];
            if (arr[i] == "+") {
                result = num1 + num2;
            } else if (arr[i] == "-") {
                result = num1 - num2;
            } else if (arr[i] == "*") {
                result = num1 * num2;
            } else if (arr[i] == "/") {
                result = num1 / num2;
            } else {
                result = num1 ^ num2;;
            }
            stack[--j] = result;
        }

    }

    print stack[1];
    # for (i = 1; i <= elm; i++) {
    #     print stack[i];
    # }
}


# {split($0,a,"|"); print a[3],a[2],a[1]}
