{ Pascal Script Engine }
{ Event: ChangeRealTimeStatus }
{ Parameter: Cockpit.Parameter = <RennStatus> }

{%%PROCEDURE.EnqueueEvent%%}
{%%PROCEDURE.CheckRaceStatus%%}
{%%FUNCTION.IsEventInactive%%}
{%%FUNCTION.BoolToInt%%}

begin
  checkRaceStatus;
end.
