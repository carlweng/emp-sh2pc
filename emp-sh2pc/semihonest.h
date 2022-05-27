#ifndef EMP_SEMIHONEST_H__
#define EMP_SEMIHONEST_H__
#include "emp-sh2pc/sh_gen.h"
#include "emp-sh2pc/sh_eva.h"

namespace emp {

template<typename IO>
inline SemiHonestParty<IO>* setup_semi_honest(IO** ios, int party, int threads, int batch_size = 1024*16) {
	if(party == ALICE) {
		HalfGateGen<IO> * t = new HalfGateGen<IO>(ios[0]);
		CircuitExecution::circ_exec = t;
		ProtocolExecution::prot_exec = new SemiHonestGen<IO>(ios, t, threads);
	} else {
		HalfGateEva<IO> * t = new HalfGateEva<IO>(ios[0]);
		CircuitExecution::circ_exec = t;
		ProtocolExecution::prot_exec = new SemiHonestEva<IO>(ios, t, threads);
	}
	return (SemiHonestParty<IO>*)ProtocolExecution::prot_exec;
}

inline void finalize_semi_honest() {
	delete CircuitExecution::circ_exec;
	delete ProtocolExecution::prot_exec;
}

}
#endif
