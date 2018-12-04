# invoke SourceDir generated makefile for app.pem3
app.pem3: .libraries,app.pem3
.libraries,app.pem3: package/cfg/app_pem3.xdl
	$(MAKE) -f C:\Users\Volpone\Dropbox\CPE403~2\CC1350\Lab_05_sensor\Tools/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\Volpone\Dropbox\CPE403~2\CC1350\Lab_05_sensor\Tools/src/makefile.libs clean

