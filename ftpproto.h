#ifndef _FTP_PROTO_H_
#define _FTP_PROTO_H_

#include"session.h"

int list_common(session_t *sess);
void handle_child(session_t *sess);

#endif
