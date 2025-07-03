type
  data1 = record
    x1 : real;
    y1 : real;
  end;

  data2 = record
    x2 : integer;
    y2 : integer;
  end;

var
  a : data1;
  b : data2;

function square(x : real) : real;
begin
  return(x * x);
end;

begin
  a.x1 := 10.2;
  b.x2 := 13;
  a.y1 := a.x1 + b.x2;
  writeln(square(a.y1));
end.