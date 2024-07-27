#include <stdio.h>  // for getc, printf
#include <stdlib.h> // malloc, free
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "ijvm.h"
#include "util.h" // read this file for debug prints, endianness helper functions
#include "stack.h"
#include "network.h"
#include "heap.h"
#include "ops.h"

// Prototypes to avoid implicit declaration.
word_t get_local_variable(ijvm *m, int i);
void set_local_variable(ijvm *m, int i, word_t value);
word_t get_constant(ijvm *m, int i);

// An array of function pointers
operation_func operations[256] = {
    [OP_BIPUSH] = op_bipush,
    [OP_DUP] = op_dup,
    [OP_IADD] = op_iadd,
    [OP_IAND] = op_iand,
    [OP_IOR] = op_ior,
    [OP_ISUB] = op_isub,
    [OP_NOP] = op_nop,
    [OP_POP] = op_pop,
    [OP_SWAP] = op_swap,
    [OP_ERR] = op_err,
    [OP_HALT] = op_halt,
    [OP_IN] = op_in,
    [OP_OUT] = op_out,
    [OP_GOTO] = op_goto,
    [OP_IFEQ] = op_ifeq,
    [OP_IFLT] = op_iflt,
    [OP_IF_ICMPEQ] = op_if_icmpeq,
    [OP_LDC_W] = op_ldc_w,
    [OP_ILOAD] = op_iload,
    [OP_ISTORE] = op_istore,
    [OP_IINC] = op_iinc,
    [OP_WIDE] = op_wide,
    [OP_INVOKEVIRTUAL] = op_invokevirtual,
    [OP_IRETURN] = op_ireturn,
    [OP_NETBIND] = op_netbind,
    [OP_NETCONNECT] = op_netconnect,
    [OP_NETIN] = op_netin,
    [OP_NETOUT] = op_netout,
    [OP_NETCLOSE] = op_netclose,
    [OP_TAILCALL] = op_tailcall,
    [OP_NEWARRAY] = op_newarray,
    [OP_IALOAD] = op_iaload,
    [OP_IASTORE] = op_iastore,
};

void op_bipush(ijvm *m) {
  int8_t value = m->text_data[m->r.pc++];
  push(m, value);
}

void op_dup(ijvm *m) { push(m, tos(m)); }

void op_iadd(ijvm *m) { push(m, pop(m) + pop(m)); }

void op_iand(ijvm *m) { push(m, pop(m) & pop(m)); }

void op_ior(ijvm *m) { push(m, pop(m) | pop(m)); }

void op_isub(ijvm *m) {
  word_t a = pop(m);
  word_t b = pop(m);
  push(m, b - a); // Note the order: second to top - top
}

void op_nop(ijvm *m) {}

void op_pop(ijvm *m) { pop(m); }

void op_swap(ijvm *m) {
  word_t a = pop(m);
  word_t b = pop(m);
  push(m, a);
  push(m, b);
}

void op_err(ijvm *m) {
  fprintf(m->out, "Unknown error encountered\n");
  m->halting = 1;
}

void op_halt(ijvm *m) { m->halting = 1; }

void op_in(ijvm *m) {
  char ch = fgetc(m->in);
  if (m->in == NULL) {
    fprintf(stderr, "File* in is NULL\n");
    return;
  }
  if (ch == EOF) {
    fprintf(stderr, "Reached EOF\n");
    push(m, 0);
  } else {
    push(m, ch);
  }
}

void op_out(ijvm *m) {
  if (m->stack.sp == -1) {
    fprintf(stderr, "Stack underflow on OUT instruction\n");
    exit(EXIT_FAILURE);
  }
  if (m->out == NULL) {
    fprintf(stderr, "File* out is NULL at PC: %d\n", m->r.pc);
    return;
  }
  fprintf(m->out, "%c", pop(m));
}

void op_goto(ijvm *m) {
  short offset = read_uint16(m->text_data + m->r.pc);
  m->r.pc += offset - 1; // -1 because pc will inc after
}

void op_ifeq(ijvm *m) {
  short offset = read_uint16(m->text_data + m->r.pc);
  word_t value = pop(m);
  if (value == 0)
    m->r.pc += offset - 1;
  else
    m->r.pc += 2;
}

void op_iflt(ijvm *m) {
  short offset = read_uint16(m->text_data + m->r.pc);
  word_t value = pop(m);
  if (value < 0)
    m->r.pc += offset - 1;
  else
    m->r.pc += 2;
}

void op_if_icmpeq(ijvm *m) {
  short offset = read_uint16(m->text_data + m->r.pc);
  word_t value1 = pop(m);
  word_t value2 = pop(m);
  if (value1 == value2)
    m->r.pc += offset - 1;
  else
    m->r.pc += 2;
}

void op_ldc_w(ijvm *m) {
  // Load const at index, push it, then pc += 2 to skip over index.
  short index = read_uint16(m->text_data + m->r.pc);
  word_t constant = get_constant(m, index);
  push(m, constant);
  m->r.pc += 2;
}

