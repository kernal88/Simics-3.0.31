/*
  gc.c - g-cache-ooo class

  Copyright 2003-2007 Virtutech AB
  
  The contents herein are Source Code which are a subset of Licensed
  Software pursuant to the terms of the Virtutech Simics Software
  License Agreement (the "Agreement"), and are being distributed under
  the Agreement.  You should have received a copy of the Agreement with
  this Licensed Software; if not, please contact Virtutech for a copy
  of the Agreement prior to using this Licensed Software.
  
  By using this Source Code, you agree to be bound by all of the terms
  of the Agreement, and use of this Source Code is subject to the terms
  the Agreement.
  
  This Source Code and any derivatives thereof are provided on an "as
  is" basis.  Virtutech makes no warranties with respect to the Source
  Code or any derivatives thereof and disclaims all implied warranties,
  including, without limitation, warranties of merchantability and
  fitness for a particular purpose and non-infringement.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <simics/api.h>
#include <simics/alloc.h>
#include <simics/utils.h>

/* cache includes */
#include "gc-common.h"
#include "gc.h"

/* include the generic code */
#include "gc-specialize.c"

/*
  Information for class registering
*/
static conf_class_t *gc_class;
static class_data_t gc_data;
static timing_model_interface_t gc_ifc;

DLL_EXPORT void
init_local(void)
{
        gc_data.new_instance = gc_new_instance;
        gc_data.description =
                "g-cache-ooo is an out-of-order cache model with the following "
                "features: "
                "configurable number of lines, line size, associativity; "
                "indexing/tagging on physical/virtual addresses; "
                "configurable policies for write-allocate and write-back; "
                "random, cyclic and lru replacement policies; "
                "several caches can be connected in chain; "
                "a single cache can be connected to several processors; "
                "multiple out-of-order transactions support.";
                
        if (!(gc_class = SIM_register_class("g-cache-ooo", 
                                            &gc_data))) {
                pr_err("Could not create g-cache-ooo class\n");
        } else {

                /* register the attributes */
                gc_common_register(gc_class);
                gc_register(gc_class);

                /* register the timing model interface */
                gc_ifc.operate = gc_operate;
                SIM_register_interface(gc_class, "timing-model", &gc_ifc);

                /* add the possiblereplacement policies */
                add_repl_policy(init_rand_repl(gc_class));
                add_repl_policy(init_lru_repl(gc_class));

                /* register the log groups */
                SIM_log_register_groups(gc_class, gc_log_groups);
        }

        /* register the other classes */
        ts_init_local();
        sort_init_local();
}
