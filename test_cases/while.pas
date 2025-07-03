program test;
var 
  x : real;
  y : real;
begin
  x := 5.0;
  while x > 4.5 do
  begin
    x := x - 0.1;
    writeln(x);
  end;
end.