void op_iload(ijvm *m) {
  byte_t index = m->text_data[m->r.pc++];
  word_t val = get_local_variable(m, index);
  push(m, val);
}

void op_istore(ijvm *m) {
  byte_t index = m->text_data[m->r.pc++];
  set_local_variable(m, index, pop(m));
}

void op_iinc(ijvm *m) {
  byte_t index = m->text_data[m->r.pc++];
  int8_t increment = m->text_data[m->r.pc++];
  set_local_variable(m, index, get_local_variable(m, index) + increment);
}

void op_wide(ijvm *m) {
  // Get next instruction for WIDE, get index, then move PC past index.
  byte_t next_instruction = m->text_data[m->r.pc++];
  uint16_t index = read_uint16(m->text_data + m->r.pc);
  m->r.pc += 2;

  if (index >= m->stack.sp - m->r.lv) {
    fprintf(stderr, "WIDE out of bounds. Index: %d, Frame locals: %d\n", index,
            m->stack.frame_locals);
  }

  switch (next_instruction) {
  case OP_ILOAD:
    push(m, get_local_variable(m, index));
    break;

  case OP_ISTORE:
    set_local_variable(m, index, pop(m));
    break;

  case OP_IINC: {
    int8_t increment = m->text_data[m->r.pc++];
    set_local_variable(m, index, get_local_variable(m, index) + increment);
    break;
  }

  default:
    fprintf(stderr, "WIDE used with incompatible instruction %02X\n",
            next_instruction);
    m->halting = 1;
  }
}

void op_invokevirtual(ijvm *m) {
  // Function parameters are stored at an index, we grab them and pc += 2
  // to move past INVOKEVIRTUAL and the index that follows.
  // We also fix endianness.
  uint16_t index = read_uint16(m->text_data + m->r.pc);
  m->r.pc += 2;

  word_t method_address = get_constant(m, index);
  uint16_t num_args = read_uint16(m->text_data + method_address);
  uint16_t num_locals = read_uint16(m->text_data + method_address + 2);
  method_address += 4;

  ////////////////////
  // For the debugger. Has no interpreter functionality and is for printing.
  // Hit MAX_FRAMES? Double it.
  if (m->call_stack.top >= m->MAX_FRAMES - 1) {
    extend_frames(m);
  }

  // Move up call_stack to new empty frame, and then populate it.
  CallStackFrame *frame = &m->call_stack.frames[++m->call_stack.top];
  frame->return_address = m->r.pc;
  frame->num_args = num_args;
  frame->num_locals = num_locals;
  frame->local_variables = malloc((num_args + num_locals) * sizeof(word_t));
  if (frame->local_variables == NULL) {
    fprintf(stderr, "Failed to allocate memory for local variables\n");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < num_args + num_locals; i++) {
    frame->local_variables[i] = m->stack.items[m->r.lv + i];
  }

  // for print_stack(ijvm*)
  m->stack.frame_locals = num_locals;
  ////////////////////

  // LV points to base of stack frame, and SP makes room for locals.
  // Note SP doesn't init locals to 0, maybe for hardening :thinking:
  word_t new_lv = m->stack.sp - num_args + 1;
  m->stack.sp += num_locals;

  // Save old PC and LV - the prologue. Also set link_ptr to SP.
  push(m, m->r.pc);
  m->stack.items[new_lv] = m->stack.sp;
  push(m, m->r.lv);

  // Set LV and PC.
  m->r.lv = new_lv;
  m->r.pc = method_address;
}

void op_ireturn(ijvm *m) {
  // If LV = 0 we're in main, so don't return.
  if (m->r.lv == 0) {
    printf(stderr, "IRETURN called in main.");
    return;
  }

  word_t return_value = pop(m);

  // Move SP to LV + 1, save old LV.
  word_t new_sp = m->stack.items[m->r.lv];
  word_t old_lv = m->r.lv;
  m->stack.sp = new_sp + 1;

  // Restore LV, PC, and SP.
  m->r.lv = pop(m);
  m->r.pc = pop(m);
  m->stack.sp = old_lv;

  // Discard OBJREF and push return value.
  pop(m);
  push(m, return_value);

  // For the debugger. Has no interpreter functionality and is for printing.
  // 'Pop' the call_stack.
  if (m->call_stack.top >= 0) {
    m->call_stack.top--;
  } else {
    fprintf(stderr, "Call stack underflow\n");
    exit(EXIT_FAILURE);
  }
}

