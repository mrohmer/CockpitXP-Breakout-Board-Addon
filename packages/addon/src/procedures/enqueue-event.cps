procedure EnqueueEvent(package1: String; package2: String; package3: String; highLevel: String; decimal: Integer);
begin
  with newQueryAnw do
  begin
    sql.Clear;

    sql.Text := 'INSERT INTO breakout_board_queue (package1, package2, package3, highLevel, decimal) VALUES (:package1, :package2, :package3, :highLevel, :decimal)';
    ParamByName('package1').AsString = package1;
    ParamByName('package2').AsString = package2;
    ParamByName('package3').AsString = package3;
    ParamByName('highLevel').AsString = highLevel;
    ParamByName('decimal').AsString = decimal;

    Open;

    Free;
  end;

end;


procedure EnqueueResetAllEvent;
begin
  EnqueueEvent('100', '001', '011', 'Reset All', 11);
end;