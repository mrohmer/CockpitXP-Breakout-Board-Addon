function isEventInactive(configName: String): Boolean;
begin
  Result := cpGetIntegerVar(configName + 'Active') = 0;
end;