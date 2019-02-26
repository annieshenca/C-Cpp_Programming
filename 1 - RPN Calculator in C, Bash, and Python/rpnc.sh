###############################################################################
# Annie Shen (ashen7)
# CMPS 109 Harrison - Spring 2018
# Lab 1 - Bash Reverse Polish Notation (RPN) Calculator
# Due Apr 8th, 2018
###############################################################################
#
# Accepts an arbitrary number of arguments in RPN, evaluates them and prints
# the answer to stdout.
#
# Help from lecture: https://opencast-player-1.lt.ucsc.edu:8443/engage/theodul/ui/core.html?id=764d4b0f-c245-4492-a317-4dded677a139

stack=()
next=0 # Stack pointer

# Due to bash reconizing '*' as something else, we're translating
# '*' into 'x' as taking in from argv.
rpn=`echo "$1" | sed 's/\*/x/g'`

for oper in $rpn; do

  if [[ "$oper" =~ ^[0-9]+$ ]]; then # If oper is a number
    stack[$next]=$oper # Insert operand into the stack

  else # If not a number, it must be a operator
    # Pop of two numbers infront of the operator
    (( idx1 = next - 2))
    (( idx2 = next - 1))
    (( next -= 2 )) # Move stack pointer back 2 due to popping off two operands
    num1=${stack[idx1]}
    num2=${stack[idx2]}

    if [[ "$oper" == "+" ]]; then
      (( result = $num1 + $num2 ))
    elif [[ "$oper" == "-" ]]; then
      (( result = $num1 - $num2 ))
    elif [[ "$oper" == "x" ]]; then
      (( result = $num1 * $num2 ))
    elif [[ "$oper" == "/" ]]; then
      (( result = $num1 / $num2 ))
    elif [[ "$oper" == "^" ]]; then
      (( result = $num1 ** $num2 ))
    fi

    stack[$next]=$result
  fi

  (( next += 1)) # Increment stack pointer
done
echo ${stack[0]}
