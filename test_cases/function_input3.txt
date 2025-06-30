program ejemplo;

type
  Punto = record
    x: integer;
    y: integer;
  end;

var
  p: Punto;
  suma: integer;

function sumacoordenadas(x,y: integer): integer;
begin
  sumacoordenadas := x + y;
end;

begin
  p.x := 3;
  p.y := 4;
  suma := sumacoordenadas(p.x,p.y);
  writeln(suma);
end.
