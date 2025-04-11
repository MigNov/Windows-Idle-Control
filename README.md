# Windows-Idle-Control

Utility to automatically logoff, shutdown, poweroff or reboot Windows machine.

## Settings

The `auto-idle-action.ini` supports following entries:

```
[Settings]
IdleTime = <idle-time-in-seconds>
Action = <logoff|shutdown|poweroff|reboot>
```

## Defaults

If the configuration file is not present it defaults to shutdown after 1h of inactivity.

## Persistency

Upon creation the new entry `AutoShutdown` with the binary name is created under the `HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run`  registry tree.
