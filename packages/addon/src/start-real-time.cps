{ Pascal Script Engine }
{ Event: StartRealTime }
{ Parameter: Keine }

{%%PROCEDURE.CheckPitlaneStatus%%}

var
  sleepTime: Integer;
begin
  cpSetIntegerVar('Exit', 0);

  sleepTime := cpGetIntegerVar('TimingSleep');

  while (True) do
  begin
    if (cpGetIntegerVar('Exit') = 1) then
    begin
      exit;
    end;

    checkPitlaneStatus();

    cpSleep(sleepTime);
  end;
end.