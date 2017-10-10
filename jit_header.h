#define TRUE 1 /* TODO: remove this */
#define FALSE 0 /* TODO: remove this */

#include "internal.h"
#include "vm_core.h"
#include "vm_exec.h"
#include "iseq.h"
#include "eval_intern.h"

/* vm.c */
VALUE vm_invoke_bmethod(rb_thread_t *th, rb_proc_t *proc, VALUE self,
			       int argc, const VALUE *argv, VALUE block_handler);

PUREFUNC(static inline const VALUE *VM_EP_LEP(const VALUE *));
static inline const VALUE *
VM_EP_LEP(const VALUE *ep)
{
    while (!VM_ENV_LOCAL_P(ep)) {
        ep = VM_ENV_PREV_EP(ep);
    }
    return ep;
}

static struct rb_captured_block *
VM_CFP_TO_CAPTURED_BLOCK(const rb_control_frame_t *cfp)
{
    VM_ASSERT(!VM_CFP_IN_HEAP_P(GET_THREAD(), cfp));
    return (struct rb_captured_block *)&cfp->self;
}

static int
VM_BH_FROM_CFP_P(VALUE block_handler, const rb_control_frame_t *cfp)
{
    const struct rb_captured_block *captured = VM_CFP_TO_CAPTURED_BLOCK(cfp);
    return VM_TAGGED_PTR_REF(block_handler, 0x03) == captured;
}

static VALUE
vm_passed_block_handler(rb_thread_t *th)
{
    VALUE block_handler = th->passed_block_handler;
    th->passed_block_handler = VM_BLOCK_HANDLER_NONE;
    vm_block_handler_verify(block_handler);
    return block_handler;
}

PUREFUNC(static inline const VALUE *VM_CF_LEP(const rb_control_frame_t * const cfp));
static inline const VALUE *
VM_CF_LEP(const rb_control_frame_t * const cfp)
{
    return VM_EP_LEP(cfp->ep);
}

PUREFUNC(static inline VALUE VM_CF_BLOCK_HANDLER(const rb_control_frame_t * const cfp));
static inline VALUE
VM_CF_BLOCK_HANDLER(const rb_control_frame_t * const cfp)
{
    const VALUE *ep = VM_CF_LEP(cfp);
    return VM_ENV_BLOCK_HANDLER(ep);
}

static inline const rb_control_frame_t *
rb_vm_search_cf_from_ep(const rb_thread_t * const th, const rb_control_frame_t *cfp, const VALUE * const ep)
{
    if (!ep) {
	return NULL;
    }
    else {
	const rb_control_frame_t * const eocfp = RUBY_VM_END_CONTROL_FRAME(th); /* end of control frame pointer */

	while (cfp < eocfp) {
	    if (cfp->ep == ep) {
		return cfp;
	    }
	    cfp = RUBY_VM_PREVIOUS_CONTROL_FRAME(cfp);
	}

	return NULL;
    }
}

extern rb_serial_t ruby_vm_global_constant_state;

#include "vm_insnhelper.h"
#define CJIT_HEADER 1
#include "vm_insnhelper.c"

/* vm_eval.c */
#include "vm_eval.c"

#include "vm_call_iseq_optimized.inc"

/* JIT-only helpers */

/* Optimized version of `vm_getivar` to use IC values without pointer access from JIT-ed code.
   This assumes ivar is already created and class is not changed. */
ALWAYS_INLINE(static VALUE vm_cached_getivar(VALUE, rb_serial_t, size_t));
static inline VALUE
jit_getivar(VALUE self, rb_serial_t ic_serial, size_t index)
{
#if USE_IC_FOR_IVAR
    if (LIKELY(RB_TYPE_P(self, T_OBJECT) && ic_serial == RCLASS_SERIAL(RBASIC(self)->klass))) {
	if (LIKELY(index < ROBJECT_NUMIV(self)))
	    return ROBJECT_IVPTR(self)[index];
    }
#endif	/* USE_IC_FOR_IVAR */
    return Qundef; /* TODO: implement deoptimization */
}
