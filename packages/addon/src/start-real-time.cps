{ Pascal Script Engine }
{ Event: StartRealTime }
{ Parameter: Keine }


begin
  cpSetIntegerVar('Exit', 0);

  while (True) do
  begin
    if (cpGetIntegerVar('Exit') = 1) then
    begin
      exit;
    end;

    cpSleep(500);
  end;
end.