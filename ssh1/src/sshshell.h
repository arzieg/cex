#include <libssh/libssh.h>

void printerrorcode(int errcode);
int verify_knownhost(ssh_session session);
int authenticate_pubkey(ssh_session session);
int establish_shell_session(ssh_session session, ssh_channel *channel);
int shell_cmd(ssh_channel channel, const char *cmd);