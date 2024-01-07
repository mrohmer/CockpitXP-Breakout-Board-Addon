{ Pascal Script Engine }
{ Event: Startampel }
{ Parameter: Cockpit.Parameter = <Parameter des Events> }

{%%PROCEDURE.EnqueueEvent%%}
{%%FUNCTION.IsEventActive%%}

var
  value: Integer;
begin
  if (not isEventActive('StartLight')) then
  begin
    exit;
  end;

  if (cpGetIntegerVar('StartLightParameter1') = Cockpit.Parameter) then
  begin
    value := 1;
  end
  else if (cpGetIntegerVar('StartLightParameter2') = Cockpit.Parameter) then
  begin
    value := 2;
  end
  else if (cpGetIntegerVar('StartLightParameter3') = Cockpit.Parameter) then
  begin
    value := 3;
  end
  else if (cpGetIntegerVar('StartLightParameter4') = Cockpit.Parameter) then
  begin
    value := 4;
  end
  else if (cpGetIntegerVar('StartLightParameter5') = Cockpit.Parameter) then
  begin
    value := 5;
  end
  else
  begin
    exit;
  end;


  EnqueueStartingLightNbrEvent(value);
end.