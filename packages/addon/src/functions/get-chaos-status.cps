function getChaosStatus(parameter: Integer): Boolean;
begin
  if (parameter = 6) or (parameter = 7) then
  begin
    Result := true;
  end
  else
  begin
    Result := false;
  end;
end;

