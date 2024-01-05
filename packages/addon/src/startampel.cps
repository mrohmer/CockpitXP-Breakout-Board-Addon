{ Pascal Script Engine }
{ Event: Startampel }
{ Parameter: Cockpit.Parameter = <Parameter des Events> }

{%%PROCEDURE.EnqueueEvent%%}
begin
  EnqueueStartingLightNbrEvent(Cockpit.Parameter);
end.