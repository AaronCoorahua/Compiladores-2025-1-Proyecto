program ejemplo;

var
  x, y: integer;

begin
  x := 8;
  y := 3;

  if x < y then
  begin
    writeln(x);
    x := x + 1;
  end
  else
  begin
    writeln(y);
    y := y + 1;
  end;

  writeln(x);
  writeln(y);
end.
