program ejemplo;

type
  Datos = record
    x: integer;
    y: integer;
    r: integer;
  end;

var
  d: Datos;

function mayor(a, b: integer): integer;
begin
  if a < b then
    mayor := b
  else
    mayor := a;
end;

begin
  d.x := 10;
  d.y := 20;
  d.r := mayor(d.x, d.y);
  writeln(d.r);
end.