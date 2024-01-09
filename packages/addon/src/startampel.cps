{ Pascal Script Engine }
{ Event: Startampel }
{ Parameter: Cockpit.Parameter = <Parameter des Events> }

{%%PROCEDURE.EnqueueEvent%%}
{%%FUNCTION.IsEventInactive%%}

var
  value: Integer;
begin
  if (isEventInactive('StartLight')) then
  begin
    exit;
  end;

  if (cpGetIntegerVar('StartLightParameterFalseStart') = Cockpit.Parameter) then
  begin
    EnqueueFalseStartEvent();
    exit;
  end;
  if (cpGetIntegerVar('StartLightParameterGreen') = Cockpit.Parameter) then
  begin
    EnqueueStartingLightGreenEvent();
    exit;
  end;

  if (cpGetIntegerVar('StartLightParameterOff') = Cockpit.Parameter) then
  begin
    value := 0;
  end
  else if (cpGetIntegerVar('StartLightParameter1') = Cockpit.Parameter) then
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