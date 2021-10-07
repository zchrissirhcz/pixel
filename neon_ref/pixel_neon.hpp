#include "pixel_neon_base.hpp"

#include "arithmetic/vadd.hpp"
#include "arithmetic/vaddl.hpp"
#include "arithmetic/vaddw.hpp"
#include "arithmetic/vaddhn.hpp"
#include "arithmetic/vqadd.hpp"
#include "arithmetic/vhadd.hpp"
#include "arithmetic/vrhadd.hpp"

#include "arithmetic/vsub.hpp"
#include "arithmetic/vsubl.hpp"
#include "arithmetic/vsubw.hpp"
#include "arithmetic/vsubhn.hpp"
#include "arithmetic/vqsub.hpp"
#include "arithmetic/vhsub.hpp"
#include "arithmetic/vrsubhn.hpp"

#include "multiply/vmul.hpp"
#include "multiply/vmul_n.hpp"
#include "multiply/vmul_lane.hpp"
#include "multiply/vmull.hpp"
#include "multiply/vmull_n.hpp"
#include "multiply/vmull_lane.hpp"
#include "multiply/vqdmull.hpp"
#include "multiply/vqdmull_n.hpp"
#include "multiply/vqdmull_lane.hpp"
#include "multiply/vqdmulh.hpp"
#include "multiply/vqdmulh_n.hpp"
#include "multiply/vqdmulh_lane.hpp"
#include "multiply/vqrdmulh.hpp"
#include "multiply/vqrdmulh_n.hpp"
#include "multiply/vqrdmulh_lane.hpp"
#include "multiply/vmla.hpp"
#include "multiply/vmla_n.hpp"
#include "multiply/vmla_lane.hpp"
#include "multiply/vmlal.hpp"
#include "multiply/vmlal_n.hpp"
#include "multiply/vmlal_lane.hpp"
#include "multiply/vfma.hpp"
#include "multiply/vqdmlal.hpp"
#include "multiply/vqdmlal_n.hpp"
#include "multiply/vqdmlal_lane.hpp"
#include "multiply/vmls.hpp"
#include "multiply/vmls_n.hpp"
#include "multiply/vmls_lane.hpp"
#include "multiply/vmlsl.hpp"
#include "multiply/vmlsl_n.hpp"
#include "multiply/vfms.hpp"
#include "multiply/vqdmlsl.hpp"
#include "multiply/vqdmlsl_n.hpp"
#include "multiply/vqdmlsl_lane.hpp"

#include "round/vrnd.hpp"
#include "round/vrndn.hpp"
#include "round/vrnda.hpp"
#include "round/vrndp.hpp"
#include "round/vrndm.hpp"

#include "logical_and_compare/vceq.hpp"
#include "logical_and_compare/vcge.hpp"

#include "misc/vdup_n.hpp"