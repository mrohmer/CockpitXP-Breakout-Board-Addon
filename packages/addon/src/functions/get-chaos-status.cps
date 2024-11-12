function getChaosStatus(parameter: Integer): Boolean;
begin
  if cpGetRacingStatus() = 'P' then
  begin
    Result := true;
  end
  else
  begin
    Result := false;
  end;
end;