import std/strscans

var l, w, h: int
var a = 0
while true:
  var line: string
  try:
    line = readLine(stdin)
  except:
    break
  if scanf(line, "$ix$ix$i", l, w, h):
    a += 2*l*w+2*l*h+2*w*h + min(l*w, min(l*h, w*h))
  else:
    break

echo "Part one: ", a
