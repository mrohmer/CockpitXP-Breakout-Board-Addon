procedure EnqueueEvent(highLevel: String; decimal: Integer);
begin
  with newQueryAnw do
  begin
    sql.Clear;

    sql.Text := 'INSERT INTO breakout_board_queue (highLevel, decimal) VALUES (:highLevel, :decimal)';
    ParamByName('highLevel').AsString := highLevel;
    ParamByName('decimal').AsInteger := decimal;

    Open;

    Free;
  end;

end;

procedure EnqueueResetAllEvent;
begin
  EnqueueEvent('Reset All', 11);
end;

procedure EnqueueVirtualSafetyCarEvent(isOn: Boolean);
begin
  if (isOn) then
  begin
    EnqueueEvent('Virtual Safety Car On', 21);
  end
  else
  begin
    EnqueueEvent('Virtual Safety Car Off', 22);
  end;
end;

procedure EnqueueStartingLightNbrEvent(lightsOn: Integer);
begin
  case lightsOn of
    0: EnqueueEvent('Starting light off', 28);
    1: EnqueueEvent('Starting light 1', 31);
    2: EnqueueEvent('Starting light 2', 35);
    3: EnqueueEvent('Starting light 3', 37);
    4: EnqueueEvent('Starting light 4', 38);
    5: EnqueueEvent('Starting light 5', 41);
  end;
end;

procedure EnqueueStartingLightGreenEvent;
begin
  EnqueueEvent('Starting light Green', 44);
end;

procedure EnqueueFalseStartEvent;
begin
  EnqueueEvent('False start', 42);
end;

procedure EnqueueNewTrackRecordEvent;
begin
  EnqueueEvent('New track record', 49);
end;

procedure EnqueueNewSessionRecordEvent;
begin
  EnqueueEvent('New session record', 50);
end;

procedure EnqueuePitlaneEvent(pitlaneNbr: Integer; nbr: Integer);
begin
  if (pitlaneNbr = 1) then
  begin
    case percentage of
      -1: EnqueueEvent('Pitlane 1 off', 55);
      0: EnqueueEvent('Pitlane 1 0%', 59);
      1: EnqueueEvent('Pitlane 1 7%', 61);
      2: EnqueueEvent('Pitlane 1 14%', 62);
      3: EnqueueEvent('Pitlane 1 21%', 73);
      4: EnqueueEvent('Pitlane 1 29%', 74);
      5: EnqueueEvent('Pitlane 1 36%', 75);
      6: EnqueueEvent('Pitlane 1 43%', 76);
      7: EnqueueEvent('Pitlane 1 50%', 77);
      8: EnqueueEvent('Pitlane 1 57%', 78);
      9: EnqueueEvent('Pitlane 1 64%', 79);
      10: EnqueueEvent('Pitlane 1 71%', 81);
      11: EnqueueEvent('Pitlane 1 79%', 82);
      12: EnqueueEvent('Pitlane 1 86%', 83);
      13: EnqueueEvent('Pitlane 1 93%', 84);
      14: EnqueueEvent('Pitlane 1 100%', 85);
    end;
  end
  else if (pitlaneNbr = 2) then
  begin
    case percentage of
      -1: EnqueueEvent('Pitlane 2 off', 89);
      0: EnqueueEvent('Pitlane 2 0%', 90);
      1: EnqueueEvent('Pitlane 2 7%', 91);
      2: EnqueueEvent('Pitlane 2 14%', 92);
      3: EnqueueEvent('Pitlane 2 21%', 93);
      4: EnqueueEvent('Pitlane 2 29%', 94);
      5: EnqueueEvent('Pitlane 2 36%', 95);
      6: EnqueueEvent('Pitlane 2 43%', 97);
      7: EnqueueEvent('Pitlane 2 50%', 98);
      8: EnqueueEvent('Pitlane 2 57%', 99);
      9: EnqueueEvent('Pitlane 2 64%', 100);
      10: EnqueueEvent('Pitlane 2 71%', 101);
      11: EnqueueEvent('Pitlane 2 79%', 102);
      12: EnqueueEvent('Pitlane 2 86%', 103);
      13: EnqueueEvent('Pitlane 2 93%', 105);
      14: EnqueueEvent('Pitlane 2 100%', 106);
    end;
  end;
end;

procedure EnqueueSlotNeedsFuelingEvent(slotId: Integer; isOn: Boolean);
var
  decimal: Integer;
  highLevel: String;
begin
  case slotId of
    1: decimal := 145;
    2: decimal := 153;
    3: decimal := 161;
    4: decimal := 169;
    5: decimal := 177;
    6: decimal := 185;
    else
    begin
      exit;
    end;
  end;

  if (isOn) then
  begin
    highLevel := 'Slot ' + IntToStr(slotId) + ' nees to refuel';
  end
  else
  begin
    highLevel := 'Slot ' + IntToStr(slotId) + ' does not need to refuel anymore';
    decimal := decimal + 1;
  end;

  EnqueueEvent(highLevel, decimal);
end;

procedure EnqueueSlotIsFuelingEvent(slotId: Integer; isOn: Boolean);
var
  decimal: Integer;
  highLevel: String;
begin
  case slotId of
    1: decimal := 147;
    2: decimal := 155;
    3: decimal := 163;
    4: decimal := 171;
    5: decimal := 179;
    6: decimal := 187;
    else
    begin
      exit;
    end;
  end;

  if (isOn) then
  begin
    highLevel := 'Slot ' + IntToStr(slotId) + ' starts refueling';
  end
  else
  begin
    highLevel := 'Slot ' + IntToStr(slotId) + ' stops refueling';
    decimal := decimal + 1;
  end;

  EnqueueEvent(highLevel, decimal);
end;
