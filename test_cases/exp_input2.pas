program ExampleProgram;
var x, y: integer;
function suma(a, b: integer): integer;
begin
suma := a + b;
end;

begin
x := 1;
y := 20;
writeln(suma(x, y));
end.