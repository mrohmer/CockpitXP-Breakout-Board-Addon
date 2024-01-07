function isEventActive(configName: String): Boolean;
begin
  Result := cpGetIntegerVar(configName + 'Active') = 1;
end;