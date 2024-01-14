procedure checkRaceStatus;
begin
  if (isEventInactive('RaceState')) then
  begin
    exit;
  end;

  value := cpGetRacingStatus() = 'R';
  oldValue := cpGetIntegerVar('RaceState:OldValue') = 1;
  if (oldValue = value) then
  begin
    exit;
  end;

  EnqueueRaceStateChange(value);
  cpSetIntegerVar('RaceState:OldValue', boolToInt(value));
end;