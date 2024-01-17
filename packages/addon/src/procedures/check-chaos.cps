function getChaosValue(parameter: Integer): Boolean;
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
procedure checkChaos(parameter: Integer);
var
  value, oldValue: Boolean;
begin
  if (isEventInactive('Chaos')) then
  begin
    exit;
  end;

  value := getChaosValue(parameter);
  oldValue := cpGetIntegerVar('Chaos:OldValue') = 1;
  if (oldValue = value) then
  begin
    exit;
  end;

  EnqueueChaosChange(value);
  cpSetIntegerVar('Chaos:OldValue', boolToInt(value));
end;