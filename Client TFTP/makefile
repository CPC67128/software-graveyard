mytftp : mytftp.c
	sh -c 'if test ${OSTYPE} = Linux; then gcc -o mytftp.linux mytftp.c; else gcc -o mytftp.solaris mytftp.c -lsocket -lnsl; fi'
clean :
	\rm mytftp.solaris
	\rm mytftp.linux
