program ejemplo;

var
  x, y, r: integer;

function mayor(a, b: integer): integer;
begin
  if a < b then
    mayor := b
  else
    mayor := a;
end;

begin
  x := 30;
  y := 20;
  r := mayor(x, y);
  writeln(r);
end.