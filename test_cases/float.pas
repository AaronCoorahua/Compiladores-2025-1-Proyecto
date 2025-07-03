program comparacion;

var
  x : integer;
  y : real;


begin
  y := -4;
  x := y.toInt();

  if x > y then
    writeln(x)
  else
    writeln(y);
end.
