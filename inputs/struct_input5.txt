program ejemplo;

type
  Punto = record
    x: integer;
    y: integer;
  end;

var
  p: Punto;

function suma(x,y:integer): integer;
begin
  suma := x + y;
end;

begin
  p.x := 3;
  p.y := 2;
  writeln(suma(p.x,p.y));
end.
