program ejemplo;

var
  a, b, resultado: integer;

function suma(x, y: integer): integer;
begin
  suma := x + y;
end;

begin
  a := 7;
  b := 5;
  resultado := suma(a, b);
  writeln(resultado);
end.
