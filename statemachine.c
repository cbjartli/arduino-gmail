#include "statemachine.h"
#include <stdint.h>
#include <stdbool.h>


t_state *state; 
signal_ringbuf queue;
t_state s_fail, s_init, s_init_timer, s_init_wifi, s_init_udp, s_receive, s_listen;

const t_sig  _sig_succ = { .type = sig_success };
const t_sig  _sig_err = { .type = sig_err };
const t_sig  _sig_tout = { .type = sig_timeout };
const t_sig  _sig_recv = { .type = sig_recv };

/* Based on http://www.downtowndougbrown.com/2013/01/microcontrollers-interrupt-safe-ring-buffers/ */
bool queue_signal(const t_sig *s) {
    uint8_t next_head = (queue.ring_head + 1) % MAX_PENDING_SIG;
    if (next_head != queue.ring_tail) {
        /* There is room */
        queue.signals[queue.ring_head] = *s;
        queue.ring_head = next_head;
        return true;
    } else {
        /* No room */
        return false;
    }
}

bool dequeue_signal(t_sig *s) {
    if (queue.ring_head != queue.ring_tail) {
        *s = queue.signals[queue.ring_tail];
        queue.ring_tail = (queue.ring_tail + 1) % MAX_PENDING_SIG;
        return true;
    } else {
        return false;
    }
}

void sm_init(t_state *s) {
    state = s;
    s->onEnter();
}


void sm_dispatch(const t_sig *s) {
  if (s->type >= NUM_SIGS) {
    return;
  }

  // NOTE: This makes assumptions about the memory layout of t_transition! 
  // It assumes that it is laid out like a contiguous array of t_state*, in the order defined by t_sig_type.
  t_state *transition_state = *((t_state **)&state->transitions + s->type); 

  bool transition = transition_state && (state != transition_state);
  
  if (transition && state->onExit) {
    state->onExit();
  }

  if (state->onSignal) {
      state->onSignal(s);
  }

  if (transition_state) {
      state = transition_state;
  }

  if (transition && state->onEnter) {
    state->onEnter();
  }
}

void sm_emit(const t_sig *s) {
    queue_signal(s);
}

void sm_process_signals(){
    t_sig s;

    if (dequeue_signal(&s)) {
        sm_dispatch(&s);
    }
}
