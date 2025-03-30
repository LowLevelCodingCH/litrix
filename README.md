# Litrix - XK v7
## Note that this is written in german.
## Diese Datei ist in Deutsch geschrieben.
## Historie
```
NitrixBootLD (1)  1.  Version meines Kernels
NitrixGBoot  (2)  2.  Version meines Kernels
NXKRNLLD     (3)  3.  Version meines Kernels
Kerneli      (4)  4.  Version meines Kernels
NxKernV      (5)  5.  Version meines Kernels
NxOS         (6)  6.  Version meines Kernels
SQOS         (7)  7.  Version meines Kernels
ak48         (8)  8.  Version meines Kernels
NXV9         (9)  9.  Version meines Kernels
NitxX0       (10) 10. Version meines Kernels
MergeNITX    (11) 11. Version meines Kernels
xk2          (12) 12. Version meines Kernels
xk3          (13) 13. Version meines Kernels
xk4          (14) 14. Version meines Kernels
xk5          (15) 15. Version meines Kernels
xk6          (16) 16. Die Version mit den meisten Features. (Schlechter code)
litrix/xk7   (17) 17. Jetzige Version. Zweitmeiste Features (Krasser code. Ähnlich wie Linux's)
```

## Features
```
Litrix, wie jedes Betriebssystem (hoffe ich mal), hat ein Syscall interface.
Dieser wird via schreiben zur Adresse 0x1dddddd gewählt.
Nach dem muss man auf 0x1ffffff eine 1 schreiben, sodass der Kernel weiss, dass es etwas zutun hat.
Was der Kernel danach wieder rausspuckt geht in die Addresse 0x1dddddd.

Auch ein Dateisystem unterstützt der Kernel, LiFS, Litrix Datei System.
Dieses benutzt iNodes. Siehe mehr bei `include/litrix/fs/lifs.h`.
Es unterstützt auch SMFS (Simple FS), welches aber nur zum testen existiert.
Siehe mehr bei `include/litrix/fs/smfs.h`.

Es hat auch einen "Scheduler". Es performiert hierbei aber keinen Context-Switch (Kontextaustausch), sondern es ruft einfach eine funktion fürimmer auf. Dumm aber es geht (Nicht).
Ich werde warscheinlich noch sowas wie Round Robin einbauen.

Ausführbare Dateien werden im Arbeitsspeicher bei Addresse 0 (Dezimal) anfangen.
```

## Schon da, aber nicht ganz fertig
Ausführbare Dateien.

## Kommt bald
Round Robin Scheduler.
Context-Switch.
