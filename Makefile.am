SUBDIRS = FeLib Main Script

EXTRA_DIST = LICENSING .customs.emacs

install-data-local:
	-./mkinstalldirs -p $(localstatedir) $(bindir) $(datadir)/ivan/Script/ $(localstatedir)/ivan/bones/ $(localstatedir)/ivan/save/ $(localstatedir)/ivan/dump/ $(localstatedir)/ivan/config/
	-touch $(localstatedir)/ivan/ivan-highscore.scores
	-touch $(localstatedir)/ivan/ivan-logfile
	-chown games.games $(localstatedir)/ivan/ivan-highscore.scores
	-chown games.games $(localstatedir)/ivan/ivan-logfile
	-chmod 664 $(localstatedir)/ivan/ivan-highscore.scores
	-chmod 664 $(localstatedir)/ivan/ivan-logfile
	-chown -R games.games $(localstatedir)/ivan/bones/
	-chown -R games.games $(localstatedir)/ivan/save/
	-chown -R games.games $(localstatedir)/ivan/dump/
	-chown -R games.games $(localstatedir)/ivan/config/
	-chmod -R 776 $(localstatedir)/ivan/config/
	-chmod -R 770 $(localstatedir)/ivan/save/
	-chmod -R u=rwx $(localstatedir)/ivan/bones/
	-chmod -R g=rwx $(localstatedir)/ivan/bones/
	-chmod -R o=r $(localstatedir)/ivan/bones/
	-chmod -R 776 $(localstatedir)/ivan/dump/
	-chgrp games $(bindir)/ivan 
	-chmod 2111 $(bindir)/ivan
