program ejemplo;

type
  Datos = record
    x: real;
    y: integer;
  end;

var
  d: Datos;
  a: real;
  r: integer;

function sumaEntera(b: integer): integer;
begin
  sumaEntera := b + d.y;
end;

begin
  d.x := 10.58;
  d.y := 3;

  writeln(d.x);
  writeln(d.y);

  a := d.x + 4.2;
  r := sumaEntera(7);

  writeln(a);
  writeln(r);
end.
