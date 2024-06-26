#include <libssh/libssh.h>

void printerrorcode(int errcode);
int verify_knownhost(ssh_session session);