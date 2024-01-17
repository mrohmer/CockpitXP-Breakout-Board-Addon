{ Pascal Script Engine }
{ Event: ChangeRealTimeStatus }
{ Parameter: Cockpit.Parameter = <RennStatus> }

{%%PROCEDURE.EnqueueEvent%%}
{%%PROCEDURE.CheckRaceStatus%%}
{%%PROCEDURE.CheckChaos%%}
{%%FUNCTION.IsEventInactive%%}
{%%FUNCTION.BoolToInt%%}

begin
  checkRaceStatus;
  checkChaos;
end.
