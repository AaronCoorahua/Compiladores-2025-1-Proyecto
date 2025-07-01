program demo;

var i, resultfor, countback : integer;

begin
  resultfor := 0;

  for i := 1 to 5 do
    resultfor := resultfor + i;
  writeln(resultfor);

  for i := 5 downto 1 do
  begin
    countback := i;
    writeln(countback);
  end;
end.
