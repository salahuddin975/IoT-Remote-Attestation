
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <picon/shared.h>

#include <picon/injected_code.c>

void __CFI_SET_FDS(int fd_client_to_monitor, int fd_monitor_to_client);
void __CFI_SET_MODULE_ID(uint16_t m_id);

typedef struct {
        uint32_t           len;
        const char * const data;
} __attribute__((packed)) data_t;


static const char _s_cfi[] =
"\x0c\x00\x00\x00"
;

static const char _s_fctids[] =
"\x0c\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x72\x61\x6e\x64"
"\x6f\x6d\x5f\x63\x68\x65\x63\x6b\x73\x75\x6d\x00\x01\x00\x00\x00"
"\x00\x00\x00\x00\x63\x61\x6c\x63\x75\x6c\x61\x74\x65\x5f\x72\x61"
"\x6e\x64\x6f\x6d\x5f\x63\x68\x65\x63\x6b\x73\x75\x6d\x00\x02\x00"
"\x00\x00\x00\x00\x00\x00\x63\x61\x6c\x63\x75\x6c\x61\x74\x65\x5f"
"\x73\x65\x71\x75\x65\x6e\x74\x69\x61\x6c\x5f\x63\x68\x65\x63\x6b"
"\x73\x75\x6d\x00\x03\x00\x00\x00\x00\x00\x00\x00\x63\x61\x6c\x63"
"\x75\x6c\x61\x74\x65\x5f\x63\x68\x65\x63\x6b\x73\x75\x6d\x00\x04"
"\x00\x00\x00\x00\x00\x00\x00\x63\x68\x65\x63\x6b\x73\x75\x6d\x00"
"\x05\x00\x00\x00\x00\x00\x00\x00\x76\x75\x6c\x6e\x65\x72\x61\x62"
"\x6c\x65\x5f\x6d\x61\x69\x6e\x00\x06\x00\x00\x00\x00\x00\x00\x00"
"\x76\x75\x6c\x6e\x65\x72\x61\x62\x6c\x65\x00\x07\x00\x00\x00\x00"
"\x00\x00\x00\x73\x65\x74\x5f\x63\x75\x73\x74\x6f\x6d\x65\x72\x5f"
"\x69\x6e\x66\x6f\x00\x08\x00\x00\x00\x04\x00\x00\x00\x6d\x61\x69"
"\x6e\x00\x09\x00\x00\x00\x00\x00\x00\x00\x61\x63\x63\x65\x70\x74"
"\x5f\x63\x6f\x6d\x6d\x61\x6e\x64\x00\x0a\x00\x00\x00\x00\x00\x00"
"\x00\x6c\x61\x6d\x70\x5f\x6f\x6e\x00\x0b\x00\x00\x00\x00\x00\x00"
"\x00\x6c\x61\x6d\x70\x5f\x6f\x66\x66\x00"
;

static const char _s_callgraph[] =
"\x06\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00"
"\x03\x00\x00\x00\x02\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x00"
"\x04\x00\x00\x00\x01\x00\x00\x00\x03\x00\x00\x00\x06\x00\x00\x00"
"\x01\x00\x00\x00\x05\x00\x00\x00\x08\x00\x00\x00\x02\x00\x00\x00"
"\x04\x00\x00\x00\x09\x00\x00\x00\x09\x00\x00\x00\x02\x00\x00\x00"
"\x06\x00\x00\x00\x07\x00\x00\x00"
;

void __attribute__((visibility("hidden"))) _init_wrapper(void) {
}
void __attribute__((visibility("hidden"))) _fini_wrapper(void) {
}
static void __attribute__((constructor)) _init_cfi(void) {

  int fd_loading_to_monitor;
  int fd_monitor_to_loading;
  int fd_client_to_monitor;
  int fd_monitor_to_client;
  int err = 0;
  loading_packet pkt;
  monitor_to_loading_packet m2l_pkt;
  char *ptr = NULL, *s;

  s = getenv(ENV_FD_LOADING_TO_MONITOR);
  assert(s != NULL  && "ENV_FD_LOADING_TO_MONITOR not set");
  fd_loading_to_monitor = strtol(s, &ptr, 10);
  assert(ptr != NULL  && "invalid ENV_FD_LOADING_TO_MONITOR");
  if (fd_loading_to_monitor < 0) abort();

  s = getenv(ENV_FD_MONITOR_TO_LOADING);
  assert(s != NULL  && "ENV_FD_MONITOR_TO_LOADING not set");
  fd_monitor_to_loading = strtol(s, &ptr, 10);
  assert(ptr != NULL  && "invalid ENV_FD_MONITOR_TO_LOADING");
  if (fd_monitor_to_loading < 0) abort();

  s = getenv(ENV_FD_CLIENT_TO_MONITOR);
  assert(s != NULL  && "ENV_FD_CLIENT_TO_MONITOR not set");
  fd_client_to_monitor = strtol(s, &ptr, 10);
  assert(ptr != NULL  && "invalid ENV_FD_CLIENT_TO_MONITOR");
  if (fd_client_to_monitor < 0) abort();

  s = getenv(ENV_FD_MONITOR_TO_CLIENT);
  assert(s != NULL  && "ENV_FD_MONITOR_TO_CLIENT not set");
  fd_monitor_to_client = strtol(s, &ptr, 10);
  assert(ptr != NULL  && "invalid ENV_FD_MONITOR_TO_CLIENT");
  if (fd_monitor_to_client < 0) abort();

  __CFI_SET_FDS(fd_client_to_monitor, fd_monitor_to_client);

  pkt.event=CFI_LOADING_MODULE_BEGIN;
  pkt.size=sizeof(_s_cfi);
  pkt.value=(loading_packet_value*)_s_cfi;
  WRITE_LOADING_PACKET(fd_loading_to_monitor,pkt,err);
  if (err) abort();

  READ_MONITOR_TO_LOADING_PACKET(fd_monitor_to_loading,m2l_pkt,err);
  if (err) abort();

  __CFI_SET_MODULE_ID(m2l_pkt.id);

  pkt.event=CFI_LOADING_SECTION_FUNCTION_ID;
  pkt.size=sizeof(_s_fctids);
  pkt.value=(loading_packet_value*)_s_fctids;
  WRITE_LOADING_PACKET(fd_loading_to_monitor,pkt,err);
  if (err) abort();

  pkt.event=CFI_LOADING_SECTION_FUNCTION_TRANSITION;
  pkt.size=sizeof(_s_callgraph);
  pkt.value=(loading_packet_value*)_s_callgraph;
  WRITE_LOADING_PACKET(fd_loading_to_monitor,pkt,err);
  if (err) abort();

  



  pkt.event=CFI_LOADING_MODULE_END;
  pkt.size=0;
  pkt.value=NULL;
  WRITE_LOADING_PACKET(fd_loading_to_monitor,pkt,err);
  if (err) abort();



  pkt.event=CFI_LOADING_TERMINATED;
  pkt.size=0;
  pkt.value=NULL;
  WRITE_LOADING_PACKET(fd_loading_to_monitor,pkt,err);
  if (err) abort();

  (void)_init_wrapper();
}
static void __attribute__((destructor)) _fini_cfi(void) {
  (void)_fini_wrapper();
}
