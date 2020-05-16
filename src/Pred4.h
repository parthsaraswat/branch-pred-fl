//========================================================================
// Pred4.h
//========================================================================
// Declarations for the 2LBHT class.
/* +------------------------------------------------------------------------------------------------+
|                                                                                                |
|  +------------------------+                                                                    |
|  |         PC             |                                                                    |
|  +------------------------+                                                                    |
|                 |-----|                                                                        |
|             +---------+                                                                        |
|                |   |     +-------+    +-------+               +-------+                        |
|                |   |     |       |    |       |               |       |                        |
|                |   |     |       |    |       |               |       |                        |
|                |   /m    |       |    |       |               |       |                        |
|                |   |     |       |    |       |               |       |                        |
|                |   |     |       |    |       |               |       |                        |
|                |   +---->+ BHSRT +-+  |  PHT  |   2^k PHTs    |  PHT  |                        |
|                |         |       | |  |       |   <------->   |       |                        |
|                /k        |       | |  |       |               |       |                        |
|                |         |       | |  |       |               |       |                        |
|                |         |       | +-----------------/n-------------->+                        |
|                |         |       |    |       |               |       |                        |
|                |         |       |    |       |               |       |                        |
|                |         +-------+    +-------+  +  +  +  +   +-------+                        |
|                |                          v      v  v  v  v       v                            |
|                |                        XXXXXXXXXXXXXXXXXXXXXXXXXXXX                           |
|                +------------------------->XXXXXXXXXXXXXXXXXXXXXXXX                             |
|                                             XXXXXXXXXXXXXXXXXXXX                               |
|                                                      +                                         |
|                                                      |                                         |
|                                                      |                                         |
|                                                      v                                         |
|                                                 +---------+                                    |
|                                                 |   FSM   |                                    |
|                                                 | Output  |                                    |
|                                                 |  logic  |                                    |
|                                                 +---------+                                    |
|                                                      |                                         |
|                                                      |                                         |
|                                                      v                                         |
|                                                  Prediction!                                   |
|                                                                                                |
+------------------------------------------------------------------------------------------------+
 */



#include <stdio.h>
#include <math.h>
#include <vector>
#include <map>

class Pred4{
  // Member functions
  public:
    // Constructors
    Pred4();
    Pred4( size_t m, size_t n, size_t k );
    // Functions
    bool predict( void* ip );
    void update( void* ip, bool taken );
    void print_contents();

    // Design for test functions
    std::map<char, size_t> get_dims();
    int                    get_state(size_t n, size_t k);
    double get_size();
  // Member variables
  private:
    // m, n, and k
    size_t m_bhsrt_idx;
    size_t m_pht_idx;
    size_t m_pht_select;
    // lengths affected by m, n, and k
    size_t m_bhsrt_length;
    size_t m_pht_length;
    size_t m_no_pht;
    // misc
    enum STATE {ST, T, NT, SNT};
    // data structures
    std::vector<size_t>              bhsrt;
    std::vector<std::vector<STATE> > pht_collection;

};