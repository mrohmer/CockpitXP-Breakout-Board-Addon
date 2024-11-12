function getFinishedStatus(parameter: Integer): Boolean;
begin
  if (parameter = 0) then
  begin
    Result := false;
  end
  else if (parameter = 3) or (parameter = 4) or (cpGetIntegerVar('IsFinished') = 1) then
  begin
    Result := true;
  end
  else
  begin
    Result := false;
  end;
end;