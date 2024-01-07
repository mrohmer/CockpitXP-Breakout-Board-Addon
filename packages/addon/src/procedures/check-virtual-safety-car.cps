procedure checkVirtualSafetyCar;
var
  value, oldValue: Boolean;
begin
  if (not isEventActive('VirtualSafetyCar')) then
  begin
    exit;
  end;

  value := cpGetIntegerVarG(cpGetIntegerVar('VirtualSafetyCarVariable'));
  oldValue := cpGetIntegerVar('VirtualSafetyCar:OldValue') = 1;
  if (oldValue = value) then
  begin
    exit;
  end;

  EnqueueVirtualSafetyCarEvent(value);
  cpSetIntegerVar('VirtualSafetyCar:OldValue', boolToInt(value));
end;