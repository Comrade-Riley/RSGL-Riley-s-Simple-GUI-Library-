VERSION = main
all:
	@echo "make sure to set VERSION to your current VERSION with vi 'MakeFile'"
	@echo ""
	@echo "this makeFile is for linux only"
	@echo ""
	@echo "do 'make install' to install"
	@echo " "
	@echo "do 'make update' to update"
	@echo " "
	@echo "do 'make remove' to remove"


install:
	mv ../RSGL-Riley-s-Simple-GUI-Library--$(VERSION) /usr/include/RSGL
update:
	rm -r /usr/include/RSGL
	mv ../RSGL-Riley-s-Simple-GUI-Library--$(VERSION) /usr/include/RSGL
remove:
	rm -r /usr/include/RSGL
