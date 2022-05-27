#include "emp-sh2pc/emp-sh2pc.h"
using namespace emp;
using namespace std;
int party, threads = 4;

void test_bit(NetIO **ios) {
	NetIO *io = ios[0];
	bool b[] = {true, false};
	int p[] = {PUBLIC, ALICE, BOB};

	for(int i = 0; i < 2; ++i)
		for(int j = 0; j < 3; ++j)
			for(int k = 0; k < 2; ++k)
				for (int l= 0; l < 3; ++l)  {
					{
						Bit b1(b[i], p[j]);
						Bit b2(b[k], p[l]);
						bool res = (b1&b2).reveal(PUBLIC);
						if(res != (b[i] and b[k])) {
							cout<<"AND" <<i<<" "<<j<<" "<<k<<" "<<l<<" "<<res<<endl;
							error("test bit error!");
						}
						res = (b1 & b1).reveal(PUBLIC);
						if (res != b[i]) {
							cout<<"AND" <<i<<" "<<j<<res<<endl;
							error("test bit error!");
						}

						res = (b1 & (!b1)).reveal(PUBLIC);
						if (res) {
							cout<<"AND" <<i<<" "<<j<<res<<endl;
							error("test bit error!");
						}

					}
					{
						Bit b1(b[i], p[j]);
						Bit b2(b[k], p[l]);
						bool res = (b1^b2).reveal(PUBLIC);
						if(res != (b[i] xor b[k])) {
							cout <<"XOR"<<i<<" "<<j<<" "<<k<<" "<<l<< " " <<res<<endl;
							error("test bit error!");
						}

						res = (b1 ^ b1).reveal(PUBLIC);
						if (res) {
							cout<<"XOR" <<i<<" "<<j<<res<<endl;
							error("test bit error!");
						}

						res = (b1 ^ (!b1)).reveal(PUBLIC);
						if (!res) {
							cout<<"XOR" <<i<<" "<<j<<res<<endl;
							error("test bit error!");
						}
					}
					{
						Bit b1(b[i], p[j]);
						Bit b2(b[k], p[l]);
						bool res = (b1^b2).reveal(XOR);
						if(party == ALICE) {
							io->send_data(&res, 1);
						} else {
							bool tmp;io->recv_data(&tmp, 1);
							res = res != tmp;
							if(res != (b[i] xor b[k])) {
								cout <<"XOR"<<i<<" "<<j<<" "<<k<<" "<<l<< " " <<res<<endl;
								error("test bit error!");
							}

						}
					}
				}
	io->flush();
	cout <<"success!"<<endl;
}

int main(int argc, char** argv) {
	int port;
	parse_party_and_port(argv, &party, &port);
	NetIO *ios[threads];
	for(int i = 0; i < threads; ++i)
		ios[i] = new NetIO(party==ALICE?nullptr:"127.0.0.1", port+i);
	setup_semi_honest(ios, party, threads);
	test_bit(ios);
	finalize_semi_honest();
	for(int i = 0; i < threads; ++i)
		delete ios[i];
	return 0;
}
