{ Pascal Script Engine }
{ Event: StartRealTime }
{ Parameter: Keine }

{%%PROCEDURE.CheckPitlaneStatus%%}

var
  sleepTime: Integer;
begin
  cpSetIntegerVar('Exit', 0);
  cpSetIntegerVar('IsFinished', 0);

  sleepTime := cpGetIntegerVar('TimingSleep');

  while (True) do
  begin
    if (cpGetIntegerVar('Exit') = 1) then
    begin
      exit;
    end;

    checkPitlaneStatus();

    if (Cockpit.CUStartampelStatus > 1) and (cpGetIntegerVar('IsChaos') = 1) then
    begin
      cpSetOutput('Flags1', false);
      cpSetOutput('Flags2', false);
    end;

    cpSleep(sleepTime);
  end;
end.