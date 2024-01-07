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

  EnqueueSlotIsFuelingEvent(slot, value);
  cpSetIntegerVar('IsFueling:OldValue:' + IntToStr(slot), boolToInt(value));
end;

procedure checkSlotIsFueling;
var
  slot: Integer;
begin
  if (not isEventActive('Refueling')) then
  begin
    exit;
  end;
  for slot := 1 to cpCountOfSlots do
  begin
    checkSlotIsFuelingByNbr(slot)
  end;
end;