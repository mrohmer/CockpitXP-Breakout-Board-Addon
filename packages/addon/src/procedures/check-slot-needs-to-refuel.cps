procedure checkSlotNeedsToRefuelByNbr(slot: Integer);
var
  value, oldValue: Boolean;
begin
  Cockpit.Slot := slot;

  oldValue := cpGetIntegerVar('NeedsToRefuel:OldValue:' + IntToStr(slot)) = 1;
  value := Cockpit.TankStand <= cpGetIntegerVar('NeedsToRefuelValue');

  if (oldValue = value) then
  begin
    exit;
  end;

  if (value) then
  begin
    EnqueueSlotNeedsFuelingEvent(slot);
  end
  else
  begin
    EnqueueSlotStopsNeedsFuelingEvent(slot);
  end;

  cpSetIntegerVar('NeedsToRefuel:OldValue:' + IntToStr(slot), boolToInt(value));
end;

procedure checkSlotNeedsToRefuel;
var
  slot: Integer;
begin
  if (isEventInactive('NeedsToRefuel')) then
  begin
    exit;
  end;
  for slot := 1 to cpCountOfSlots do
  begin
    checkSlotNeedsToRefuelByNbr(slot)
  end;
end;