{ Pascal Script Engine }
{ Event: ChangeRealTimeStatus }
{ Parameter: Cockpit.Parameter = <RennStatus> }

{%%PROCEDURE.EnqueueEvent%%}
{%%FUNCTION.IsEventInactive%%}
{%%FUNCTION.CheckRaceStatus%%}

begin
  checkRaceStatus;
end.
