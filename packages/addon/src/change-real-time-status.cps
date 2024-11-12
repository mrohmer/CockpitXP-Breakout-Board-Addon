{ Pascal Script Engine }
{ Event: ChangeRealTimeStatus }
{ Parameter: Cockpit.Parameter = <RennStatus> }

{%%FUNCTION.GetRaceStatus%%}
{%%FUNCTION.GetChaosStatus%%}

var
  raceStatus, chaosStatus: Boolean;
begin
  raceStatus := getRaceStatus();
  chaosStatus := getChaosStatus(Cockpit.Parameter);

  cpSetIntegerVar('IsChaos', 0);

  if (Cockpit.Parameter = 0) then
  begin
    cpSetOutput('Flags1', false);
    cpSetOutput('Flags2', false);
  end
  else if (chaosStatus) then
  begin
    cpSetIntegerVar('IsChaos', 1);
    cpSetOutput('Flags1', false);
    cpSetOutput('Flags2', true);
  end
  else if (Cockpit.Parameter = 1) then
  begin
    cpSetOutput('Flags1', true);
    cpSetOutput('Flags2', false);
  end
  else
  begin
    cpSetOutput('Flags1', false);
    cpSetOutput('Flags2', false);
  end;
end.
