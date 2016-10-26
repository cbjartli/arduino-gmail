#ifndef STATEMACHINE_H_INCLUDED
#define STATEMACHINE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define NUM_SIGS 4
#define MAX_PENDING_SIG 16
#define TBUF_SIZ 8


typedef enum {
    sig_success = 0,
    sig_err,
    sig_timeout,
    sig_recv
} t_sig_type;

typedef struct {
    t_sig_type  type;
    char        data[TBUF_SIZ];
} t_sig;

extern const t_sig _sig_succ;
extern const t_sig _sig_err;
extern const t_sig _sig_tout;
extern const t_sig _sig_recv;
#define SIGSUCC &_sig_succ
#define SIGERR  &_sig_err
#define SIGTOUT &_sig_tout
#define SIGRECV &_sig_recv

typedef struct t_state t_state; // In order to allow t_state to contain pointer to itself.
typedef struct t_transition t_transition; // Likewise.

struct t_transition {
    /* These must be in enum order. */
    t_state *success;
    t_state *err;
    t_state *timeout;
    t_state *recv;
};

struct t_state {
    void (*onEnter)(void);
    void (*onExit)(void);
    void (*onSignal)(t_sig *);
    t_transition transitions;
};

typedef struct __attribute__((packed)) {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t num;
  char empty[4];
} t_data_recv;

typedef struct {
   t_sig signals[MAX_PENDING_SIG]; 
   uint8_t ring_head;
   uint8_t ring_tail;
} signal_ringbuf;

void sm_dispatch(const t_sig *);
void sm_init(t_state *);
void sm_emit(const t_sig *);
void sm_process_signals();

#ifdef __cplusplus
}
#endif

#endif
