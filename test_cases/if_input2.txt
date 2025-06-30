program ejemplo;

var
  x, y: integer;

begin
  x := 8;
  y := 3;

  if x < y then
    x := x + 1
  else
    y := y + 1;

  writeln(x);
  writeln(y);
end.
