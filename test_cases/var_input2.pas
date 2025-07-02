program test3;

type
  Punto = record
    x: integer;
    y: integer;
  end;

var
  p1, p2: Punto;
begin
  p1.x :=1;
  writeln(p1.x);

end.
