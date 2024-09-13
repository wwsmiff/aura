#ifndef AURA_INTERPRETER_H_
#define AURA_INTERPRETER_H_

typedef struct {
} aura_Interpreter_t;

aura_Interpreter_t aura_interpreter_create(void *);
void aura_interpreter_destroy(aura_Interpreter_t *interpreter);

#endif // AURA_INTERPRETER_H_