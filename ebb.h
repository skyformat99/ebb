/* Ebb Web Server
 * Copyright (c) 2007 Ry Dahl <ry.d4hl@gmail.com>
 * All rights reserved.
 */

#include <glib.h>
#include "tcp.h"
#include "mongrel/parser.h"

#ifndef ebb_h
#define ebb_h

typedef struct ebb_server ebb_server;
typedef struct ebb_client ebb_client;

#define EBB_LOG_DOMAIN "Ebb"
#define ebb_error(str, ...)  \
  g_log(EBB_LOG_DOMAIN, G_LOG_LEVEL_ERROR, str, ## __VA_ARGS__);


/*** Ebb Server ***/

typedef void (*ebb_request_cb_t)(ebb_client*, void*);

ebb_server* ebb_server_new();
void ebb_server_free(ebb_server*);
void ebb_server_stop(ebb_server*);
void ebb_server_start(ebb_server*, char *host, int port, ebb_request_cb_t, void *request_cb_data);

struct ebb_server {
  tcp_server *socket;
  void *request_cb_data;
  ebb_request_cb_t request_cb;
};

/*** Ebb Client ***/

ebb_client* ebb_client_new(ebb_server *, tcp_client *);
void ebb_client_free(ebb_client *);

struct ebb_client {
  ebb_server *server;
  tcp_client *socket;
  http_parser *parser;
  GQueue *env; /* queue of ebb_env_pairs */
  GString *buffer;
};

typedef struct ebb_env_pair {
  const char *field;
  size_t flen;
  const char *value;
  size_t vlen;
} ebb_env_pair;

ebb_env_pair* ebb_env_pair_new(const char *field, size_t flen, const char *value, size_t vlen);
#define ebb_env_pair_new2(f,v) ebb_env_pair_new(f,strlen(f),v,strlen(v))
#define ebb_env_pair_free(pair) free(pair)

#endif ebb_h
