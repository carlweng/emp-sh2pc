#ifndef EMP_SH_PARTY_H__
#define EMP_SH_PARTY_H__
#include <algorithm>
#include "emp-tool/emp-tool.h"
#include "emp-ot/emp-ot.h"

namespace emp {

template<typename IO>
class SemiHonestParty: public ProtocolExecution { public:
	int party;
	IO* io = nullptr;
	FerretCOT<IO> * ot = nullptr;
	PRG shared_prg;

	block * buf = nullptr;
	bool * buff = nullptr;
	int top = 0;
	int batch_size = 1024*16;

	SemiHonestParty(IO ** ios, int threads, int party) : ProtocolExecution(party) {
		this->party = party;
		this->io = ios[0];
		ot = new FerretCOT<IO>(party, threads, ios, false, false);
		buf = new block[batch_size];
		buff = new bool[batch_size];
	}
	void set_batch_size(int size) {
		delete[] buf;
		delete[] buff;
		batch_size = size;
		buf = new block[batch_size];
		buff = new bool[batch_size];
	}

	~SemiHonestParty() {
		delete[] buf;
		delete[] buff;
		delete ot;
		std::remove((this->party==ALICE?PRE_OT_DATA_REG_SEND_FILE:PRE_OT_DATA_REG_RECV_FILE).c_str()); // TODO: temporary solution
	}
};
}
#endif
