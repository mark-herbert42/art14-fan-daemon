PREFIX   = /usr/local

bindir   = $(PREFIX)/bin
confdir  = $(PREFIX)/etc
sharedir = $(PREFIX)/share
sysddir  = $(PREFIX)/lib/systemd/system

CC = g++
CFLAGS   = -Os
LDFLAGS  = -s

PROGS = src/art14_fan_control

all: $(PROGS)

install-progs:   src/art14_fan_control
	install -Dm 755 src/art14_fan_control          $(DESTDIR)$(bindir)/art14_fan_control


install-systemd:    art14_fan_control.service
	# /usr/local/lib/systemd/system
	install -Dm 644 art14_fan_control.service     $(DESTDIR)$(sysddir)/art14_fan_control.service

install: install-progs install-systemd

uninstall:
	# Binaries
	rm -f $(DESTDIR)$(bindir)/art14_fan_control

	# /usr/local/lib/systemd/system
	rm -f $(DESTDIR)$(sysddir)/art14_fan_control.service	

clean:
	rm -f  src/*.o src/art14_fan_control

# =============================================================================
# Binaries ====================================================================
# =============================================================================


src/art14_fan_control: \
	src/daemon.cpp 
	$(CC) $(CPPFLAGS) $(CFLAGS) src/daemon.cpp -o src/art14_fan_control $(LDLIBS) $(LDFLAGS)

