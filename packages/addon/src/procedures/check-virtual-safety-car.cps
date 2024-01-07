procedure checkVirtualSafetyCar;
var
  value, oldValue: Boolean;
begin
  if (isEventInactive('VirtualSafetyCar')) then
  begin
    exit;
  end;

  value := cpGetIntegerVarG(cpGetIntegerVar('VirtualSafetyCarVariable')) = 1;
  oldValue := cpGetIntegerVar('VirtualSafetyCar:OldValue') = 1;
  if (oldValue = value) then
  begin
    exit;
  end;

  EnqueueVirtualSafetyCarEvent(value);
  cpSetIntegerVar('VirtualSafetyCar:OldValue', boolToInt(value));
end;