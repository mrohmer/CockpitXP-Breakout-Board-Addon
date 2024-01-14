procedure DropTable;
begin
  with newQueryAnw do
  begin
    sql.Clear;

    sql.Text := 'DROP TABLE IF EXISTS breakout_board_queue';

    Open;

    Free;
  end;
end;

procedure CreateTable;
begin
  with newQueryAnw do
  begin
    sql.Clear;

    sql.Text := 'CREATE TABLE breakout_board_queue (id INTEGER PRIMARY KEY, date DATETIME DEFAULT CURRENT_TIMESTAMP, package1 STRING, package2 STRING, package3 STRING, highLevel STRING, decimal INTEGER, eventGroup STRING)';

    Open;

    Free;
  end;
end;


procedure InitTable;
begin
  // Drop the table to have a clean queue
  DropTable();
  CreateTable();
end;