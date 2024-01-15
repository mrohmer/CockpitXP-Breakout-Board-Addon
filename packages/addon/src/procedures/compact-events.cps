procedure ExecCompactEventsOfGroup;
begin
  with newQueryAnw do
  begin
    sql.Clear;

    sql.Text := 'DELETE FROM breakout_board_queue WHERE id NOT IN (SELECT id FROM breakout_board_queue GROUP BY eventGroup HAVING date = MAX(date))';

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