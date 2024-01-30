import strutils # parseInt in readLine(stdin).parseInt()

echo "Enter the desired #1s and #0s:"
let O = readLine(stdin).parseInt()
let Z = readLine(stdin).parseInt()
echo "O=", O, " Z=", Z
#FIXME unhandled exception ValueError: invalid integer

let str = join(['0'.repeat(Z), '1'.repeat(O)])
echo str
