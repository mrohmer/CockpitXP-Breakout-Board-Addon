procedure EnqueueEvent(highLevel: String; decimal: Integer; eventGroup: String);
begin
  with newQueryAnw do
  begin
    sql.Clear;

    sql.Text := 'INSERT INTO breakout_board_queue (highLevel, decimal, eventGroup) VALUES (:highLevel, :decimal, :eventGroup)';
    ParamByName('highLevel').AsString := highLevel;
    ParamByName('decimal').AsInteger := decimal;
    ParamByName('eventGroup').AsString := eventGroup;

    Open;

    Free;
  end;

end;

procedure EnqueueResetAllEvent;
begin
  EnqueueEvent('Reset All', 11, 'Reset');
end;

procedure EnqueueVirtualSafetyCarEvent(isOn: Boolean);
begin
  if (isOn) then
  begin
    EnqueueEvent('Virtual Safety Car On', 21, 'VirtualSafetyCar');
  end
  else
  begin
    EnqueueEvent('Virtual Safety Car Off', 22, 'VirtualSafetyCar');
  end;
end;

procedure EnqueueStartingLightNbrEvent(lightsOn: Integer);
begin
  case lightsOn of
    0: EnqueueEvent('Starting light off', 28, 'StartLight');
    1: EnqueueEvent('Starting light 1', 31, 'StartLight');
    2: EnqueueEvent('Starting light 2', 35, 'StartLight');
    3: EnqueueEvent('Starting light 3', 37, 'StartLight');
    4: EnqueueEvent('Starting light 4', 38, 'StartLight');
    5: EnqueueEvent('Starting light 5', 41, 'StartLight');
  end;
end;

procedure EnqueueStartingLightGreenEvent;
begin
  EnqueueEvent('Starting light Green', 44, 'StartLight');
end;

procedure EnqueueFalseStartEvent;
begin
  EnqueueEvent('False start', 42, 'StartLight');
end;

procedure EnqueueNewTrackRecordEvent;
begin
  EnqueueEvent('New track record', 49, 'TrackRecord');
end;

procedure EnqueueNewSessionRecordEvent;
begin
  EnqueueEvent('New session record', 50, 'SessionRecord');
end;

procedure EnqueuePitlaneEvent(pitlaneNbr: Integer; nbr: Integer);
begin
  if (pitlaneNbr = 1) then
  begin
    case percentage of
      -1: EnqueueEvent('Pitlane 1 off', 55, 'Pitlane1');
      0: EnqueueEvent('Pitlane 1 0%', 59, 'Pitlane1');
      1: EnqueueEvent('Pitlane 1 7%', 61, 'Pitlane1');
      2: EnqueueEvent('Pitlane 1 14%', 62, 'Pitlane1');
      3: EnqueueEvent('Pitlane 1 21%', 73, 'Pitlane1');
      4: EnqueueEvent('Pitlane 1 29%', 74, 'Pitlane1');
      5: EnqueueEvent('Pitlane 1 36%', 75, 'Pitlane1');
      6: EnqueueEvent('Pitlane 1 43%', 76, 'Pitlane1');
      7: EnqueueEvent('Pitlane 1 50%', 77, 'Pitlane1');
      8: EnqueueEvent('Pitlane 1 57%', 78, 'Pitlane1');
      9: EnqueueEvent('Pitlane 1 64%', 79, 'Pitlane1');
      10: EnqueueEvent('Pitlane 1 71%', 81, 'Pitlane1');
      11: EnqueueEvent('Pitlane 1 79%', 82, 'Pitlane1');
      12: EnqueueEvent('Pitlane 1 86%', 83, 'Pitlane1');
      13: EnqueueEvent('Pitlane 1 93%', 84, 'Pitlane1');
      14: EnqueueEvent('Pitlane 1 100%', 85, 'Pitlane1');
    end;
  end
  else if (pitlaneNbr = 2) then
  begin
    case percentage of
      -1: EnqueueEvent('Pitlane 2 off', 89, 'Pitlane2');
      0: EnqueueEvent('Pitlane 2 0%', 90, 'Pitlane2');
      1: EnqueueEvent('Pitlane 2 7%', 91, 'Pitlane2');
      2: EnqueueEvent('Pitlane 2 14%', 92, 'Pitlane2');
      3: EnqueueEvent('Pitlane 2 21%', 93, 'Pitlane2');
      4: EnqueueEvent('Pitlane 2 29%', 94, 'Pitlane2');
      5: EnqueueEvent('Pitlane 2 36%', 95, 'Pitlane2');
      6: EnqueueEvent('Pitlane 2 43%', 97, 'Pitlane2');
      7: EnqueueEvent('Pitlane 2 50%', 98, 'Pitlane2');
      8: EnqueueEvent('Pitlane 2 57%', 99, 'Pitlane2');
      9: EnqueueEvent('Pitlane 2 64%', 100, 'Pitlane2');
      10: EnqueueEvent('Pitlane 2 71%', 101, 'Pitlane2');
      11: EnqueueEvent('Pitlane 2 79%', 102, 'Pitlane2');
      12: EnqueueEvent('Pitlane 2 86%', 103, 'Pitlane2');
      13: EnqueueEvent('Pitlane 2 93%', 105, 'Pitlane2');
      14: EnqueueEvent('Pitlane 2 100%', 106, 'Pitlane2');
    end;
  end;
end;

procedure EnqueueSlotNeedsFuelingEvent(slotId: Integer);
var
  decimal: Integer;
begin
  case slotId of
    1: decimal := 137;
    2: decimal := 139;
    3: decimal := 141;
    4: decimal := 143;
    5: decimal := 146;
    6: decimal := 148;
  else
  begin
    exit;
  end;
  end;

  EnqueueEvent('Slot ' + IntToStr(slotId) + ' nees to refuel', decimal, 'SlotNeedsFueling' + IntToStr(slotId));
end;
procedure EnqueueSlotStopsNeedsFuelingEvent(slotId: Integer);
var
  decimal: Integer;
begin
  case slotId of
    1: decimal := 138;
    2: decimal := 140;
    3: decimal := 142;
    4: decimal := 145;
    5: decimal := 147;
    6: decimal := 149;
  else
  begin
    exit;
  end;
  end;

  EnqueueEvent('Slot ' + IntToStr(slotId) + ' does not need to refuel anymore', decimal, 'SlotNeedsFueling' + IntToStr(slotId));
end;

procedure EnqueueSlotIsFuelingEvent(slotId: Integer; isOn: Boolean);
var
  decimal: Integer;
begin
  case slotId of
    1: decimal := 161;
    2: decimal := 163;
    3: decimal := 165;
    4: decimal := 167;
    5: decimal := 170;
    6: decimal := 172;
    else
    begin
      exit;
    end;
  end;

  EnqueueEvent('Slot ' + IntToStr(slotId) + ' starts refueling', decimal, 'SlotIsFueling' + IntToStr(slotId));
end;

procedure EnqueueSlotStopsFuelingEvent(slotId: Integer; isOn: Boolean);
var
  decimal: Integer;
begin
  case slotId of
    1: decimal := 162;
    2: decimal := 164;
    3: decimal := 166;
    4: decimal := 169;
    5: decimal := 171;
    6: decimal := 173;
    else
    begin
      exit;
    end;
  end;

  EnqueueEvent('Slot ' + IntToStr(slotId) + ' stops refueling', decimal, 'SlotIsFueling' + IntToStr(slotId));
end;
