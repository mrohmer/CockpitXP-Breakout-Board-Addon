function boolToInt(Value: Boolean):Integer;
begin
  if Value then
  begin
    Result:=1;
  end
  else
  begin
    Result:=0;
  end;
end;
