{ Pascal Script Engine }
{ Event: StartRealTime }
{ Parameter: Keine }

{%%PROCEDURE.InitTable%%}
{%%PROCEDURE.ProcessNextInQueue%%}
{%%PROCEDURE.EnqueueEvent%%}

begin
  cpSetIntegerVar('Exit', 0);

  InitTable();
  EnqueueResetAllEvent();

  while (True) do
  begin
    if (cpGetIntegerVar('Exit') = 1) then
    begin
      exit;
    end;

    ProcessNextInQueue();

    cpSleep(50);
  end;
end.