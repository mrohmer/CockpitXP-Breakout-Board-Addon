procedure ExecCompactEventsOfGroup;
var
  groupStatements: string;
begin
  with newQueryAnw do
  begin
    sql.Clear;

    groupStatements := '';
    for i := 1 to 19 do
    begin
      if (i > 1) then
      begin
        groupStatements := groupStatements + ' OR ';
      end;

      groupStatements := groupStatements + '(id NOT IN (SELECT id FROM breakout_board_queue WHERE eventGroup = :group' + IntToStr(i) + ' ORDER BY date desc LIMIT 1) and eventGroup = :group' + IntToStr(i) + ')';
    end;


    sql.Text := 'DELETE FROM breakout_board_queue WHERE ' + groupStatements;
    ParamByName('group1').AsString := 'VirtualSafetyCar';
    ParamByName('group2').AsString := 'StartLight';
    ParamByName('group3').AsString := 'Reset';
    ParamByName('group4').AsString := 'TrackRecord';
    ParamByName('group5').AsString := 'SessionRecord';
    ParamByName('group6').AsString := 'Pitlane1';
    ParamByName('group7').AsString := 'Pitlane2';
    ParamByName('group8').AsString := 'SlotNeedsFueling1';
    ParamByName('group9').AsString := 'SlotNeedsFueling2';
    ParamByName('group10').AsString := 'SlotNeedsFueling3';
    ParamByName('group11').AsString := 'SlotNeedsFueling4';
    ParamByName('group12').AsString := 'SlotNeedsFueling5';
    ParamByName('group13').AsString := 'SlotNeedsFueling6';
    ParamByName('group14').AsString := 'SlotIsFueling1';
    ParamByName('group15').AsString := 'SlotIsFueling2';
    ParamByName('group16').AsString := 'SlotIsFueling3';
    ParamByName('group17').AsString := 'SlotIsFueling4';
    ParamByName('group18').AsString := 'SlotIsFueling5';
    ParamByName('group19').AsString := 'SlotIsFueling6';

    Open;

    Free;
  end;
end;


// Removes all occurances of one event & only keeps the most recent one
procedure CompactEvents;
begin
  // try
  ExecCompactEventsOfGroup;
  // except
  // end;
end;