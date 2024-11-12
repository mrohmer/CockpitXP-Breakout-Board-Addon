{ Pascal Script Engine }
{ Event: ChangeRealTimeStatus }
{ Parameter: Cockpit.Parameter = <RennStatus> }

{%%FUNCTION.GetChaosStatus%%}
{%%FUNCTION.GetFinishedStatus%%}

var
  chaosStatus, finishedStatus: Boolean;
begin
  chaosStatus := getChaosStatus(Cockpit.Parameter);
  finishedStatus := getFinishedStatus(Cockpit.Parameter);

  cpSetIntegerVar('IsChaos', 0);
  cpSetIntegerVar('IsFinishedStatus', 0);

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
  else if (finishedStatus) then
  begin
    cpSetIntegerVar('IsFinished', 1);
    cpSetOutput('Flags1', true);
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
