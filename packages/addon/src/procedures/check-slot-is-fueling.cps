procedure checkSlotIsFuelingByNbr(slot: Integer);
var
  value, oldValue: Boolean;
begin
  Cockpit.Slot := slot;

  oldValue := cpGetIntegerVar('IsFueling:OldValue:' + IntToStr(slot)) = 1;
  value := Cockpit.EsWirdGetankt;

  if (oldValue = value) then
  begin
    exit;
  end;

  if (value) then
  begin
    EnqueueSlotIsFuelingEvent(slot);
  end
  else
  begin
    EnqueueSlotStopsFuelingEvent(slot);
  end;

  cpSetIntegerVar('IsFueling:OldValue:' + IntToStr(slot), boolToInt(value));
end;

procedure checkSlotIsFueling;
var
  slot: Integer;
begin
  if (isEventInactive('Refueling')) then
  begin
    exit;
  end;
  for slot := 1 to cpCountOfSlots do
  begin
    checkSlotIsFuelingByNbr(slot)
  end;
end;