void op_netbind(ijvm *m) {
  // To test binding, `echo -n "AB" | nc 127.0.0.1 5555`
  int port = pop(m);
  // IPv4 TCP.
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in server_addr, client_addr;

  if (sock < 0) {
    push(m, 0);
    return;
  }

  // htonl and htons give us this correctly formatted, otherwise our ints are
  // just incorrect.
  // We memset because it's insecure to potentially grab garbage as an addr.
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(port);

  // If this fails it returns -1 and executes the block
  if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr))) {
    printf(stderr, "Bind failed.");
    close(sock);
    exit(EXIT_FAILURE);
    return;
  }

  // Keep listening forever :D
  if (listen(sock, 5) < 0) {
    printf(stderr, "Listen failed.");
    close(sock);
    exit(EXIT_FAILURE);
  }

  socklen_t len = sizeof(client_addr);
  int conn = accept(sock, (struct sockaddr *)&client_addr, &len);
  if (conn < 0) {
    close(sock);
    push(m, 0);
    return;
  }

  // conn_index -1 indicates failure, so we close everything.
  // Otherwise we save the details of our connection.
  int conn_index = find_free_connection(m);
  if (conn_index == -1) {
    close(sock);
    close(conn);
    push(m, 0);
  } else {
    m->conn[conn_index].sock = sock;
    m->conn[conn_index].conn = conn;
    m->conn[conn_index].used = 1;
    push(m, conn_index + 1);
  }
}

void op_netconnect(ijvm *m) {
  // To test connecting, python3 net_test.py
  int port = pop(m);
  word_t host = pop(m);

  // Again, htonl and htons to properly get host and port.
  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(host);
  server_addr.sin_port = htons(port);

  // IPv4 TCP.
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    printf(stderr, "Socket creation failed");
    exit(EXIT_FAILURE);
  }

  if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    printf(stderr, "Connect failed");
    close(sock);
    exit(EXIT_FAILURE);

    // No conn, as opposed to NETCONNECT.
    // We can use this for NETIN and NETOUT to differentiate whether
    // or not we're a server or client.
  } else {
    int conn_index = find_free_connection(m);
    if (conn_index == -1) {
      close(sock);
      push(m, 0);
    } else {
      m->conn[conn_index].sock = sock;
      m->conn[conn_index].used = true;
      push(m, conn_index + 1);
    }
  }
}

void op_netin(ijvm *m) {
  int netref = pop(m) - 1;

  // If errored netref, too many connections, or an unused connection
  if (netref < 0 || netref >= m->MAX_CONNECTIONS || !m->conn[netref].used) {
    push(m, 0);
    return;
  }

  char ch;
  // for signed size_t, -1 means an error, so we can detect if we don't set it.
  ssize_t n = -1;

  if (m->conn[netref].conn == 0) {
    // Client
    n = recv(m->conn[netref].sock, &ch, 1, 0);
  } else {
    // Server
    n = recv(m->conn[netref].conn, &ch, 1, 0);
  }

  if (n < 0) {
    printf(stderr, "recv failed");
    push(m, 0);
  } else if (n == 0) {
    printf("Connection closed by peer\n");
    push(m, 0);
  } else {
    printf("GOT: %c\n", ch);
    push(m, ch);
  }
}

void op_netout(ijvm *m) {
  int netref = pop(m) - 1; // -1 for connections index
  char ch = (char)pop(m);

  // If errored netref, too many connections, or an unused connection.
  if (netref < 0 || netref >= m->MAX_CONNECTIONS || !m->conn[netref].used) {
    return;
  }

  if (m->conn[netref].conn == 0) {
    // Client
    send(m->conn[netref].sock, &ch, 1, 0);
  } else {
    // Server
    send(m->conn[netref].conn, &ch, 1, 0);
  }
}

void op_netclose(ijvm *m) {
  int netref = pop(m) - 1; // -1 for connections index

  // If valid netref, not an extraneous connection, and used,
  // close client and server and set used to false.
  if (netref >= 0 && netref < m->MAX_CONNECTIONS && m->conn[netref].used) {
    close(m->conn[netref].conn);
    close(m->conn[netref].sock);
    m->conn[netref].used = false;
  }
}

void op_tailcall(ijvm *m) {
  // no idea lol
}

void op_newarray(ijvm *m) {
  int count = pop(m);
  int array_ref = allocate_array(m, count);
  push(m, array_ref);
}

void op_iaload(ijvm *m) {
  int array_ref = pop(m);
  int index = pop(m);

  if (array_ref < 0 || array_ref >= m->h.count) {
    fprintf(stderr, "Array reference out of bounds\n");
    m->halting = 1;
    return;
  }
  if (index < 0 || index >= m->h.sizes[array_ref]) {
    fprintf(stderr, "Array index out of bounds\n");
    m->halting = 1;
    return;
  }
  push(m, m->h.arrays[array_ref][index]);
}

void op_iastore(ijvm *m) {
  int array_ref = pop(m);
  int index = pop(m);
  int value = pop(m);

  if (array_ref < 0 || array_ref >= m->h.count) {
    fprintf(stderr, "Array reference out of bounds\n");
    m->halting = 1;
    return;
  }
  if (index < 0 || index >= m->h.sizes[array_ref]) {
    fprintf(stderr, "Array index out of bounds\n");
    m->halting = 1;
    return;
  }
  m->h.arrays[array_ref][index] = value;
}
